#include <iostream>
#include "trianglemesh.h"

#include <glm/glm.hpp>
#include <math.h>

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
	vector<QVector3D> replicatedVertices, normals;
	vector<unsigned int> perFaceTriangles;

    vector<QVector2D> replicatedColors;

    computeCornerTables();
    computeVertexCorners();
    principalCurvatures();

    buildReplicatedVertices(replicatedVertices, normals, perFaceTriangles, replicatedColors);



	program->bind();

	vao.destroy();
	vao.create();
	if(vao.isCreated())
		vao.bind();
	else
		return false;

	vboVertices.destroy();
	vboVertices.create();
	if(vboVertices.isCreated())
		vboVertices.bind();
	else
		return false;
	vboVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
	program->enableAttributeArray(0);
	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

	vboNormals.destroy();
	vboNormals.create();
	if(vboNormals.isCreated())
		vboNormals.bind();
	else
		return false;
	vboNormals.setUsagePattern(QOpenGLBuffer::StaticDraw);
	program->enableAttributeArray(1);
	program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);

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
    program->enableAttributeArray(2);
    program->setAttributeBuffer(2, GL_FLOAT, 0, 2, 0);

//    //gaussian curvature vbo
//    vboColorGauss.destroy();
//    vboColorGauss.create();
//    if(vboColorGauss.isCreated())
//        vboColorGauss.bind();
//    else
//        return false;
//    vboColorGauss.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    program->enableAttributeArray(2);
//    program->setAttributeBuffer(2, GL_FLOAT, 0, 1, 0);

//    //mean curvature vbo
//    vboColorMean.destroy();
//    vboColorMean.create();
//    if(vboColorMean.isCreated())
//        vboColorMean.bind();
//    else
//        return false;
//    vboColorMean.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    program->enableAttributeArray(3);
//    program->setAttributeBuffer(3, GL_FLOAT, 0, 1, 0);




    fillVBOs(replicatedVertices, normals, perFaceTriangles, replicatedColors);

	vao.release();
	program->release();


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

//    vboColorMean.destroy();
//    vboColorGauss.destroy();

    vboCurvatureColors.destroy();

    gaussianCurvature.clear();
    meanCurvature.clear();
    corners.clear();
    cornersVertices.clear();
    oppositesVec.clear();
    opposites.clear();

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


        replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i]], meanCurvature[triangles[i]]) );
        replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i+1]], meanCurvature[triangles[i+1]]));
        replicatedColors.push_back( QVector2D(gaussianCurvature[triangles[i+2]], meanCurvature[triangles[i+2]]));

	}
}

void TriangleMesh::fillVBOs(vector<QVector3D> &replicatedVertices, vector<QVector3D> &normals, vector<unsigned int> &perFaceTriangles,  vector<QVector2D> &replicatedColors)
{
    std::cout << "VBO: filling gaussian vbo, of size: "<< replicatedVertices.size()/3 << std::endl;
	vboVertices.bind();
	vboVertices.allocate(&replicatedVertices[0], 3 * sizeof(float) * replicatedVertices.size());
	vboVertices.release();

	vboNormals.bind();
	vboNormals.allocate(&normals[0], 3 * sizeof(float) * normals.size());
	vboNormals.release();

	eboTriangles.bind();
	eboTriangles.allocate(&perFaceTriangles[0], sizeof(int) * perFaceTriangles.size());
	eboTriangles.release();


//    std::cout << "VBO: filling gaussian vbo, of size: "<< gaussianCurvature.size() << std::endl;
//    vboColorGauss.bind();
//    vboColorGauss.allocate(&gaussianCurvature[0], sizeof(float) * gaussianCurvature.size());
//    vboColorGauss.release();

//    std::cout << "VBO: filling mean vbo, of size: "<< meanCurvature.size() << std::endl;
//    vboColorMean.bind();
//    vboColorMean.allocate(&meanCurvature[0], sizeof(float) * meanCurvature.size());
//    vboColorMean.release();

    std::cout << "VBO: filling gaussian vbo, of size: "<< replicatedVertices.size()/3 << std::endl;
    vboCurvatureColors.bind();
    vboCurvatureColors.allocate(&replicatedColors[0], 2 * sizeof(float) * replicatedVertices.size());
    vboCurvatureColors.release();


}


void TriangleMesh::computeCornerTables()
{


    //compare all the triangles

    std::vector<int> edges;

    for (uint t1 = 0; t1 < triangles.size()-2; t1 = t1+3)
    {
        std::cout << "trianglo :" << t1 << std::endl;

        int a = triangles[t1];
        int b = triangles[t1 + 1];
        int c = triangles[t1 + 2];

        std::cout << "a:" << a << "  b:" << b <<"  c:" << c << std::endl;

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

        std::cout << "1st entry index:"<<t1 <<"  values:" << c1.a() << "  :" << c1.b() << "  :" << c1.c() << std::endl;
        std::cout << "2nd entry index:"<<t1+1 <<"  values:" << c2.a() << "  :" << c2.b() << "  :" << c2.c() << std::endl;
        std::cout << "3rd entry index:"<<t1+2 <<"  values:" << c3.a() << "  :" << c3.b() << "  :" << c3.c() << std::endl;


        corners.push_back(c1);
        corners.push_back(c2);
        corners.push_back(c3);

        std::cout << "created corners: c1 :" << c1.c() << "  c2 :" << c2.c() << "  c3 : " << c3.c() << std::endl;
    }

    std::cout << "corners created , size: " << corners.size() << " compare to triangles : " << triangles.size() << std::endl;


    //opposite table

    oppositesVec.resize(corners.size());
    opposites.resize(corners.size());
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
                CornerEdge o1,o2;
                o1.set(a1,b1,c2);
                o2.set(a2,b2,c1);

                oppositesVec[i] = o1;
                oppositesVec[j] = o2;

                opposites[i] = j;
                opposites[j] = i;

                std::cout << "set the values of corners :" << std::endl << "ind:" << i << "  value:" << opposites[i] << std::endl << "ind: " << j << "  value:" << opposites[j] << std::endl;
            }
        }

    }
    
}


//COMPUTE FOR EACH VERTEX ONE OF THE CORRISPONDENT CORNERS
//N.B: ALSO COMPUTE THE BOUNDIG BOX
void TriangleMesh::computeVertexCorners()
{
    //bounding box variables
    float min_x,min_y,min_z, max_x,max_y,max_z;

    std::cout << " " <<std::endl;
    std::cout << "*****VERTEX CORNER ***** " <<std::endl;
    std::cout << " " <<std::endl;

    cornersVertices.resize(vertices.size());

    for( uint v = 0; v < (int)vertices.size() ; v++)
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
        std::cout << " for vertex v: "<<v<< " we defined corner c: " << cornersVertices[v] << std::endl;
    }


    boundingBoxMin = QVector3D(min_x, min_y, min_z);
    boundingBoxMax = QVector3D(max_x, max_y, max_z);
}





void TriangleMesh::principalCurvatures()
{
    //gaussian curvature
    std::cout << " " <<std::endl;
    std::cout << " " <<std::endl;

    gaussianCurvature.clear();
    meanCurvature.clear();

    for( int v = 0; v < vertices.size(); v++)
    {

        std::vector<int> ring;

        std::cout << "computing gaussian curvature of vertex "<< v <<std::endl;
        int corner = cornersVertices[v];
        std::cout << "extracted corner " << corner << std::endl;

        //first triangle
        int nex, prev ;

        nex = next(corner);
        prev = previous(corner);



        int vertNext = corners[nex].c();
        int vertPrev = corners[prev].c();

//        ring.push_back(vertPrev);
        ring.push_back(vertNext);


        std::cout << "next corner:" << nex  <<" vertex:"<<vertNext << ";   previous corner:"<< prev <<" vertex:" << vertPrev << std::endl;


        int newNext = opposites[prev];

        int newVertNext = corners[newNext].c();

        std::cout << "opposite corner / new next corner:" << newNext  << " new next vertex:" << newVertNext << std::endl;

        while( newVertNext != vertNext)
        {

            int newPrev = next(newNext);


            int newVertPrev = corners[newPrev].c();

//            std::cout << "new prev:" << newPrev  << "  vertex:" << newVertPrev << std::endl;
            std::cout  << "new next:" << newNext << " vertex:" <<  newVertNext << std::endl;


//            ring.push_back(newVertPrev);
            ring.push_back(newVertNext);

            newNext = opposites[newPrev];
            newVertNext = corners[newNext].c();

        }


        std::cout << "for vertex :"<<v<< "  we get a ring of " << ring.size() << " vertices" <<std::endl;

        std::cout << " " <<std::endl;

        std::vector<float> areasRing;
        std::vector<float> anglesRing;

        //computation of the areas and anfles of each triangle of the ring
        for( uint r = 0; r < ring.size(); r++ )
        {

            int v2 = ring[r];
            int v3 = ring[r+1];
            if( r == (ring.size()-1) )
            {
                v2 = ring[r];
                v3 = ring[0];
            }

            glm::vec3 vert1 = glm::vec3(vertices[v].x(), vertices[v].y(), vertices[v].z() );
            glm::vec3 vert2 = glm::vec3(vertices[v2].x(), vertices[v2].y(), vertices[v2].z() );
            glm::vec3 vert3 = glm::vec3(vertices[v3].x(), vertices[v3].y(), vertices[v3].z() );


            float area = 0.5f* glm::length(glm::cross((vert2 - vert1),(vert3 - vert1)));

            std::cout << "area of the ring:" << area  <<std::endl;

            areasRing.push_back(area);


            glm::vec3 vect1 = glm::normalize(vert2 - vert1);
            glm::vec3 vect2 = glm::normalize(vert3 - vert1);

            float cosine = glm::dot(vect1,vect2);
            float angle = acos(cosine);

            std::cout << "angle:" << angle  << std::endl;
            anglesRing.push_back(angle);

        }
        std::cout << " computed " << areasRing.size() << "  areas" << std::endl;
        std::cout << " " <<std::endl;
        std::cout << " " <<std::endl;
        std::cout << "*****GAUSSIAN CURVATURE ***** " <<std::endl;
        std::cout << " " <<std::endl;

        float tot_angles = 0;
        for( uint ang = 0; ang < anglesRing.size(); ang ++)
            tot_angles = tot_angles + anglesRing[ang];
        std::cout <<" sum angles: " <<tot_angles << std::endl;

        float tot_areas = 0;
        for( uint ar = 0; ar < areasRing.size(); ar ++)
            tot_areas = tot_areas + areasRing[ar];

        std::cout <<" sum areas: " <<tot_areas << std::endl;


        float A = tot_areas/3;

        float Kg = (2*PI - tot_angles)/A;

        gaussianCurvature.push_back(Kg);
        std::cout << "for vertex v: " << v << " the gaussian curvature Kg: " << Kg << std::endl;

        std::cout << " " <<std::endl;
        std::cout << "*****MEAN CURVATURE ***** " <<std::endl;
        std::cout << " " <<std::endl;

        glm::vec3 tot_sum = glm::vec3(0);

        for( uint r = 0; r < ring.size(); r++ )
        {
            float alpha,beta = 0.0f;

            int v_beta = ring[r];
            int v_j = ring[r+1];
            int v_alpha = ring[r+2];

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

            glm::vec3 vert_i = glm::vec3(vertices[v].x(), vertices[v].y(), vertices[v].z() );
            glm::vec3 vert_beta = glm::vec3(vertices[v_beta].x(), vertices[v_beta].y(), vertices[v_beta].z() );
            glm::vec3 vert_j = glm::vec3(vertices[v_j].x(), vertices[v_j].y(), vertices[v_j].z() );
            glm::vec3 vert_alpha = glm::vec3(vertices[v_alpha].x(), vertices[v_alpha].y(), vertices[v_alpha].z() );


            glm::vec3 vec_beta_1 = glm::normalize(vert_i - vert_beta);
            glm::vec3 vec_beta_2 = glm::normalize(vert_j - vert_beta);

            float cosine_beta = glm::dot(vec_beta_1,vec_beta_2);
            beta = acos(cosine_beta);

            std::cout << " beta: " << beta << std::endl;


            glm::vec3 vec_alpha_1 = glm::normalize(vert_i - vert_alpha);
            glm::vec3 vec_alpha_2 = glm::normalize(vert_j - vert_alpha);

            float cosine_alpha = glm::dot(vec_alpha_1,vec_alpha_2);
            alpha = acos(cosine_alpha);

            std::cout << " alpha: " << alpha << std::endl;



            float cotg_sum_b = 1.0f/tan(alpha) + 1.0f/tan(beta);
            float cotg_sum = cos(alpha)/sin(alpha) + cos(beta)/tan(beta);

            std::cout << "  b)sum(cotg(beta) + cotg(alpha)): " << cotg_sum_b << std::endl;
            std::cout << "  a)sum(cotg(beta) + cotg(alpha)): " << cotg_sum << std::endl;



            glm::vec3 vec_ij = vert_j -vert_i;
            std::cout << "  vector i-j : " << vec_ij.x << ", " << vec_ij.y << ", " << vec_ij.z << std::endl;



            tot_sum = tot_sum + (cotg_sum *vec_ij);
            std::cout << "  tot_sum : " << tot_sum.x << ", " << tot_sum.y << ", " << tot_sum.z << std::endl;


        }

        std::cout << " " <<std::endl;

        glm::vec3 Sv;
        Sv = 1/(2*A) * tot_sum;
        std::cout << "  Sv : " << Sv.x << ", " << Sv.y << ", " << Sv.z << std::endl;
        std::cout << "  length(Sv) : " << glm::length(Sv);

        float Kh = (glm::length(Sv))/2.0f;


        std::cout << " " <<std::endl;
        std:: cout << " mean curvature of vertex :" << v << "  is :" << Kh << std::endl;

        meanCurvature.push_back(Kh);

        std::cout << " " <<std::endl;
        std::cout << "*************************************************************** " <<std::endl;
        std::cout << " " <<std::endl;




    }



    std::cout << "******* bounding box :" << std::endl;
    std::cout << "  min: x: " <<boundingBoxMin.x()<< " min: y:" <<boundingBoxMin.y()<< " min: z:" <<boundingBoxMin.z() <<std::endl;
    std::cout << "  max: x: " <<boundingBoxMax.x()<< " max: y:" <<boundingBoxMax.y()<< " max: z:" <<boundingBoxMax.z() <<std::endl;



}
