#ifndef FONCTIONSUTILES_H
#define FONCTIONSUTILES_H
#include <QtWidgets>
#include <math.h>
#include "librairieAlglib/stdafx.h"
#include "librairieAlglib/statistics.h"
#include "librairieAlglib/linalg.h"

using namespace alglib;
class fonctionsUtiles
{
public:
    fonctionsUtiles();
    static double median(QVector<double> data);
    static real_2d_array convert(QList<QVector<double> *>dataToConvert);
    static QList<QVector<double> *> convertInv(real_2d_array dataToConvert);
    static real_2d_array MatriceProduct(real_2d_array a,real_2d_array b);
    static real_2d_array GenerateMatrix(int ncol, int nrow, int nbMax);
    static real_1d_array generateVector(int nb,int nbmax);
    static real_2d_array TransposeMatrix(real_2d_array vectors);
    static real_2d_array BuildMatrixDiag(real_1d_array diag);
    static real_2d_array ComputeWhiteningMatrix(real_2d_array RealData);
    static int max(int a ,int b);
    static int min(int a,int b);
    static void MakeSet(QVector<int> *vector, int nbFrameB, int nbframeA);
    static QList<QVector<double> *> InitialiserQlist(int size);
    static int getIndexMax(QVector<double> vector);

};

#endif // FONCTIONSUTILES_H
