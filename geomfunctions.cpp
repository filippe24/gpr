#include "geomfunctions.h"

geomfunctions::geomfunctions()
{

}

//~~~LAB~~~3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LAPLACIAN: LINEAR SISTEM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void geomfunctions::buildMatrixA()
{
    cout << endl;

//    int n = mesh.getVerticesSize(); // number of vertices
//    cout << " number of vertices : " << n << endl;

//    // Assembly:
//    std::vector<T> coefficients;            // list of non-zeros coefficients
//    Eigen::VectorXd b(n);                   // the right hand side-vector resulting from the constraints

//    cout << " matrix A : number of columuns " <<matrixA.cols()<< "  number of row " << matrixA.rows() << endl;
//    //BUILD COEFFICENTS
//    for(int v = 0; v < mesh.getVerticesSize(); v++)
//    {
//        vector <uint> ring;
//        mesh.computeRing(ring, uint(v));

//        //1:uniform case:

//        //first value -1 for (v,v) position in the matrix
//        coefficients.push_back(T(v,v,-1.0));
//        double weight = 1.0/ring.size();
//        //all the other value
//        for(int r = 0; r < ring.size() ; r++)
//        {
//            coefficients.push_back(T(v,r,weight));
//        }
//    }
//    SparseMatrix A(n,n);
//    A.setFromTriplets(coefficients.begin(), coefficients.end());

//    matrixA = A;
//    cout << " matrix A : number of columuns " <<matrixA.cols()<< "  number of row " << matrixA.rows() << endl;

}
