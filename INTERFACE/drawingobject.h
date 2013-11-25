#ifndef DRAWINGOBJECT_H
#define DRAWINGOBJECT_H
#include <QtWidgets>
#include "myqplot.h"

class drawingObject:public QObject
{
    Q_OBJECT
    typedef QList<ushort> myArray;

private:
    bool channelsDraw[256];
    int FramesToPlot;

public :
    drawingObject();
    myQPlot *qplot;
    void fillDataToPlot(const myArray dataToDraw);
public slots:
    void drawThread();
    void drawData(const myArray dataToDraw);
signals:
    void emitPlotChannel();
};

#endif // DRAWINGOBJECT_H
