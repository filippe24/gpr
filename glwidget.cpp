#include "glwidget.h"
#include <iostream>
#include <QApplication>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <plyreader.h>



using namespace std;


const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 3.0f;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), bPolygonFill(true), angleX(0.0f), angleY(0.0f), distance(2.0f)
{
	program = NULL;
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    setFormat(format);

}

GLWidget::~GLWidget()
{
	if(program) delete program;
}


void GLWidget::initializeGL()
{
    std::cout << "INITIALIZE GL" << std::endl;

	initializeOpenGLFunctions();

	program = new QOpenGLShaderProgram();
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");
	program->link();
	if(!program->isLinked())
	{
			cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
			QApplication::quit();
	}
	program->bind();

	mesh.buildCube();
	if(!mesh.init(program))
	{
			cout << "Could not create vbo" << endl;
			QApplication::quit();
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{
    std::cout << "RESIZE GL" << std::endl;

	glViewport(0,0,w,h);
	setProjection((float)w/h);
	setModelview();
}

void GLWidget::paintGL()
{

    std::cout << "PAINT GL" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->bind();
	program->setUniformValue("bLighting", bPolygonFill);

    //curvature
    program->setUniformValue("bCurvature", curvtures_active);
    program->setUniformValue("select_gauss", select_gauss);

    //reflection lines
    program->setUniformValue("bReflectionLines", bReflectionLines);

    if(curvtures_active)
    {
        program->setUniformValue("min_gauss", mesh.min_gauss);
        program->setUniformValue("max_gauss", mesh.max_gauss);
        program->setUniformValue("min_mean", mesh.min_mean);
        program->setUniformValue("max_mean", mesh.max_mean);
    }




	if(bPolygonFill)
		program->setUniformValue("color", QVector4D(0.75, 0.8, 0.9, 1.0));
	else
	{
		program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//cube.render(*this);
		mesh.render(*this);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        program->setUniformValue("color", QVector4D(0.05, 0.05, 0.15, 1.0));
	}
	//cube.render(*this);
	mesh.render(*this);
	program->release();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Rotation
	if(event->buttons() & Qt::LeftButton)
	{
		angleX += rotationFactor * (event->y() - lastMousePos.y());
		angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
		angleY += rotationFactor * (event->x() - lastMousePos.x());
	}
	// Zoom
	if(event->buttons() & Qt::RightButton)
	{
		distance += 0.01f * (event->y() - lastMousePos.y());
		distance = max(minDistanceCamera, min(distance, maxDistanceCamera));
	}

	lastMousePos = event->pos();

	makeCurrent();
	setModelview();
	doneCurrent();
	update();
}

void GLWidget::setProjection(float aspect)
{
	QMatrix4x4 projectionMatrix;

	projectionMatrix.perspective(60, aspect, 0.01, 100.0);
	program->bind();
	program->setUniformValue("projection", projectionMatrix);
	program->release();
}

void GLWidget::setModelview()
{
	QMatrix4x4 modelviewMatrix;

	modelviewMatrix.translate(0, 0, -distance);
	modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
	program->bind();
	program->setUniformValue("modelview", modelviewMatrix);
	program->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
	program->release();
}

void GLWidget::setPolygonMode(bool bFill)
{
	bPolygonFill = bFill;

	makeCurrent();
	if(bFill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	doneCurrent();
	update();
}

void GLWidget::loadMesh(const QString &filename)
{
    std::cout << "LOAD MESH" << std::endl;

	PLYReader reader;

	mesh.destroy();
	reader.readMesh(filename, mesh);
	makeCurrent();
	if(!mesh.init(program))
	{
			cout << "Could not create vbo" << endl;
			QApplication::quit();
	}
	doneCurrent();
	update();
}
void GLWidget::compute_curvatures()
{
    curvtures_active = !curvtures_active;
    update();
}

void GLWidget::select_gauss_mean(bool b)
{
    select_gauss = b;
    update();
    std::cout << " print gauss ?? " << b << std::endl;

    std::cout << " gauss max: " << mesh.max_gauss << ", gauss min: " << mesh.min_gauss << ".  mean max:" << mesh.max_mean << ", mean min: " << mesh.min_mean << std::endl;
}


void GLWidget::set_reflection_lines(bool b)
{
    bReflectionLines = b;
    update();
}


void GLWidget::laplacian_operator()
{
    mesh.computeLaplacianOperator();
    mesh.applyLaplacian();
    update();
}
