#include "test.h"

test::test()
{
    showMatrix=new QTextEdit;
}

void test::seeMatrix(real_2d_array matrix, int nColSee, int NrowSee)
{
    showMatrix->append("values:");
    for(int i=0;i<nColSee;i++)
    {
        showMatrix->append("\n ");
        QString text;
        for(int j=0;j<NrowSee;j++)
        {
            text.append("\t" + QString::number(matrix(i,j)));
        }
        showMatrix->append(text);
    }
}

void test::startTest()
{
    showMatrix->show();
    //matrice product
    showMatrix->append("test matrice product");
    real_2d_array a=fonctionsUtiles::GenerateMatrix(2,2,5);
    seeMatrix(a,2,2);
    real_2d_array b=fonctionsUtiles::GenerateMatrix(2,2,5);
    seeMatrix(b,2,2);
    real_2d_array c=fonctionsUtiles::MatriceProduct(a,b);
    seeMatrix(c,2,2);
    //transpose
    showMatrix->append("test matrixtranspose");
    a=fonctionsUtiles::GenerateMatrix(2,2,10);
    b=fonctionsUtiles::TransposeMatrix(a);
    seeMatrix(a,2,2);
    seeMatrix(b,2,2);
    //matrixDiag
    real_1d_array vector=fonctionsUtiles::generateVector(5,10);
    a=fonctionsUtiles::BuildMatrixDiag(vector);
    seeMatrix(a,5,5);
    //whitening matrix
}

void test::slotPlot(real_2d_array matrix,int nbrow,int nbcol,QString name)
{
    showMatrix->append(name);
    seeMatrix(matrix,nbcol,nbrow);
}
