#ifndef PLOTDATA_H
#define PLOTDATA_H
#include "qcustomplot.h"
#include <QtWidgets>

class plotData: public QWidget
{
    Q_OBJECT
    typedef QList<ushort> myArray;

public:
    plotData();
    int nbChannelTot;
    int SamplingRate;//khz
    int timeBetweenFrames;
    void setTimeWindow(int time);
    int getTimeWindow();
    int getRecordTime();
private:
    //widgets
    QScrollBar *TimeScrollbar;
    QComboBox *cbTimeWindow;
    QComboBox *cbAmpWindow;
    QPushButton *btChangeTime;
    QSpinBox *spNextOccurence;
    QLabel *labNextOccurence;
    QDoubleSpinBox *spSpeed;

    //parameters
    double windowAmp;
    int timeWindow;
    int deltaCursorTime;
    double firstTime;
    double lastTime;//must be connected to the length of the file we are reading in.
    double currentTime;
    int removePoints;
    int recordTime;
    bool channels[256];

    QGridLayout *layoutOfChannel;
    QList<QCustomPlot *> listChannelsPlot;


    void UpdatePlots();

    //convenients
    void initParameters();
    void initChannelsVisible();
    void initChannels();
    void findNbRowCol(int number);
    void setCurrentTime(int value);
    int nrow;
    int ncol;


public slots:
    void SlotFillGraph(const myArray dataToPlot);
    //change parameters
    void slotChangeAmp(int indexSelected);
    void slotChangeTime(int indexSelected);
    void modifyTimeCursor();
    void slotModifyTimeView();
    void slotChangeSpeed();
signals:
    void emitModifyTime(int Time);
    void emitEnableReading();
    void emitSizetoSend(int TimeBetweenFrames);
    void emitChangeSpeed(double Speed);
};

#endif // PLOTDATA_H
