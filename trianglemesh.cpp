#include "trianglemesh.h"



#define PI 3.14159265

using namespace std;


struct CornerEdge
{
	int vertexA, vertexB, corner;

	bool operator<(const CornerEdge &cEdge) { return (vertexA < cEdge.vertexA) || ((vertexA == cEdge.vertexA) && (vertexB < cEdge.vertexB)); }
	bool operator==(const CornerEdge &cEdge) { return (vertexA == cEdge.vertexA) && (vertexB == cEdge.vertexB); }

    void set(int a, int b, int c)
    {
        vertexA = a;
        vertexB = b;
        corner = c;
    }

    int a()
    {
        return vertexA;
    }
    int b()
    {
        return vertexB;
    }
    int c()
    {
        return corner;
    }
};


vector<CornerEdge> corners;
vector<CornerEdge> oppositesVec;
vector<int> opposites;
vector<int> cornersVertices;




int next(int corner)
{
	return 3 * (corner / 3) + (corner + 1) % 3;
}

int previous(int corner)
{
	return 3 * (corner / 3) + (corner + 2) % 3;
}


TriangleMesh::TriangleMesh() : vboVertices(QOpenGLBuffer::VertexBuffer),
										 vboNormals(QOpenGLBuffer::VertexBuffer),
                                         eboTriangles(QOpenGLBuffer::IndexBuffer),
                                         vboColorGauss(QOpenGLBuffer::VertexBuffer),
                                         vboColorMean(QOpenGLBuffer::VertexBuffer)
{
}


void TriangleMesh::addVertex(const QVector3D &position)
{
	vertices.push_back(position);
}

void TriangleMesh::addTriangle(int v0, int v1, int v2)
{
	triangles.push_back(v0);
	triangles.push_back(v1);
	triangles.push_back(v2);
}

void TriangleMesh::buildCube()
{
	GLfloat vertices[] = {-1, -1, -1,
								  1, -1, -1,
								  1,  1, -1,
								 -1,  1, -1,
								 -1, -1,  1,
								  1, -1,  1,
								  1,  1,  1,
								 -1,  1,  1
								};

	GLuint faces[] = {3, 1, 0, 3, 2, 1,
							5, 6, 7, 4, 5, 7,
							7, 3, 0, 0, 4, 7,
							1, 2, 6, 6, 5, 1,
							0, 1, 4, 5, 4, 1,
							2, 3, 7, 7, 6, 2
						  };

	int i;

	for(i=0; i<8; i+=1)
		addVertex(0.5f * QVector3D(vertices[3*i], vertices[3*i+1], vertices[3*i+2]));
	for(i=0; i<12; i++)
		addTriangle(faces[3*i], faces[3*i+1], faces[3*i+2]);
}

bool TriangleMesh::init(QOpenGLShaderProgram *program)
{
    active_program = program;

    computeCornerTables();
    computeVertexCorners();

//    principalCurvatures();



    original_vertices = vertices;


    return buildMesh();

}

bool TriangleMesh::buildMesh()
{
    vector<QVector3D> replicatedVertices, normals;
    vector<unsigned int> perFaceTriangles;

    vector<QVector2D> replicatedColors;

    //BUILDING THE MESH
    buildReplicatedVertices(replicatedVertices, normals, perFaceTriangles, replicatedColors);

    //defining the program
    active_program->bind();

    vao.destroy();
    vao.create();
    if(vao.isCreated())
        vao.bind();
    else
        return false;

    //vertices
    vboVertices.destroy();
    vboVertices.create();
    if(vboVertices.isCreated())
        vboVertices.bind();
    else
        return false;
    vboVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    active_program->enableAttributeArray(0);
    active_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    //normals
    vboNormals.destroy();
    vboNormals.create();
    if(vboNormals.isCreated())
        vboNormals.bind();
    else
        return false;
    vboNormals.setUsagePattern(QOpenGLBuffer::StaticDraw);
    active_program->enableAttributeArray(1);
    active_program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);

    //triangles
    eboTriangles.destroy();
    eboTriangles.create();
    if(eboTriangles.isCreated())
        eboTriangles.bind();
    else
        return false;
    eboTriangles.setUsagePattern(QOpenGLBuffer::StaticDraw);


    //curvatures colors vbo
    vboCurvatureColors.destroy();
    vboCurvatureColors.create();
    if(vboCurvatureColors.isCreated())
        vboCurvatureColors.bind();
    else
        return false;
    vboCurvatureColors.setUsagePattern(QOpenGLBuffer::StaticDraw);
    active_program->enableAttributeArray(2);
    active_program->setAttributeBuffer(2, GL_FLOAT, 0, 2, 0);

    fillVBOs(replicatedVertices, normals, perFaceTriangles, replicatedColors);

    vao.release();
    active_program->release();


    return true;
}

void TriangleMesh::destroy()
{
	vao.destroy();
	vboVertices.destroy();
	vboNormals.destroy();
	eboTriangles.destroy();


	vertices.clear();
	triangles.clear();

    //corners table
    corners.clear();
    cornersVertices.clear();
    oppositesVec.clear();
    opposites.clear();

    //curvatures
    vboCurvatureColors.destroy();
    gaussianCurvature.clear();
    meanCurvature.clear();

    //laplacian
    laplace_vertices.clear();
    laplace_operators.clear();

    //armonics
    border_chain.clear();
    lonely_corners.clear();
}


void TriangleMesh::render(QOpenGLFunctions &gl)
{
	vao.bind();
	eboTriangles.bind();
	gl.glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
	vao.release();
}

void TriangleMesh::buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors)
{
	normals.resize(triangles.size());

	for(unsigned int i=0; i<triangles.size(); i+=3)
	{
		replicatedVertices.push_back(vertices[triangles[i]]);
		replicatedVertices.push_back(vertices[triangles[i+1]]);
		replicatedVertices.push_back(vertices[triangles[i+2]]);

		QVector3D N = QVector3D::crossProduct(vertices[triangles[i+1]] - vertices[triangles[i]], vertices[triangles[i+2]] - vertices[triangles[i]]);
		N.normalize();
		normals[i] = N;
		normals[i+1] = N;
        normals[i+2] = N;

		perFaceTriangles.push_back(perFaceTriangles.size());
		perFaceTriangles.push_back(perFaceTriangles.size());
		perFaceTriangles.push_back(perFaceTriangles.size());

        if(curvatureON)
        {
            replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i]], meanCurvature[triangles[i]]) );
            replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i+1]], meanCurvature[triangles[i+1]]));
            replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i+2]], meanCurvature[triangles[i+2]]));
        }
        else
        {
            replicatedColors.push_back( QVector2D(0.0f, 0.0f));
            replicatedColors.push_back( QVector2D(0.0f, 0.0f));
            replicatedColors.push_back( QVector2D(0.0f, 0.0f));
        }


    }
}

void TriangleMesh::fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles,  vector<QVector2D> &replicatedColors)
{
//    std::cout << "VBO: filling gaussian vbo, of size: "<< replicatedVertices.size()/3 << std::endl;
	vboVertices.bind();
	vboVertices.allocate(&replicatedVertices[0], 3 * sizeof(float) * replicatedVertices.size());
	vboVertices.release();

	vboNormals.bind();
	vboNormals.allocate(&normals[0], 3 * sizeof(float) * normals.size());
	vboNormals.release();

	eboTriangles.bind();
	eboTriangles.allocate(&perFaceTriangles[0], sizeof(int) * perFaceTriangles.size());
	eboTriangles.release();

    vboCurvatureColors.bind();
    vboCurvatureColors.allocate(&replicatedColors[0], 2 * sizeof(float) * replicatedVertices.size());
    vboCurvatureColors.release();

}



void TriangleMesh::updateVBOCurvatures()
{
    vector<QVector2D> replicatedColors;
    for(unsigned int i=0; i<triangles.size(); i+=3)
    {
        if(curvatureON)
        {
            replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i]], meanCurvature[triangles[i]]) );
            replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i+1]], meanCurvature[triangles[i+1]]));
            replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i+2]], meanCurvature[triangles[i+2]]));
        }
        else
        {
            replicatedColors.push_back( QVector2D(0.0f, 0.0f));
            replicatedColors.push_back( QVector2D(0.0f, 0.0f));
            replicatedColors.push_back( QVector2D(0.0f, 0.0f));
        }

        vboCurvatureColors.bind();
        vboCurvatureColors.allocate(&replicatedColors[0], 2 * sizeof(float) * replicatedColors.size());
        vboCurvatureColors.release();


    }
}


















//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~CORNER~~~AND~OPPOSITE~~~~~~~~~~~~~~TABLES~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~AND~BORDER~~~~~~~~~~~
void TriangleMesh::computeCornerTables()
{


    corners.clear();
    opposites.clear();
    oppositesVec.clear();
    lonely_corners.clear();

    //compare all the triangles
    std::vector<int> edges;

    for (uint t1 = 0; t1 < triangles.size()-2; t1 = t1+3)
    {
//        std::cout << "trianglo :" << t1 << std::endl;

        int a = triangles[t1];
        int b = triangles[t1 + 1];
        int c = triangles[t1 + 2];

//        std::cout << "a:" << a << "  b:" << b <<"  c:" << c << std::endl;

        CornerEdge c1,c2,c3;

        if(a < b)
            c1.set(a,b,c);
        else
            c1.set(b,a,c);

        if(b < c)
            c2.set(b,c,a);
        else
            c2.set(c,b,a);

        if(c < a)
            c3.set(c,a,b);
        else
            c3.set(a,c,b);

//        std::cout << "1st entry index:"<<t1 <<"  values:" << c1.a() << "  :" << c1.b() << "  :" << c1.c() << std::endl;
//        std::cout << "2nd entry index:"<<t1+1 <<"  values:" << c2.a() << "  :" << c2.b() << "  :" << c2.c() << std::endl;
//        std::cout << "3rd entry index:"<<t1+2 <<"  values:" << c3.a() << "  :" << c3.b() << "  :" << c3.c() << std::endl;
        corners.push_back(c1);
        corners.push_back(c2);
        corners.push_back(c3);
//        std::cout << "created corners: c1 :" << c1.c() << "  c2 :" << c2.c() << "  c3 : " << c3.c() << std::endl;
    }
//    std::cout << "corners created , size: " << corners.size() << " compare to triangles : " << triangles.size() << std::endl;


    //opposite table
    oppositesVec.resize(corners.size());
    opposites.resize(corners.size());

    //Border: check if a vertex has opposite
    std::vector<bool> lonely;
    for(uint i =0; i < corners.size(); i++)
        {lonely.push_back(true);}

    for (uint i = 0; i < corners.size(); i++)
    {
        int a1 = corners[i].a();
        int b1 = corners[i].b();
        int c1 = corners[i].c();


        for (uint j = i +1 ; j < corners.size(); j++)
        {
            int a2 = corners[j].a();
            int b2 = corners[j].b();
            int c2 = corners[j].c();


            if( a1 == a2 and b1 == b2)
            {

                //check for lonlienss :(
                lonely[i] = false;
                lonely[j] = false;


                CornerEdge o1,o2;
                o1.set(a1,b1,c2);
                o2.set(a2,b2,c1);

                oppositesVec[i] = o1;
                oppositesVec[j] = o2;

                opposites[i] = j;
                opposites[j] = i;

//                std::cout << "set the values of corners :" << std::endl << "ind:" << i << "  value:" << opposites[i] << std::endl << "ind: " << j << "  value:" << opposites[j] << std::endl;
            }
        }

    }

    for(uint i =0; i < lonely.size(); i++)
    {
        if(lonely[i] == true)
        {
            lonely_corners.push_back(i);

            uint a = uint(corners[i].a());
            uint b = uint(corners[i].b());
            uint a_test = uint(next(int(i)));
            uint b_test = uint(previous(int(i)));

            border_chain.push_back(a);
            border_chain.push_back(b);
            std::cout <<" lonley corner: " << i <<std::endl;
            std::cout<< " edge     " << a << " -  " << b << std::endl ;
            std::cout<< " edge test" << corners[a_test].c() << " -  " << corners[b_test].c() << std::endl ;

        }
    }
    std::cout << " lonely list have size :" << lonely_corners.size() << std::endl;
    std::cout << " border have size :" << border_chain.size() << std::endl;


}

//COMPUTE FOR EACH VERTEX ONE OF THE CORRISPONDENT CORNERS
//N.B: ALSO COMPUTE THE BOUNDIG BOX
void TriangleMesh::computeVertexCorners()
{
    //bounding box variables
    float min_x,min_y,min_z, max_x,max_y,max_z;
//    std::cout << " " <<std::endl;
//    std::cout << "*****VERTEX CORNER ***** " <<std::endl;
//    std::cout << " " <<std::endl;
    cornersVertices.resize(vertices.size());
    for( uint v = 0; v < vertices.size() ; v++)
    {
        //COMPUTATION OF BOUNDING BOX
        if(v == 0)
        {
            min_x = vertices[v].x();
            min_y = vertices[v].y();
            min_z = vertices[v].z();
            max_x = vertices[v].x();
            max_y = vertices[v].y();
            max_z = vertices[v].z();
        }
        else
        {
            if(vertices[v].x() < min_x)
                min_x = vertices[v].x();
            else if(vertices[v].x() > max_x)
                max_x = vertices[v].x();

            if(vertices[v].y() < min_y)
                min_y = vertices[v].y();
            else if(vertices[v].y() > max_y)
                max_y = vertices[v].y();

            if(vertices[v].z() < min_z)
                min_z = vertices[v].z();
            else if(vertices[v].z() > max_z)
                max_z = vertices[v].z();
        }
        //CREATION OF TABLE
        for( uint c = 0; c < corners.size() ; c++)
        {
            if(corners[c].c() == v)
            {
//                cornersVertices.push_back(c);
                cornersVertices[v] = c;
            }
        }
//        std::cout << " for vertex v: "<<v<< " we defined corner c: " << cornersVertices[v] << std::endl;
    }
    boundingBoxMin = QVector3D(min_x, min_y, min_z);
    boundingBoxMax = QVector3D(max_x, max_y, max_z);

    std::cout << " done with vertex corners" << std::endl;
}









void TriangleMesh::parametrizeChain()
{
    parametrizeVertices.clear();

    float circonf = float(2*M_PI);

    //compute chein length
    float totalDistance = 0;
    std::vector<float> individualDistance;
    for(uint b = 0; b < border_chain.size(); b++)
    {
        glm::vec3 v1,v2;
        if(b == border_chain.size())
        {
            v1 = glm::vec3(vertices[b].x(),vertices[b].y(),vertices[b].z());
            v2 = glm::vec3(vertices[0].x(),vertices[0].y(),vertices[0].z());
            std::cout << " vertices :" << b << "  " << 0 ;
        }
        else
        {
            v1 = glm::vec3(vertices[b].x(),vertices[b].y(),vertices[b].z());
            v2 = glm::vec3(vertices[b+1].x(),vertices[b+1].y(),vertices[b+1].z());
            std::cout << " vertices :" << b << "  " << b+1 ;

        }

        float distance = glm::distance(v1,v2);
        totalDistance = totalDistance + distance;
        individualDistance.push_back(distance);
        std::cout << "  have distance :" << distance << " and total distance is : " << totalDistance << std::endl;
    }

    //unit step on the circonference
    float unit_step = totalDistance/circonf;
    glm::vec2 center = glm::vec2(0.5,0.5);
    float radius = 0.5;
    //parametrize the border
    for(uint b = 0; b < border_chain.size(); b++)
    {
        glm::vec2 pos;
        float angle = individualDistance[b]*unit_step;
        //first parametrize value on the right, on (1, 0.5) of my parametrization sapce
        if(b == 0)
            pos = center + glm::vec2(radius,0.0);
        else pos =  center + glm::vec2( sin(angle)*radius , cos(angle)*radius );

        parametrizeVertices.push_back(pos);

    }




}














//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~USEFULL FUNCTIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//COMPUTE THE RING OF THE SELECTED VERTEX ver
void TriangleMesh::computeRing(vector<uint> &ring, uint ver)
{
    uint corner = uint(cornersVertices[ver]);

    //first triangle
    uint nex, prev ;
    nex = next(corner);
    prev = previous(corner);

    uint vertNext = uint(corners[nex].c());
//    int vertPrev = corners[uint(prev)].c();
//    ring.push_back(vertPrev);
    ring.push_back(vertNext);

    uint newNext = opposites[prev];
    uint newVertNext = corners[newNext].c();

    while( newVertNext != vertNext)
    {
        uint newPrev = next(newNext);
//        int newVertPrev = corners[uint(newPrev)].c();
//        ring.push_back(newVertPrev);
        ring.push_back(newVertNext);

        newNext = opposites[newPrev];
        newVertNext = corners[newNext].c();
     }
}

//COMPUTE COTANGENT OF THE VERTEX ver AND THE RING VERTEX r
//with the two opposite angle of the edge ver-r
//return cotg(alpha) + cotg(beta)
float TriangleMesh::computeCotangent(uint ver, vector<uint> ring, uint r, bool alternative)
{
    float alpha,beta = 0.0f;

    uint v_beta = ring[r];
    uint v_j = ring[r+1];
    uint v_alpha = ring[r+2];

    if( r == (ring.size()-2) )
    {
        v_beta = ring[r];
        v_j = ring[r+1];
        v_alpha = ring[0];
    }
    else if( r == (ring.size()-1) )
    {
        v_beta = ring[r];
        v_j = ring[0];
        v_alpha = ring[1];
    }

    glm::vec3 vert_i = glm::vec3(vertices[ver].x(), vertices[ver].y(), vertices[ver].z() );
    glm::vec3 vert_beta = glm::vec3(vertices[v_beta].x(), vertices[v_beta].y(), vertices[v_beta].z() );
    glm::vec3 vert_j = glm::vec3(vertices[v_j].x(), vertices[v_j].y(), vertices[v_j].z() );
    glm::vec3 vert_alpha = glm::vec3(vertices[v_alpha].x(), vertices[v_alpha].y(), vertices[v_alpha].z() );

    beta = computeAngle(vert_beta,vert_i,vert_j);

    alpha = computeAngle(vert_alpha,vert_i,vert_j);


    //4 different computation of cotang :,,,,((((
    float cotg_sum_b = 1.0f/tan(alpha) + 1.0f/tan(beta);
    float cotg_sum = 0;
    float cos_a = cos(alpha);
    if(cos_a < 1.0f) cos_a = 1.0f;
    if(cos_a < -1.0f) cos_a = -1.0f;
    float sin_a = sin(alpha);
    if(sin_a < 1.0f) cos_a = 1.0f;
    if(sin_a < -1.0f) cos_a = -1.0f;
    float cos_b= cos(alpha);
    if(cos_b < 1.0f) cos_b = 1.0f;
    if(cos_b < -1.0f) cos_b = -1.0f;
    float sin_b = sin(beta);
    if(sin_b < 1.0f) cos_b = 1.0f;
    if(sin_b < -1.0f) cos_b = -1.0f;
    if(sin_a != 0 && sin_b !=0)
        cotg_sum = cos_a/sin_a + cos_b/sin_b;
    else cotg_sum = 0.0f;
    double cotg_sum_c = (tan(M_PI_2 - double(alpha))) - tan(M_PI_2 - double(beta));
    float cotg_alpha = cotangentComputation(vert_alpha, vert_i, vert_j);
    float cotg_beta = cotangentComputation(vert_beta, vert_i, vert_j);
    float cotg_sum_d = cotg_alpha + cotg_beta;

    if(alternative)
        return cotg_sum_b;
    else return cotg_sum;
}

float TriangleMesh::cotangentComputation(glm::vec3 v_i,glm::vec3 v_ang, glm::vec3 v_j)
{
    glm::vec3 vec_ang_1 = (v_i - v_ang);
    glm::vec3 vec_ang_2 = (v_j - v_ang);
    glm::vec3 vec_cross = glm::cross(vec_ang_2,vec_ang_1);
    float area = glm::length(vec_cross);
    float sin = area/(glm::length(vec_ang_1)*glm::length(vec_ang_2));
    if(sin > 1.0f)
        sin = 1.0f;
    if(sin < 0.0001f)
        sin = 0.0001f;

    float cos = sqrt(1 - sin*sin);
    return cos/sin;
}

//float TriangleMesh::areaTriangle(int vi, int vj, int vk){
//    return 0.5f*QVector3D::crossProduct((vertices[vj]-vertices[vi]),(vertices[vk]-vertices[vi])).length();
//}

float TriangleMesh::computeAngle(glm::vec3 v_ang,glm::vec3 v_i, glm::vec3 v_j)
{
    glm::vec3 v_ang_i = (v_i - v_ang);
    glm::vec3 v_ang_j = (v_j - v_ang);

    glm::normalize(v_ang_i);
    glm::normalize(v_ang_j);

    return acos(glm::dot(v_ang_i, v_ang_j));
}











//~~~LAB~~~1~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~CURVATURES~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::principalCurvatures()
{




    //****************************
    //****GAUSSIAN CURVATURE******
    //****************************
    gaussianCurvature.clear();
    meanCurvature.clear();
    max_gauss = 0.0;
    min_gauss = 0.0;
    max_mean = 0.0;
    min_mean = 0.0;

    for( uint v = 0; v < vertices.size(); v++)
    {

        std::vector<uint> ring;
        computeRing(ring, v);

        std::vector<float> areasRing;
        std::vector<float> anglesRing;

        //computation of the areas and angles of each triangle of the ring
        for( uint r = 0; r < ring.size(); r++ )
        {

            uint v2 = uint(ring[r]);
            uint v3 = uint(ring[r+1]);
            if( r == (ring.size()-1) )
            {
                v2 = uint(ring[r]);
                v3 = uint(ring[0]);
            }

            glm::vec3 vert1 = glm::vec3(vertices[v].x(), vertices[v].y(), vertices[v].z() );
            glm::vec3 vert2 = glm::vec3(vertices[v2].x(), vertices[v2].y(), vertices[v2].z() );
            glm::vec3 vert3 = glm::vec3(vertices[v3].x(), vertices[v3].y(), vertices[v3].z() );


            float area = 0.5f* glm::length(glm::cross((vert2 - vert1),(vert3 - vert1)));

            areasRing.push_back(area);


            glm::vec3 vect1 = glm::normalize(vert2 - vert1);
            glm::vec3 vect2 = glm::normalize(vert3 - vert1);

            float cosine = glm::dot(vect1,vect2);
            float angle = acos(cosine);
            anglesRing.push_back(angle);

        }

        float tot_angles = 0;
        for( uint ang = 0; ang < anglesRing.size(); ang ++)
            tot_angles = tot_angles + anglesRing[ang];

        float tot_areas = 0;
        for( uint ar = 0; ar < areasRing.size(); ar ++)
            tot_areas = tot_areas + areasRing[ar];

        float A = tot_areas/3;
        float Kg = (2*PI - tot_angles)/A;
        gaussianCurvature.push_back(Kg);

        if(v==0)
        {
            min_gauss = Kg;
            max_gauss = Kg;
        }
        else
        {
            if(Kg < min_gauss)
                min_gauss = Kg;
            if(Kg > max_gauss)
                max_gauss = Kg;
        }


        //****************************
        //******MEAN CURVATURE********
        //****************************
        glm::vec3 tot_sum = glm::vec3(0);

        for( uint r = 0; r < ring.size(); r++ )
        {

            uint v_j = ring[r+1];

            if( r == (ring.size()-2) )
                v_j = ring[r+1];
            else if( r == (ring.size()-1) )
                v_j = ring[0];

            glm::vec3 vert_i = glm::vec3(vertices[v].x(), vertices[v].y(), vertices[v].z() );
            glm::vec3 vert_j = glm::vec3(vertices[v_j].x(), vertices[v_j].y(), vertices[v_j].z() );


            glm::vec3 vec_ij = vert_j -vert_i;

            float cotg_sum = computeCotangent(v,ring,r);

            tot_sum = tot_sum + (cotg_sum *vec_ij);
        }

        glm::vec3 Sv;
        Sv = 1/(2*A) * tot_sum;

        float Kh = (glm::length(Sv))/2.0f;
        meanCurvature.push_back(Kh);

        if(v==0)
        {
            min_mean = Kh;
            max_mean = Kh;
        }
        else
        {
            if(Kh < min_mean)
                min_mean = Kh;
            if(Kh > max_mean)
                max_mean = Kh;
        }
    }
}













//~~~LAB~~~2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~ITERATIVE LAPLACIAN: SINGLE STEP COMPUTATION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::computeLaplacianOperator(float lambda_in)
{

    std::cout << "compute laplacian" << std::endl;

    laplace_operators.clear();
    laplace_vertices.clear();

    for( uint v = 0; v < vertices.size(); v++)
    {
        std::vector<uint> ring;
        computeRing(ring, v);

        //1.uniform weights
        //access the ring
        glm::vec3 laplacian_v = glm::vec3(0.0,0.0,0.0);

        glm::vec3 vec_i = glm::vec3(vertices[v].x(),vertices[v].y(),vertices[v].z());



        //COMPUTATION OF THE LAPLACIAN UNIFORM OR COTANGENT
        float total_weight = 0;
        //UNIFORM:
        if (current_weight_type == WeightType::UNIFORM)
        {
            for(uint r = 0; r < ring.size(); r++)
            {
                float weight = 1.0f/ring.size();

                uint v_j = ring[r];
                glm::vec3 vec_j = glm::vec3(vertices[v_j].x(),vertices[v_j].y(),vertices[v_j].z());
                laplacian_v = laplacian_v + weight*vec_j;
            }
            laplacian_v = laplacian_v - vec_i;

        }
        else if (current_weight_type == WeightType::COTANGENT)  //COTANGENT
        {
            for(uint r = 0; r < ring.size(); r++)
            {
                float weight = computeCotangent(uint(v),ring,r,false);
                total_weight = total_weight + weight;

                uint v_j = ring[r];
                glm::vec3 vec_j = glm::vec3(vertices[v_j].x(),vertices[v_j].y(),vertices[v_j].z());
                laplacian_v = laplacian_v + weight*(vec_j);
//                laplacian_v = laplacian_v + weight*(vec_i-vec_j);
            }
            laplacian_v = (laplacian_v/total_weight) - vec_i ;
//            laplacian_v = (laplacian_v/total_weight) ;

        }


        laplace_operators.push_back(QVector3D(laplacian_v.x,laplacian_v.y,laplacian_v.z));
        glm::vec3 new_vert_i = vec_i + lambda*laplacian_v;
        laplace_vertices.push_back(QVector3D(new_vert_i.x,new_vert_i.y,new_vert_i.z));
    }
    std::cout << "Computed Laplacian of " << vertices.size() << "vertices" << std::endl;
}
void TriangleMesh::applyLaplacian()
{
    computeLaplacianOperator(lambda);

    if(current_iterative_type == IterativeType::NORMAL)
        vertices = laplace_vertices;
    else if(current_iterative_type == IterativeType::BILAPLACIAN)
    {
        vertices = laplace_vertices;
        computeLaplacianOperator(-lambda);
        vertices = laplace_vertices;
    }
    else if(current_iterative_type == IterativeType::TAUBIN)
    {
        vertices = laplace_vertices;
        float Kpb = 0.1f;
        float mu = 1.0f/(Kpb + (1.0f/lambda));
        computeLaplacianOperator(mu);
        vertices = laplace_vertices;

    }
}











//~~~LAB~~~3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~GLOBAL~~~~~~~~~~SMOOTHING
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::applyGlobalSmoothing()
{
    matrixClass = geomfunctions();
    matrixClass.buildMatrixA();

}















//~~~LAB~~~5~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~DISCRETE~HARMONIC~MAPPING
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~































//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~SETTER~~~~~~~~~~~~~~~~~~COMUNICATION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::setLaplacianMode(WeightType selectedType)
{
    current_weight_type = selectedType;
}
void TriangleMesh::setDefault()
{
    vertices = original_vertices;

}
void TriangleMesh::setLambda(float value)
{
    lambda = value;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~GETTER~~~~~~~~~~~~~~~~~~COMUNICATION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int TriangleMesh::getVerticesSize()
{
    return vertices.size();
}
