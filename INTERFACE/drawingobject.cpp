#include "drawingobject.h"

drawingObject::drawingObject()
{
    //initialise
    for(int i=0;i<256;i++)
    {
        channelsDraw[i]=false;
    }
    channelsDraw[0]=true;
    FramesToPlot=1000;
    qplot=new myQPlot();
    connect(this,SIGNAL(emitPlotChannel()),qplot,SLOT(slotUpdatePlots()));
}
//------PROCESS-------
void drawingObject::drawThread()
{
    //synchronisation des threads reading et drawing
    forever{

    }

}

void drawingObject::drawData(const myArray dataToDraw)
{
    fillDataToPlot(dataToDraw);
    emit emitPlotChannel();
}
void drawingObject::fillDataToPlot(const myArray dataToDraw)
{
    int channelNb=0;
    //clear data
    qplot->DataChannelToPlot.clear();
    for(int i=0;i<256;i++)
    {
        if(channelsDraw[i])
        {
            qplot->DataChannelToPlot.append(new QVector<double>(FramesToPlot));
            for (int j=0; j<FramesToPlot; j++)
            {
                //conversion
                double current=(dataToDraw.at(j*256+i)-32767)*0.01;
                qplot->DataChannelToPlot[channelNb]->operator [](j)=current;
            }
            channelNb++;
        }
    }
}
