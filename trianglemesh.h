#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <math.h>
#include <iostream>
#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>




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
    enum IterativeType
    {
        NORMAL,
        BILAPLACIAN,
        TAUBIN
    };
public:
	void addVertex(const QVector3D &position);
	void addTriangle(int v0, int v1, int v2);

	void buildCube();

	bool init(QOpenGLShaderProgram *program);
    bool buildMesh();
	void destroy();
    void updateVBOCurvatures();

	void render(QOpenGLFunctions &gl);


    //LAB 1: Gaussian and Mean Curvatures:
    void computeCornerTables();
    void computeVertexCorners();
    void principalCurvatures();
    bool curvatureON = false;

    //min and max values
    float max_gauss = 0.0;
    float min_gauss = 0.0;
    float max_mean = 0.0;
    float min_mean = 0.0;

    //LAB 2: Laplacian operators
    void setCurrentIterativeType(IterativeType t){current_iterative_type = t;}
    void setCurrentIterativeType(int t){current_iterative_type = IterativeType(t);}

    void applyLaplacian();


    //LAB 3: Global Smoothing
    void applyGlobalSmoothing();
    void setPercentage(int per){percentage = per;}

    //LAB4: Magnify Details
    void applyMagnify();
    void setNumIterationMag(int iter){number_of_iteration = iter;}
    void setMultiplierMag(int mult){mult_of_iteration = mult;}
    void setParameterMag(float par){magnify_par = par;}


    //LAB 5: Compute parametrization
    void computeParametrization();



    //COMUNICATION AND UPDATES:
    void setLambda(float value);
    void setDefault();
    void setLaplacianMode(WeightType selectedType);
    int getVerticesSize();


    //FUNCTION GENERAL USAGE:
    void computeRing(vector<uint> &ring, uint ver);
    float computeCotangent(uint ver, vector<uint> ring, uint r, bool alternative = false);
    float cotangentComputation(glm::vec3 v_i,glm::vec3 v_ang, glm::vec3 v_j);
    float computeAngle(glm::vec3 v_ang,glm::vec3 v_i, glm::vec3 v_j);



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
    IterativeType current_iterative_type = IterativeType::NORMAL;

    void computeLaplacianOperator(float lambda_in, WeightType selected_weight_type);
    glm::vec3 computeLaplacian(uint v, std::vector<uint> ring, WeightType selected_weight_type);

    vector<QVector3D>  getLaplacianVertices(IterativeType type, WeightType weight_type, int iteration);




    //LAB 3: Global Smoothing
    //sparse Matrix
//    typedef Eigen::SparseMatrix<double> SparseMatrix; // declares a column-major sparse matrix type of double
    typedef Eigen::Triplet<double> Triplet; // a small structure to hold a non zero as a triplet (i,j,value)

    std::vector<uint> fixedVertices;
    int percentage = 30;


    void buildMatrixA();
    vector<int> fixVertices();
    std::vector<double> computeWeight(uint v , std::vector<uint> ring, double &total_weigth);




    //LAB 4: Magnify high frequency details
    float magnify_par = 0.5f;
    int number_of_iteration = 10;
    int mult_of_iteration = 3;







    //LAB 5: DISCRETE HARMONIC MAPS:
    vector<uint> lonely_corners;
    vector<pair<uint,uint>> border_chain;
    vector<uint> border;
    vector<glm::vec2> parametrizeVertices;

    void parametrizeChain();
    void parametrizeOtherVertices();
    void orderBorderChain();






};


#endif // TRIANGLEMESH_H
