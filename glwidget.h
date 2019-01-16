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

    //lab1: curvatures
    void compute_curvatures();
    void set_selected_gauss(bool b);
    bool get_is_gauss();
    void set_reflection_lines(bool b);
    float mean_weight = 0.1f;
    float gauss_weight = 0.2f;


    //lab2: iterative smoothing
    void laplacian_operator(bool isCotangent);

    //lab3: global smoothing
    void global_smoothing(bool isCotangent);

    //lab4: magnify
    void magnify_details(bool isCotangent);

    //lab5:parametrization
    bool parametrization_on = false;

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

	bool bPolygonFill;

    //curvatures
    bool curvtures_active = false;
    bool select_gauss = true;
    bool bReflectionLines =false;

	float angleX, angleY, distance;
    QPoint lastMousePos;

	QOpenGLShaderProgram *program;

    //mesh
    TriangleMesh mesh;


};

#endif // GLWIDGET_H
