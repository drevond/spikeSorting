#ifndef PLOTFITTINGDATA_H
#define PLOTFITTINGDATA_H

#include <QWidget>
#include "qcustomplot.h"
#include <QtWidgets>

class plotFittingData : public QWidget
{
    Q_OBJECT
private :
    QCustomPlot *PlotValuesOfAdjust;
    QGridLayout *mainLayout;
public:
    explicit plotFittingData(QWidget *parent = 0);
    
signals:
    
public slots:

};

#endif // PLOTFITTINGDATA_H
