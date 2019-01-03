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
    enum WeightType
    {
        UNIFORM,
        COTANGENT
    };

public:
	void addVertex(const QVector3D &position);
	void addTriangle(int v0, int v1, int v2);

	void buildCube();

	bool init(QOpenGLShaderProgram *program);
    bool buildMesh();
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



    //COMUNICATION AND UPDATES:
    void setLambda(float value);
    void setDefault();
    void setLaplacianMode(WeightType selectedType);
    int getVerticesSize();


    //FUNCTION GENERAL USAGE:
    void computeRing(vector<uint> &ring, uint ver);
    float computeCotangent(uint ver, vector<uint> ring, uint r, bool alternative = false);



private:
    void buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors);
    void fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors);

private:
    QOpenGLShaderProgram *active_program;

	vector<QVector3D> vertices;
	vector<int> triangles;


	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vboVertices, vboNormals, eboTriangles;
    QOpenGLBuffer vboCurvatureColors, vboColorGauss, vboColorMean;








    //LAB 1: Gaussian and Mean Curvatures:
    vector<float> gaussianCurvature;
    vector<float> meanCurvature;
    QVector3D boundingBoxMax;
    QVector3D boundingBoxMin;


    //LAB 2: Laplacian operators
    vector<QVector3D> original_vertices;

    vector<QVector3D> laplace_operators;
    vector<QVector3D> laplace_vertices;
    float lambda = 1.0;

    WeightType current_weight_type = WeightType::UNIFORM;
    float computeWeight(int v, vector<uint> ring, uint r, bool cotang_type = false);





};


#endif // TRIANGLEMESH_H
