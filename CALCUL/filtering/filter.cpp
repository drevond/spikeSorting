#include "filter.h"

filter::filter()
{
    QString text;
    QDir currentDir = QDir::current();
    currentDir.cdUp();
    currentDir.cd("spikeSortingCode/CALCUL/filtering");
    QString filepath=currentDir.path();
    file=new QFile();
    file->setFileName(filepath+"/filtres.txt");
    QString a=file->fileName();
    if(!file->open(QIODevice::ReadOnly))
        return;
    flux=new QTextStream(file);
    flux->readLine();

    //determine le filtre
    char local1='a';
    while(local1!='\n')
    {
        char local2='a';
        while(local2!='\t'&&local2!='\n')
        {
            flux->operator >>(local2);
            text.append(local2);
        }

        filtre.append(text.toDouble());
        text="";
        local1=local2;
    }
    filtre.remove(filtre.count()-1);
    file->close();
}

QVector<double> filter::getfilter()
{
    return filtre;
}

QVector<double> filter::filterData(QVector<double> DataIn)
{
    QVector<double> resultat(DataIn.count(),0);
//convolution
    for(int n=0;n<DataIn.count();n++)
    {
        int k=0;
        while(n-k>=0&&k<filtre.count())
        {
            resultat[n]+=filtre[k]*DataIn[n-k];
            k++;
        }
    }
    return resultat;
}

void filter::SetFilter(int nbfiltre)
{
    QString text;
    QFile *file1=new QFile("C:/Users/ddrevon/Desktop/filtering/filtres.txt");
    file1->open(QIODevice::ReadOnly);
    QTextStream *flux1=new QTextStream(file1);
    for(int i=0;i<nbfiltre*2;i++)
        flux1->readLine();
    flux1->readLine();
    filtre.clear();
    //determine le filtre
    char local1='a';
    while(local1!='\n')
    {
        char local2='a';
        while(local2!='\t'&&local2!='\n')
        {
            flux1->operator >>(local2);
            text.append(local2);
        }
        filtre.append(text.toDouble());
        text="";
        local1=local2;
    }
    filtre.remove(-1);
    file->close();
}
