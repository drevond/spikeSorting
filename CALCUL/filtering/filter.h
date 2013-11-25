#ifndef FILTER_H
#define FILTER_H

#include <QtWidgets>

class filter
{
public:
    filter();
    QVector<double> getfilter();
    QVector<double> filterData(QVector<double> DataIn);

private:
    QFile *file;
    QTextStream *flux;
    QVector<double> filtre;
public:
    void SetFilter(int nbfiltre);

};

#endif // FILTER_H
