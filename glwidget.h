#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "trianglemesh.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
	GLWidget(QWidget *parent);
	~GLWidget();

	void loadMesh(const QString &filename);
	void setPolygonMode(bool bFill);
    void compute_curvatures();
    void select_gauss_mean(bool b);
    void set_reflection_lines(bool b);
    void laplacian_operator();


    //comunication
    TriangleMesh& accesMesh();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void setProjection(float aspect);
	void setModelview();

private:
	bool bPolygonFill;

    //curvatures
    bool curvtures_active = false;
    bool select_gauss = true;
    bool bReflectionLines =false;



	float angleX, angleY, distance;
	QPoint lastMousePos;

	QOpenGLShaderProgram *program;
	TriangleMesh mesh;

};

#endif // GLWIDGET_H
