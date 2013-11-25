#include "fonctionsutiles.h"

fonctionsUtiles::fonctionsUtiles()
{
}

double fonctionsUtiles::median(QVector<double> data)
{
    QVector<double> sortData=data;
    qSort(sortData);
    return sortData[data.count()/2];
}

real_2d_array fonctionsUtiles::convert(QList<QVector<double> *> dataToConvert)
{
    int m=dataToConvert.count();
    int n=dataToConvert[0]->count();
    real_2d_array res;
    res.setlength(n,m);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            res[j][i]=dataToConvert[i]->operator [](j);
        }
    }
    return res;
}

real_2d_array fonctionsUtiles::MatriceProduct(real_2d_array a, real_2d_array b)
{
    real_2d_array res;
    res.setlength(a.rows(),b.cols());
    //matrice product
    ae_int_t m = a.rows();
    ae_int_t n = b.cols();
    ae_int_t k = a.cols();
    double alpha = 1.0;
    ae_int_t ia = 0;
    ae_int_t ja = 0;
    ae_int_t optypea = 0;
    ae_int_t ib = 0;
    ae_int_t jb = 0;
    ae_int_t optypeb = 0;
    double beta = 0.0;
    ae_int_t ic = 0;
    ae_int_t jc = 0;
    rmatrixgemm(m, n, k, alpha,a, ia, ja, optypea, b, ib, jb, optypeb, beta, res, ic, jc);
    return res;
}


QList<QVector<double> *> fonctionsUtiles::convertInv(real_2d_array dataToConvert)
{
    QList<QVector<double> *> res;
    for(int i=0;i<dataToConvert.cols();i++)
    {
        QVector<double> *vec=new QVector<double>;
        for(int j=0;j<dataToConvert.rows();j++)
        {
            double a=dataToConvert(i,j);
            vec->append(a);
        }
        res.append(vec);
    }
    return res;
}

real_2d_array fonctionsUtiles::GenerateMatrix(int ncol, int nrow,int nbMax)
{
    real_2d_array res;
    double a[ncol*nrow];
    for(int i=0;i<ncol*nrow;i++)
    {
        a[i]=randominteger(nbMax);
    }
    res.setcontent(nrow,ncol,a);
    return res;
}
real_1d_array fonctionsUtiles::generateVector(int nb, int nbmax)
{
    real_1d_array res;
    double a[nb];
    for(int i=0;i<nb;i++)
    {
        a[i]=randominteger(nbmax);
    }
    res.setcontent(nb,a);
    return res;
}

real_2d_array fonctionsUtiles::TransposeMatrix(real_2d_array vectors)
{
    real_2d_array vectorsTranspose;
    vectorsTranspose.setlength(vectors.cols(),vectors.rows());
    rmatrixtranspose(vectors.rows(),vectors.cols(),vectors,0,0,vectorsTranspose,0,0);
    return vectorsTranspose;
}

real_2d_array fonctionsUtiles::BuildMatrixDiag(real_1d_array diag)
{
    real_2d_array matrixDiag;
    matrixDiag.setlength(diag.length(),diag.length());
    for(int i=0;i<diag.length();i++)
    {
        for(int j=0;j<diag.length();j++)
        {
            matrixDiag(i,j)=0;
        }
        matrixDiag(i,i)=diag[i];
    }
    return matrixDiag;
}

real_2d_array fonctionsUtiles::ComputeWhiteningMatrix(real_2d_array RealData)
{
    real_2d_array whiteningMatrix;


}

int fonctionsUtiles::max(int a, int b)
{
    if(a<b)
        return b;
    else
        return a;
}

int fonctionsUtiles::min(int a, int b)
{
    if(a<b)
        return a;
    else
        return b;
}

void fonctionsUtiles::MakeSet(QVector<int> *vector,int nbFrameB,int nbframeA)
{
    //add points that make part of spike
    int size=vector->count();
    for(int i=0;i<size;i++)
    {
        int PointC=vector->operator [](i);
        for(int a=PointC-nbFrameB;a<PointC+nbframeA;a++)
        {
            vector->append(a);
        }
    }
    //remove Doubles
    QVector<int> memory;
    int PointCourant=0;
    for(int i=0;i<vector->count();i++)
    {
        PointCourant=vector->at(i);
        int k=0;
        bool find=false;
        while(k<memory.count()&&!find)
        {
            if(memory[k]==PointCourant)
            {
                vector->remove(i);
                i--;
                find=true;
            }
            k++;
        }
        if(!find)
        {
            memory.append(PointCourant);
        }
    }
}

QList<QVector<double> *> fonctionsUtiles::InitialiserQlist(int size)
{
    QList<QVector<double> *> res;
    for(int i=0;i<size;i++)
    {
        res.append(new QVector<double>);
    }
    return res;
}

int fonctionsUtiles::getIndexMax(QVector<double> vector)
{
    int index=0;
    for(int i=0;i<vector.count();i++)
    {
        if(vector[i]>vector[index])
        {
            index=i;
        }
    }
    return index;
}
