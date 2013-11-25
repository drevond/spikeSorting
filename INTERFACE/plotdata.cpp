#include "plotdata.h"

//constructor
plotData::plotData()
{
    layoutOfChannel=new QGridLayout;
    //graphic parameters
    initParameters();

    //initialise channel actif
    initChannelsVisible();
    //initialise les channel plot
    initChannels();

    //widgets et layouts
    QVBoxLayout *MainLayout=new QVBoxLayout;

    QHBoxLayout *layoutParameters=new QHBoxLayout;
    layoutParameters->addWidget(cbAmpWindow);
    layoutParameters->addWidget(cbTimeWindow);
    layoutParameters->addWidget(btChangeTime);
    layoutParameters->addWidget(labNextOccurence);
    layoutParameters->addWidget(spNextOccurence);
    layoutParameters->addWidget(spSpeed);

    MainLayout->addLayout(layoutParameters);
    MainLayout->addLayout(layoutOfChannel);
    MainLayout->addWidget(TimeScrollbar);


    setLayout(MainLayout);
    //----------------------------------CONNECTIONS-----------------------
    connect(TimeScrollbar,SIGNAL(sliderReleased()),this,SLOT(modifyTimeCursor()));
    connect(cbAmpWindow,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeAmp(int)));
    connect(cbTimeWindow,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeTime(int)));

    connect(btChangeTime,SIGNAL(clicked()),this,SLOT(slotModifyTimeView()));
    connect(spSpeed,SIGNAL(valueChanged(double)),this,SLOT(slotChangeSpeed()));
}

//functions
void plotData::modifyTimeCursor()
{
    currentTime=firstTime+(TimeScrollbar->value())*deltaCursorTime;
    UpdatePlots();
}

void plotData::setTimeWindow(int time)
{
    TimeScrollbar->setMinimum(time);
    timeWindow=time;
}

//change parameters
void plotData::slotChangeAmp(int indexSelected)
{
    switch(indexSelected)
    {
    case 0 : windowAmp=50;
        break;
    case 1 : windowAmp=100;
        break;
    case 2 : windowAmp=200;
        break;
    }
    UpdatePlots();
}

void plotData::slotChangeTime(int indexSelected)
{
    switch(indexSelected)
    {
    case 0 :
        setTimeWindow(5);
        break;
    case 1 :
        setTimeWindow(25);
        break;
    case 2 :
        setTimeWindow(50);
        break;
    case 3 :
        setTimeWindow(100);
        break;
    case 4 :
        setTimeWindow(200);
        break;
   }
}

void plotData::slotModifyTimeView()
{
    QString textTime=QInputDialog::getText(this,
                                           "change Time cursor",
                                           "enter a valid time in milliseconds",QLineEdit::Normal);
    int Time=textTime.toInt(0,10);
    currentTime=Time;
    lastTime=currentTime;
    firstTime=currentTime;
    removePoints=recordTime/timeBetweenFrames+1;
    //remove formers data
    for(int i=0;i<listChannelsPlot.count();i++)
    {
        listChannelsPlot[i]->graph(0)->clearData();
    }
    emitModifyTime(Time);
}

void plotData::slotChangeSpeed()
{
    if(spSpeed->value()>0.05)
        emitChangeSpeed(spSpeed->value());
    else
        spSpeed->setValue(0.05);
}

//general
void plotData::SlotFillGraph(const myArray dataToPlot)
{
    emitEnableReading();//disable readingData while treating first ones.
    //refill graphs
     int nbNewPoints=dataToPlot.count()/nbChannelTot;
    for(int i=0;i<nbNewPoints;i++)
    {
        for (int j=0; j<nbChannelTot; j++)
        {
            //conversion
            double current=(dataToPlot.at(i*nbChannelTot+j)-32767)*0.142;
            //remove points
            if(removePoints==1)
                listChannelsPlot[j]->graph(0)->removeData(firstTime-0.0001,firstTime+0.0001);
            //add points
            listChannelsPlot[j]->graph(0)->addData(lastTime,current);
        }
        lastTime+=0.05;
        currentTime+=0.05;
        //update time scrollbar
        if(lastTime-firstTime<recordTime)
        {
            TimeScrollbar->setMinimum(timeWindow);
            TimeScrollbar->setMaximum((lastTime-firstTime)/deltaCursorTime-1);
            TimeScrollbar->setValue(TimeScrollbar->maximum());
        }
        //for removing points
        if(removePoints==1)
            firstTime+=0.05;
    }

    //reAdjust parameters
    UpdatePlots();
    if(removePoints!=1)
    {
        removePoints--;
    }
    emitEnableReading();//reEnable readingData
}

void plotData::UpdatePlots()
{
    //fix axis and replot.
    for(int nbChannel=0;nbChannel<nbChannelTot;nbChannel++)
    {
        //axis
        if(currentTime-timeWindow<firstTime)//for the begining
        {
            listChannelsPlot[nbChannel]->xAxis->setRange(firstTime,firstTime+timeWindow);
            listChannelsPlot[nbChannel]->yAxis->setRange(-windowAmp, windowAmp);
        }
        else
        {
            double time1=currentTime-timeWindow,time2=currentTime;
            listChannelsPlot[nbChannel]->xAxis->setRange(time1,time2);
            listChannelsPlot[nbChannel]->yAxis->setRange(-windowAmp, windowAmp);
        }
        if(channels[nbChannel])
        {
            //replot
            listChannelsPlot[nbChannel]->replot();
        }
    }
}

//convenients
int plotData::getTimeWindow()
{
    return timeWindow;
}

void plotData::initChannels()
{
    currentTime=0.0;
    int i=0,j=0,channel=0;
    while(j<ncol)
    {
        i=0;
        while(i<nrow&&channel<nbChannelTot)
        {
            listChannelsPlot.append((new QCustomPlot));
            listChannelsPlot[channel]->addGraph();
            listChannelsPlot[channel]->setMaximumHeight(45);
            // give the axes some labels:
            listChannelsPlot[channel]->xAxis->setVisible(false);
            listChannelsPlot[channel]->yAxis->setVisible(false);
            //add title
            //listChannelsPlot[channel]->plotLayout()->insertRow(0);
            QCPPlotTitle *title=new QCPPlotTitle(listChannelsPlot[channel],QString::number(channel));
            QFont serifFont("Times", 8);
            title->setFont(serifFont);
            listChannelsPlot[channel]->plotLayout()->insertRow(0);
            listChannelsPlot[channel]->plotLayout()->addElement(0, 1, title);
            //add to layout.
            layoutOfChannel->addWidget(listChannelsPlot[channel],i,j);
            //interactions
            //listChannelsPlot[channel]->setInteraction(QCP::iRangeDrag);
            i++;
            if(!channels[channel])
            {
                listChannelsPlot[channel]->setVisible(false);
            }
            channel++;
        }
        j++;
    }
}

void plotData::findNbRowCol(int number)
{
    nrow=1;
    ncol=1;
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
}

void plotData::initChannelsVisible()
{
    qsrand(QTime::currentTime().msec());
    findNbRowCol(nbChannelTot);
    for(int i=0;i<nbChannelTot;i++)
    {
        channels[i]=true;
    }
    /*int RowIndex=qrand()%(nrow-7)+3;
    int colIndex=qrand()%(ncol-7)+3;
    int k=0,i=0,j=0;
    while(i<5)
    {
        i++;j=0;
        while(j<5)
        {
            k=(RowIndex+i)*ncol+(colIndex+j);
            channels[k]=true;
            j++;
        }
    }*/
}

void plotData::initParameters()
{
    //data
    timeBetweenFrames=5;//milliseconds=>20frames per second
    SamplingRate=20;//khz
    nbChannelTot=256;
    //initialise default parameters
    windowAmp=50;
    timeWindow=5;
    recordTime=200;
    currentTime=0.0;
    firstTime=0.0;
    lastTime=0.0;
    removePoints=recordTime/timeBetweenFrames+1;
    //scroolBar
    deltaCursorTime=1;
    TimeScrollbar=new QScrollBar(Qt::Horizontal,this);
    TimeScrollbar->setMinimum(0);
    TimeScrollbar->setMaximum(0);
    //comboBoxes
    cbTimeWindow=new QComboBox;
    cbTimeWindow->addItem("5 ms");
    cbTimeWindow->addItem("25 ms");
    cbTimeWindow->addItem("50 ms");
    cbTimeWindow->addItem("100 ms");
    cbTimeWindow->addItem("200 ms");
    cbAmpWindow=new QComboBox;
    cbAmpWindow->addItem("50 µV");
    cbAmpWindow->addItem("100 µV");
    cbAmpWindow->addItem("200 µV");
    //SpinBoxes
    btChangeTime=new QPushButton("changeTimeValueBegining");
    labNextOccurence=new QLabel("Next Spike Occurence ");
    spNextOccurence=new QSpinBox;
    spSpeed=new QDoubleSpinBox;
    spSpeed->setValue(1);
    spSpeed->setSingleStep(0.1);
    emitChangeSpeed(spSpeed->value());

}

int plotData::getRecordTime()
{
    return recordTime;
}
