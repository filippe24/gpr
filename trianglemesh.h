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

    void computeCornerTables();
    void computeVertexCorners();

    void principalCurvatures();


private:
    void buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors);
    void fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors);

private:
	vector<QVector3D> vertices;
	vector<int> triangles;


	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vboVertices, vboNormals, eboTriangles;
    QOpenGLBuffer vboCurvatureColors, vboColorGauss, vboColorMean;

    QVector3D boundingBoxMax;
    QVector3D boundingBoxMin;



    vector<float> gaussianCurvature;
    vector<float> meanCurvature;

};


#endif // TRIANGLEMESH_H
