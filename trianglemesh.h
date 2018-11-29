#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <glm/glm.hpp>



using namespace std;


class TriangleMesh
{

public:
	TriangleMesh();

public:
	void addVertex(const QVector3D &position);
	void addTriangle(int v0, int v1, int v2);

	void buildCube();

	bool init(QOpenGLShaderProgram *program);
	void destroy();

	void render(QOpenGLFunctions &gl);


    //LAB 1: Gaussian and Mean Curvatures:
    void computeCornerTables();
    void computeVertexCorners();
    void principalCurvatures();

    //min and max values
    float max_gauss = 0.0;
    float min_gauss = 0.0;
    float max_mean = 0.0;
    float min_mean = 0.0;

    //LAB 2: Laplacian operators
    void computeLaplacianOperator();
    void applyLaplacian();


private:
    void buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors);
    void fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors);

private:
	vector<QVector3D> vertices;
	vector<int> triangles;


	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vboVertices, vboNormals, eboTriangles;
    QOpenGLBuffer vboCurvatureColors, vboColorGauss, vboColorMean;


    void setDefault();



    //LAB 1: Gaussian and Mean Curvatures:
    vector<float> gaussianCurvature;
    vector<float> meanCurvature;
    QVector3D boundingBoxMax;
    QVector3D boundingBoxMin;


    //LAB 2: Laplacian operators
    vector<QVector3D> laplace_operators;
    vector<QVector3D> laplace_vertices;
    vector<QVector3D> old_vertices;
    bool laplacianON = false;
    float lambda = 1.0;


};


#endif // TRIANGLEMESH_H
