#ifndef GEOMFUNCTIONS_H
#define GEOMFUNCTIONS_H

#include <QVector3D>
#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>
#include <iostream>
#include "trianglemesh.h"




using namespace std;

class geomfunctions
{
public:
    typedef Eigen::SparseMatrix<double> SparseMatrix; // declares a column-major sparse matrix type of double
    typedef Eigen::Triplet<double> T; // a small structure to hold a non zero as a triplet (i,j,value)
    geomfunctions();
    void buildMatrixA(TriangleMesh &mesh);


private:
    void buildCoefficent(vector<T> &coefficents);


private:
    Eigen::SparseMatrix<double> matrixA;



};

#endif // GEOMFUNCTIONS_H
