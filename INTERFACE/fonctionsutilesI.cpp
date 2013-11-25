#include "fonctionsutilesI.h"

FonctionsUtilesI::FonctionsUtilesI()
{
}
void  FonctionsUtilesI::fillItems(int number,QTableWidget *listInit,QList<int> listIndex)
{
    int *nrowcol;
    nrowcol=findNbRowCol(number);
    int nrow,ncol;
    nrow=nrowcol[0];ncol=nrowcol[1];
    //remove former elements
    //fill elements
    int element=0;
    listInit->clear();
    listInit->setRowCount(nrow);
    listInit->setColumnCount(ncol);
    //set items
    int i=0,j=0,k=0;
    while(j<ncol)
    {
        i=0;
        while(i<nrow&&element<number)
        {
            listInit->setItem(i,j,new QTableWidgetItem("cluster " + QString::number(element)));
            if(k<listIndex.count() && listIndex[k]==element)
            {
                int a=listInit->columnCount();
                listInit->item(i,j)->setSelected(true);
                k++;
            }
            i++;
            element++;
        }
        j++;
    }
}

int* FonctionsUtilesI::findNbRowCol(int number)
{
    int res[2];
    int nrow=1;
    int ncol=1;
    int u=nrow*nrow;//on cherche raw comme un carré parfait

    while(u<number)
    {
        nrow++;
        u=nrow*nrow;
    }
    int test=(nrow-1)*nrow-number;
    int test2=nrow*nrow-number;
    if(test>0&&test2-test>0)
    {
        ncol=nrow-1;
    }
    else
    {
        ncol=nrow;
    }
    res[0]=nrow;
    res[1]=ncol;
    return res;
}

int FonctionsUtilesI::max(int a, int b)
{
    return a;
}
