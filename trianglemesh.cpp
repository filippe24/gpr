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
                                         vboColorMean(QOpenGLBuffer::VertexBuffer),
                                         vboTextureCoordinates(QOpenGLBuffer::VertexBuffer)
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

//    computeParametrization();

//    principalCurvatures();

    original_vertices = vertices;


    return buildMesh();

}

bool TriangleMesh::buildMesh()
{
    vector<QVector3D> replicatedVertices;
    vector<QVector3D> normals;
    vector<unsigned int> perFaceTriangles;

    vector<QVector2D> replicatedColors;
    vector<QVector2D> replicatedTextureCoord;

    std::cout << " before building replicas " << std::endl;
    //BUILDING THE MESH
    buildReplicatedVertices(replicatedVertices, normals, perFaceTriangles, replicatedColors, replicatedTextureCoord);

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



    //textures  vbo
    vboTextureCoordinates.destroy();
    vboTextureCoordinates.create();
    if(vboTextureCoordinates.isCreated())
        vboTextureCoordinates.bind();
    else
        return false;
    vboTextureCoordinates.setUsagePattern(QOpenGLBuffer::StaticDraw);
    active_program->enableAttributeArray(3);
    active_program->setAttributeBuffer(3, GL_FLOAT, 0, 2, 0);

    fillVBOs(replicatedVertices, normals, perFaceTriangles, replicatedColors, replicatedTextureCoord);

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
    vboTextureCoordinates.destroy();
    vboCurvatureColors.destroy();


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

void TriangleMesh::buildReplicatedVertices(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles, vector<QVector2D> &replicatedColors, vector<QVector2D> &replicatedTextureCoordinates)
{
	normals.resize(triangles.size());
    std::cout << " start with replicas" << std::endl;

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

        if(parametrizationON)
        {
            replicatedTextureCoordinates.push_back(parametrizeVertices[triangles[i]]);
            replicatedTextureCoordinates.push_back(parametrizeVertices[triangles[i+1]]);
            replicatedTextureCoordinates.push_back(parametrizeVertices[triangles[i+2]]);
        }
        else
        {
            replicatedTextureCoordinates.push_back( QVector2D(0.0f, 0.0f));
            replicatedTextureCoordinates.push_back( QVector2D(0.0f, 0.0f));
            replicatedTextureCoordinates.push_back( QVector2D(0.0f, 0.0f));
        }

    }

    std::cout << " done with replicas" << std::endl;
}

void TriangleMesh::fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles,  vector<QVector2D> &replicatedColors, vector<QVector2D> &replicatedTextureCoordinates)
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

    vboCurvatureColors.bind();
    vboCurvatureColors.allocate(&replicatedTextureCoordinates[0], 2 * sizeof(float) * replicatedTextureCoordinates.size());
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

    vector<QVector2D> replicatedTextureCoordinates;
    for(unsigned int i=0; i<triangles.size(); i+=3)
    {
        if(parametrizationON)
        {
            replicatedTextureCoordinates.push_back(parametrizeVertices[triangles[i]]);
            replicatedTextureCoordinates.push_back(parametrizeVertices[triangles[i+1]]);
            replicatedTextureCoordinates.push_back(parametrizeVertices[triangles[i+2]]);
        }
        else
        {
            replicatedTextureCoordinates.push_back( QVector2D(0.0f, 0.0f));
            replicatedTextureCoordinates.push_back( QVector2D(0.0f, 0.0f));
            replicatedTextureCoordinates.push_back( QVector2D(0.0f, 0.0f));
        }

        vboCurvatureColors.bind();
        vboCurvatureColors.allocate(&replicatedTextureCoordinates[0], 2 * sizeof(float) * replicatedTextureCoordinates.size());
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

            std::pair<uint,uint> couple;
            couple.first = a;
            couple.second = b;
            border_chain.push_back(couple);

            //put -1 as their opposite table value
            opposites[i] = -1;

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









//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~USEFULL FUNCTIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::newComputeRing(vector<uint> &ring, uint ver)
{
    bool found_first_border = false;
    bool found_second_border = false;
    bool loopCompleted = false;

    int corner = cornersVertices[ver];

    int startingN = next(corner);
    int startingP = previous(corner);

    //indices value of next and previous
    int startingN_vert = corners[uint(startingN)].c();
    int startingP_vert = corners[uint(startingP)].c();

    ring.push_back(uint(startingN_vert));
    ring.push_back(uint(startingP_vert));

    int N = startingN;
    int P = startingP;



    //new starting value for a border
    int restartingN = -1;
    int restartingP = -1;




    //loop opposite of P
    while( !loopCompleted  && !found_first_border )
    {

        int oppP = opposites[uint(P)];
        if(oppP == -1)
        {
            found_first_border = true;
            restartingN = N;
            restartingP = P;
        }
        else
        {
            N = oppP;
            P = next(N);

            //indices value of next and previous
            int N_vert = corners[uint(N)].c();
            int P_vert = corners[uint(P)].c();


            if(N_vert == startingP_vert)
            {
                loopCompleted = true;
            }
            else
                ring.push_back(uint(N_vert));
        }

    }
    if(loopCompleted)
//    {
//        std::cout << " created a  ring with " << ring.size() << "vertices " << std::endl;
//        std::cout << " ring: ";
//        for(int r = 0; r < ring.size(); r++ )
//            std::cout << " " << ring[r] << ", ";
//        std::cout << std::endl;
//    }

    if(found_first_border)
    {

        vector<uint> temp_ring;

        if(restartingN == -1 || restartingP == -1)
            std::cout << " restarting values not initialized " << std::endl;


        //indices value of next and previous
        int restartingN_vert = corners[uint(restartingN)].c();
        int restartingP_vert = corners[uint(restartingP)].c();

        temp_ring.push_back(uint(restartingN_vert));
        temp_ring.push_back(uint(restartingP_vert));

        int test = 0;

        int reN = restartingN;
        int reP = restartingP;

        while(!found_second_border && test < 20)
        {

            int oppN = opposites[uint(reN)];
            if(oppN == -1)
            {
                found_second_border = true;
            }
            else
            {
                reP = oppN;
                reN = previous(reP);
                //indices value of previous
                int reP_vert = corners[uint(reP)].c();
                temp_ring.push_back(uint(reP_vert));
            }

        }


        if(found_second_border)
        {
            ring = temp_ring;
        }
        else {
            std::cout << " <<<<<<< SOMETHING WENT WRONG " << std::endl;
        }


    }





}

void TriangleMesh::computeRing(vector<uint> &ring, uint ver)
{
    bool found_border = false;

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








//COMPUTE THE RING OF THE SELECTED VERTEX ver
void TriangleMesh::oldComputeRing(vector<uint> &ring, uint ver)
{

    std::cout << " ring vertex i :" << ver << std::endl;

    bool found_first_border = false;
    bool found_second_border = false;

    int corner = cornersVertices[ver];


    //first triangle
    int nex, prev ;
    nex = next(corner);
    prev = previous(corner);


    uint vertNext = uint(corners[uint(nex)].c());
    uint vertPrev = uint(corners[uint(prev)].c());

//    ring.push_back(vertPrev);
    ring.push_back(vertNext);

    int newNext = opposites[uint(prev)];
    if(newNext == -1)
    {

        //CASE 1 : THE FIRST OPPOSITE WE EXPLORE IS ALREADY ON THE BORDER

        std::cout << " case 1 found first border " << std::endl;
        found_first_border = true;

        //we push back the previous
        ring.push_back(vertPrev);

        int newPrev = opposites[uint(nex)];
        if(newPrev == -1)
            std::cout << " case 1 problem" << std::endl;

        uint newPrevVert = uint(corners[uint(newPrev)].c());
        int test_iter = 0;
        while( !found_second_border && test_iter < 10  )
        {
            int newNext = next(next(newPrev));

            ring.push_back(newPrevVert);

            newPrev = opposites[uint(newNext)];
            if(newPrev == -1)
            {
                std::cout << " found second border " << std::endl;
                found_second_border = true;
            }
            else newPrevVert = uint(corners[uint(newPrev)].c());

            std::cout << " case 1 iteration:" << test_iter << std::endl;
            test_iter++;
         }


    }
    else
    {

        //CASE 2 : STARTING THE NORMAL LOOP
        std::cout << " noraml loop " << std::endl;


        uint newVertNext = uint(corners[uint(newNext)].c());

        //case3 starting variables
        int restartNext = -1, restartPrev = -1;

        while( (newVertNext != vertNext) && (!found_first_border))
        {
            int newPrev = next(newNext);

            ring.push_back(newVertNext);

            restartNext = newNext;

            newNext = opposites[uint(newPrev)];
            if(newNext == -1)
            {
                std::cout << " found first border " << std::endl;
                found_first_border = true;
                restartPrev = newPrev;

            }
            else newVertNext = uint(corners[uint(newNext)].c());
        }


        //CASE 3 : WE FOUND A BORDER IN THE LOOP AND WE HAVE TO START AGAIN FROM THAT POINT ON THE OTHER DIRECTION
        if(found_first_border)
        {
            std::cout << "case 3: found first border " << std::endl;
            //check:
            if(restartNext == -1 || restartPrev == -1)
                std::cout<< " case 3 problem: restart variables not initialized "<< std::endl;


            uint vertRestartNext = uint(corners[uint(restartNext)].c());
            uint vertRestartPrev = uint(corners[uint(restartPrev)].c());

            //we clear the ring until now
            vector<uint> temp_ring;
            //we push back the new vertices of the ring
            temp_ring.push_back(vertRestartNext);
            temp_ring.push_back(vertRestartPrev);


            int newPrev = opposites[uint(vertRestartNext)];
            if(newPrev == -1)
            {
                found_second_border = true;
                std::cout << " case 3 problem" << std::endl;
            }

            uint newVertPrev = uint(corners[uint(newPrev)].c());

            int test_iter = 0;

            while( !found_second_border && test_iter < 10 )
            {

                int newNext = previous(newPrev);

                temp_ring.push_back(newVertPrev);

                newPrev = opposites[uint(newNext)];
                if(newPrev == -1)
                {
                    std::cout << " found second border " << std::endl;
                    found_second_border = true;
                }
                else newVertPrev = uint(corners[uint(newPrev)].c());
                std::cout << " case 3 iteration:" << test_iter << std::endl;
                test_iter++;
             }
            ring = temp_ring;
        }
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
    if(cos_a > 1.0f) cos_a = 1.0f;
    if(cos_a < -1.0f) cos_a = -1.0f;
    float sin_a = sin(alpha);
    if(sin_a > 1.0f) cos_a = 1.0f;
    if(sin_a < -1.0f) cos_a = -1.0f;
    float cos_b= cos(alpha);
    if(cos_b > 1.0f) cos_b = 1.0f;
    if(cos_b < -1.0f) cos_b = -1.0f;
    float sin_b = sin(beta);
    if(sin_b > 1.0f) cos_b = 1.0f;
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
    else return cotg_sum_b;
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
            uint rj = r+1;
            uint v_j = ring[rj];

             if( r == (ring.size()-1) )
            {
                rj= 0;
                v_j = ring[rj];
            }

            glm::vec3 vert_i = glm::vec3(vertices[v].x(), vertices[v].y(), vertices[v].z() );
            glm::vec3 vert_j = glm::vec3(vertices[v_j].x(), vertices[v_j].y(), vertices[v_j].z() );

            float cotg_sum = computeCotangent(v,ring, r);

            tot_sum = tot_sum + (cotg_sum *(vert_j -vert_i));
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
//loop through all the vertices and call computeLaplacian
void TriangleMesh::computeLaplacianOperator(float lambda_in, WeightType selected_weight_type)
{

    std::cout << "compute laplacian" << std::endl;

    laplace_operators.clear();
    laplace_vertices.clear();

    for( uint v = 0; v < vertices.size(); v++)
    {
        std::vector<uint> ring;
        computeRing(ring, v);

        glm::vec3 laplacian_v = computeLaplacian(v, ring, selected_weight_type);

        laplace_operators.push_back(QVector3D(laplacian_v.x,laplacian_v.y,laplacian_v.z));

        glm::vec3 vec_i = glm::vec3(vertices[v].x(),vertices[v].y(),vertices[v].z());
        glm::vec3 new_vert_i = vec_i + lambda*laplacian_v;
        laplace_vertices.push_back(QVector3D(new_vert_i.x,new_vert_i.y,new_vert_i.z));
    }
    std::cout << "Computed Laplacian of " << vertices.size() << "vertices" << std::endl;
}

//single laplacian computation
glm::vec3 TriangleMesh::computeLaplacian(uint v, std::vector<uint> ring, WeightType selected_weight_type)
{
    glm::vec3 laplacian_v = glm::vec3(0.0,0.0,0.0);
    glm::vec3 vec_i = glm::vec3(vertices[v].x(),vertices[v].y(),vertices[v].z());

    //COMPUTATION OF THE LAPLACIAN UNIFORM OR COTANGENT
    float total_weight = 0;
    //UNIFORM:
    if (selected_weight_type == WeightType::UNIFORM)
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
    else if (selected_weight_type == WeightType::COTANGENT)  //COTANGENT
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
    return laplacian_v;
}
//manage the application of the laplacians
void TriangleMesh::applyLaplacian()
{
    vertices = getLaplacianVertices(current_iterative_type, current_weight_type, 1);
}

//apply laplacian that give back the vertices
vector<QVector3D>  TriangleMesh::getLaplacianVertices( IterativeType iterative_type,WeightType weight_type, int iteration)
{
    vector<QVector3D> new_smooth_vertices;
    vector<QVector3D> old_vertices = vertices;
    for(uint iter = 0; iter < iteration; iter++)
    {
        computeLaplacianOperator(lambda,weight_type);
        if(iterative_type == IterativeType::NORMAL)
            new_smooth_vertices = laplace_vertices;
        else if(iterative_type == IterativeType::BILAPLACIAN)
        {
            new_smooth_vertices = laplace_vertices;
            computeLaplacianOperator(-lambda,weight_type);
            new_smooth_vertices = laplace_vertices;
        }
        else if(iterative_type == IterativeType::TAUBIN)
        {
            new_smooth_vertices = laplace_vertices;
            float Kpb = 0.1f;
            float mu = 1.0f/(Kpb + (1.0f/lambda));
            computeLaplacianOperator(mu,weight_type);
            new_smooth_vertices = laplace_vertices;
        }

        //set the general vertices as smooth one fot the next step of the iteration
        vertices = new_smooth_vertices;
    }


    //not necessary: reset the general vertices to the previous one
    vertices = old_vertices;
    return new_smooth_vertices;

}










//~~~LAB~~~3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~GLOBAL~~~~~~~~~~SMOOTHING
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::applyGlobalSmoothing()
{
//    matrixClass = geomfunctions();
//    matrixClass.buildMatrixA();
    buildMatrixA();
}
//create the fixed list
vector<int> TriangleMesh::fixVertices()
{
    fixedVertices.clear();
    vector<int> fix_checking;
    int iterator = 0;
    vector<uint> unfixed_vertices;
    for(uint v = 0; v < vertices.size(); v++)
    {
        int r = rand() % 100;
        if(r < percentage)
        {
            fixedVertices.push_back(v);
            fix_checking.push_back(-1);
        }
        else
        {
            fix_checking.push_back(iterator);
            iterator++;
            unfixed_vertices.push_back(v);
        }
    }
    std::cout << " fixed  "<< fixedVertices.size() << "  vertices out of " << vertices.size() << "  with percentage : " << percentage << std::endl;
    return fix_checking;
}

//check if a vertices is inside the fixed list
std::vector<double> TriangleMesh::computeWeight(uint v, std::vector<uint> ring, double &total_weight )
{
    std::vector<double> weightPerRing;
    total_weight = 0.0;

    for(uint r = 0; r < ring.size(); r++)
    {
        if (current_weight_type == WeightType::UNIFORM)
        {
            weightPerRing.push_back(1);
        }
        else if (current_weight_type == WeightType::COTANGENT)
        {
            double weight = double( computeCotangent(uint(v),ring,r,false) );
            weightPerRing.push_back(weight);
            total_weight = total_weight + weight;
        }
    }
    if(current_weight_type == WeightType::UNIFORM)
        total_weight = ring.size();

    return weightPerRing;
}

//COMPUTATION
void TriangleMesh::buildMatrixA()
{

    vector<int> fix_checking = fixVertices();

    int n = int( 3*vertices.size() );

    int fixed = int( 3*fixedVertices.size());

    //matrix A
    Eigen::SparseMatrix<double> matrixA = Eigen::SparseMatrix<double>( n , (n-fixed) );
    Eigen::SparseMatrix<double> vectorB = Eigen::SparseMatrix<double>(n, 3);

    std::cout << " sizes : matrix A: " << n <<" x " << n-fixed << "  and vector B: "<< n << " x " << 3 << std::endl;

    //coefficents T = Eigen::Triplet<double>
    std::vector<Triplet> coefficentsA;
    std::vector<Triplet> coefficentsB;
    //vector of known values
//    Eigen::VectorXd b(n);


    //loop over all the vertices:
    for(uint v = 0; v < vertices.size(); v ++)
    {
        std::vector<uint> ring;
        computeRing(ring, v);



        int column_i = fix_checking[v];

        //if a vertex is not fixed just put is own value in the column
        if( fix_checking[v] != -1 )
        {
            //add its own position
            Triplet own_value_x = Triplet( 3*int(v),   3*column_i,   -1 );
            Triplet own_value_y = Triplet( 3*int(v)+1, 3*column_i+1, -1 );
            Triplet own_value_z = Triplet( 3*int(v)+2, 3*column_i+2, -1 );

            coefficentsA.push_back(own_value_x);
            coefficentsA.push_back(own_value_y);
            coefficentsA.push_back(own_value_z);


            //if inside cactus, if outside no cactus
            //add the weight to the one in the ring for each vertex
            double total_weight = 0.0;
            vector<double> weigthPerRing = computeWeight(v, ring, total_weight);
            //uniform
//            double w = 1.0/ring.size();
            //b values
            double bx_i = 0.0, by_i = 0.0, bz_i = 0.0;

            for(uint r = 0; r < ring.size(); r++)
            {
                uint ri = ring[r];
                int ring_column_i = fix_checking[ri];

                //if the neighbour is fixed compute the value of B, don't add any column
                if( fix_checking[ri] == -1 )
                {
//                    bx_i = bx_i + double(vertices[ri].x());
//                    by_i = by_i + double(vertices[ri].y());
//                    bz_i = bz_i + double(vertices[ri].z());
                    bx_i = bx_i + double(vertices[ri].x())*weigthPerRing[r]/total_weight;
                    by_i = by_i + double(vertices[ri].y())*weigthPerRing[r]/total_weight;
                    bz_i = bz_i + double(vertices[ri].z())*weigthPerRing[r]/total_weight;
                }
                else
                {
                    //else put the waight w in the right column
                    Triplet new_value_x = Triplet( 3*int(v),   3*ring_column_i,   weigthPerRing[r]/total_weight);
                    Triplet new_value_y = Triplet( 3*int(v)+1, 3*ring_column_i+1, weigthPerRing[r]/total_weight);
                    Triplet new_value_z = Triplet( 3*int(v)+2, 3*ring_column_i+2, weigthPerRing[r]/total_weight);

                    coefficentsA.push_back(new_value_x);
                    coefficentsA.push_back(new_value_y);
                    coefficentsA.push_back(new_value_z);
                }

            }

            //fill the b vertex
            Triplet new_b_value_x = Triplet( 3*int(v),   0, -(bx_i) );
            Triplet new_b_value_y = Triplet( 3*int(v)+1, 1, -(by_i) );
            Triplet new_b_value_z = Triplet( 3*int(v)+2, 2, -(bz_i) );

            coefficentsB.push_back(new_b_value_x);
            coefficentsB.push_back(new_b_value_y);
            coefficentsB.push_back(new_b_value_z);


        }
    }

    //fill the matrices
    matrixA.setFromTriplets(coefficentsA.begin(), coefficentsA.end());
    vectorB.setFromTriplets(coefficentsB.begin(), coefficentsB.end());


    Eigen::SimplicialCholesky<Eigen::SparseMatrix<double>> solver;
    solver.compute(matrixA.transpose()*matrixA);
    Eigen::SparseMatrix<double> solution = solver.solve(matrixA.transpose()*vectorB);



    //UPDATING THE VERTICES
    for(uint v_i = 0; v_i < fix_checking.size(); v_i++)
    {
        //if is not fixed we replace the new vertex with the old one
        if(fix_checking[v_i] != -1 )
        {
            uint index = uint(fix_checking[v_i]);
            QVector3D new_vector = QVector3D( solution.coeff(3*index, 0), solution.coeff(3*index+1, 1), solution.coeff(3*index+2, 2)  );
            vertices[v_i] = new_vector;
        }

    }

}













//~~~LAB~~~4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MAGNIFY~~DETAILS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::applyMagnify()
{
    vector<QVector3D> smoothVertices1;
    vector<QVector3D> smoothVertices2;
    vector<QVector3D> newMagnifyVertices;

    smoothVertices1 = getLaplacianVertices(IterativeType::NORMAL, current_weight_type, number_of_iteration);
    smoothVertices2 = getLaplacianVertices(IterativeType::NORMAL, current_weight_type, mult_of_iteration*number_of_iteration);

    std::cout << " compute smoothing" << std::endl;
    std::cout << " sizes: vertices  " << vertices.size() <<  "   smooth1 : " << smoothVertices1.size() << "  smooth2: " << smoothVertices2.size() << std::endl;

    //M + lambda*(smooth1 - smooth2)
    for(uint v = 0; v < vertices.size(); v++)
    {

        vertices[v] = vertices[v] +  magnify_par*(smoothVertices1[v] - smoothVertices2[v]);
    }

//    vertices = newMagnifyVertices;


}







//~~~LAB~~~5~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~DISCRETE~HARMONIC~MAPPING
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TriangleMesh::computeParametrization()
{
    orderBorderChain();
    std::cout << "done with order border chain" << std::endl;
    parametrizeChain();
    std::cout << "done with parametrize chain" << std::endl;

    parametrizeOtherVertices();
}


void TriangleMesh::parametrizeChain()
{
    parametrizeVertices.clear();
    parametrizeVertices.resize(vertices.size());

    float circonf = float(2*M_PI);

    //compute chein length
    float totalDistance = 0;
    std::vector<float> individualDistance;
    for(uint b = 0; b < border.size(); b++)
    {
        glm::vec3 v1,v2;
        if(b == border.size()-1)
        {
            v1 = glm::vec3(vertices[border[b]].x(),vertices[border[b]].y(),vertices[border[b]].z());
            v2 = glm::vec3(vertices[border[0]].x(),vertices[border[0]].y(),vertices[border[0]].z());
        }
        else
        {
            v1 = glm::vec3(vertices[border[b]].x(),vertices[border[b]].y(),vertices[border[b]].z());
            v2 = glm::vec3(vertices[border[b+1]].x(),vertices[border[b+1]].y(),vertices[border[b+1]].z());

        }

        float distance = glm::distance(v1,v2);
        totalDistance = totalDistance + distance;
        individualDistance.push_back(totalDistance);
//        std::cout << "  have distance :" << distance << " and total distance is : " << totalDistance << std::endl;
    }

    //unit step on the circonference
    float unit_step = circonf/totalDistance;
    glm::vec2 center = glm::vec2(0.5,0.5);
    float radius = 0.5;
    //parametrize the border
    for(uint b = 0; b < border.size(); b++)
    {
        glm::vec2 pos;
        float angle = individualDistance[b]*unit_step;
        //first parametrize value on the right, on (1, 0.5) of my parametrization sapce
        if(b == 0)
            pos = center + glm::vec2(radius,0.0);
        else pos =  center + glm::vec2( cos(angle)*radius, sin(angle)*radius );
        uint index = border[b];
        parametrizeVertices[index] = QVector2D(pos.x,pos.y);
    }
}


void TriangleMesh::parametrizeOtherVertices()
{

    //compute fix border checking
    int iter= 0;
    for(uint i = 0; i< vertices.size(); i ++)
    {
        bool isInBorder = false;
        for(uint b = 0; b < border.size(); b++)
        {
            if(i == border[b] )
                isInBorder = true;
        }
        if(isInBorder)
        {
            fix_border_checking.push_back(iter);
            iter++;
        }
        else fix_border_checking.push_back(-1);
    }





    int n = int( 2*vertices.size() );

    int fixed = int( 2*border.size());

    //matrix A
    Eigen::SparseMatrix<double> matrixA = Eigen::SparseMatrix<double>( n , (n-fixed) );
    Eigen::SparseMatrix<double> vectorB = Eigen::SparseMatrix<double>(n, 2);

    std::cout << " sizes : matrix A: " << n <<" x " << n-fixed << "  and vector B: "<< n << " x " << 3 << std::endl;

    //coefficents T = Eigen::Triplet<double>
    std::vector<Triplet> coefficentsA;
    std::vector<Triplet> coefficentsB;
    //vector of known values
//    Eigen::VectorXd b(n);


    //loop over all the vertices:
    for(uint v = 0; v < vertices.size(); v ++)
    {
        std::vector<uint> ring;
        newComputeRing(ring, v);



        int column_i = fix_border_checking[v];

        //if a vertex is not fixed just put is own value in the column
        if( fix_border_checking[v] != -1 )
        {
            //add its own position
            Triplet own_value_x = Triplet( 2*int(v),   2*column_i,   -1 );
            Triplet own_value_y = Triplet( 2*int(v)+1, 2*column_i+1, -1 );
//            Triplet own_value_z = Triplet( 3*int(v)+2, 3*column_i+2, -1 );

            coefficentsA.push_back(own_value_x);
            coefficentsA.push_back(own_value_y);
//            coefficentsA.push_back(own_value_z);


            //if inside cactus, if outside no cactus
            //add the weight to the one in the ring for each vertex
            double total_weight = 0.0;
            vector<double> weigthPerRing = computeWeight(v, ring, total_weight);
            //b values
            double bx_i = 0.0, by_i = 0.0;

            for(uint r = 0; r < ring.size(); r++)
            {
                uint ri = ring[r];
                int ring_column_i = fix_border_checking[ri];

                //if the neighbour is fixed compute the value of B, don't add any column
                if( fix_border_checking[ri] == -1 )
                {
//                    bx_i = bx_i + double(vertices[ri].x());
//                    by_i = by_i + double(vertices[ri].y());
//                    bz_i = bz_i + double(vertices[ri].z());
                    bx_i = bx_i + double(vertices[ri].x())*weigthPerRing[r]/total_weight;
                    by_i = by_i + double(vertices[ri].y())*weigthPerRing[r]/total_weight;
//                    bz_i = bz_i + double(vertices[ri].z())*weigthPerRing[r]/total_weight;
                }
                else
                {
                    //else put the waight w in the right column
                    Triplet new_value_x = Triplet( 2*int(v),   2*ring_column_i,   weigthPerRing[r]/total_weight);
                    Triplet new_value_y = Triplet( 2*int(v)+1, 2*ring_column_i+1, weigthPerRing[r]/total_weight);
//                    Triplet new_value_z = Triplet( 3*int(v)+2, 3*ring_column_i+2, weigthPerRing[r]/total_weight);

                    coefficentsA.push_back(new_value_x);
                    coefficentsA.push_back(new_value_y);
//                    coefficentsA.push_back(new_value_z);
                }

            }

            //fill the b vertex
            Triplet new_b_value_x = Triplet( 2*int(v),   0, -(bx_i) );
            Triplet new_b_value_y = Triplet( 2*int(v)+1, 1, -(by_i) );
//            Triplet new_b_value_z = Triplet( 3*int(v)+2, 2, -(bz_i) );

            coefficentsB.push_back(new_b_value_x);
            coefficentsB.push_back(new_b_value_y);
//            coefficentsB.push_back(new_b_value_z);


        }
    }

    //fill the matrices
    matrixA.setFromTriplets(coefficentsA.begin(), coefficentsA.end());
    vectorB.setFromTriplets(coefficentsB.begin(), coefficentsB.end());


    Eigen::SimplicialCholesky<Eigen::SparseMatrix<double>> solver;
    solver.compute(matrixA.transpose()*matrixA);
    Eigen::SparseMatrix<double> solution = solver.solve(matrixA.transpose()*vectorB);



    //UPDATING THE VERTICES
    for(uint v_i = 0; v_i < fix_border_checking.size(); v_i++)
    {
        //if is not fixed we replace the new vertex with the old one
        if(fix_border_checking[v_i] != -1 )
        {
            uint index = uint(fix_border_checking[v_i]);
            QVector2D new_vector = QVector2D( solution.coeff(2*index, 0), solution.coeff(2*index+1, 1) );
            parametrizeVertices[v_i] = new_vector;
        }
    }
}




//check for each vertex vert if it is one of the border vertices
void TriangleMesh::orderBorderChain()
{

    border.clear();
    //check for each edge of the border chain
    std::cout << border_chain.size() << std::endl;
    
    //to check if a element of the border chain was already added to the list
    vector<bool> check_border_chain(border_chain.size(),true);

    uint starting_first = border_chain[0].first;
    uint starting_second = border_chain[0].second;
    border.push_back(starting_first);
    border.push_back(starting_second);
    check_border_chain[0] = false;


    while(border.size() < border_chain.size())
    {
        //we get the last added vector to border
        uint last_added = border.back();


        //find the next one
        for(uint b = 0; b < border_chain.size(); b++)
        {
            if(check_border_chain[b])
            {
                uint next_first = border_chain[b].first;
                uint next_second = border_chain[b].second;

                if(last_added == next_first)
                {
                    border.push_back(next_second);
                    check_border_chain[b] = false;
                }
                else if(last_added == next_second)
                {
                    border.push_back(next_first);
                    check_border_chain[b] = false;
                }
            }
        }
    }



//    //test
//    for(uint b = 0; b < border.size()-1; b++)
//    {

//        std::cout << " border : " << border[b] << " - "  << border[b+1] << std::endl;
//    }

//    std::cout << "finish border size: " << border.size() << std::endl;



}



















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
