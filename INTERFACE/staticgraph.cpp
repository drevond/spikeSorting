#include "staticgraph.h"

StaticGraph::StaticGraph()
{
    nbChannels=256;
    nbChannelsW=10;
    mainLayout=new QGridLayout;
    plot=new QCustomPlot;
    plot->addGraph();
    plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    plot->yAxis->setRange(-100,10);
    btDenoisedTrue=new QPushButton("Work Denoised");
    labDenoised=new QLabel("Denoised=false");
    TrueDenoised=false;
    spChannel=new QSpinBox;
    sptime1=new QSpinBox;
    sptime2=new QSpinBox;
    spChannel->setMaximum(nbChannels-1);
    sptime1->setMaximum(100000);
    sptime2->setMaximum(100000);
    sptime1->setValue(0);
    sptime2->setValue(1000);
    spChannel->setValue(15);
    labchannel=new QLabel("channel");
    labtime=new QLabel("time");
    btUpdate=new QPushButton("update");
    //----------LAYOUT------------
    mainLayout->addWidget(plot,0,0,5,8);
    mainLayout->addWidget(labchannel,5,1,1,1);
    mainLayout->addWidget(spChannel,5,2,1,1);
    mainLayout->addWidget(labtime,5,3,1,1);
    mainLayout->addWidget(sptime1,5,4,1,1);
    mainLayout->addWidget(sptime2,5,5,1,1);
    mainLayout->addWidget(btUpdate,5,6,1,1);
    mainLayout->addWidget(btDenoisedTrue,5,7,1,1);
    mainLayout->addWidget(labDenoised,5,8,1,1);
    setLayout(mainLayout);
    //-----------------CONNECTIONS---------------
    connect(btUpdate,SIGNAL(clicked()),this,SLOT(changePlot()));
    connect(btDenoisedTrue,SIGNAL(clicked()),this,SLOT(slotBtDenoisedTrue()));
 }

void StaticGraph::generateTimeVec()
{
    VecTime.clear();
    int time1;
    int time2;
    if(sptime1->value()<sptime2->value())
    {
        time1=sptime1->value();
        time2=sptime2->value();
    }
    else
    {
        time1=sptime2->value();
        time2=sptime1->value();
    }
    int duree=(time2-time1)*20;
    double time=time1;
    for(int i=0;i<duree;i++)
    {
        VecTime.append(time);
        time+=0.05;
    }
}

void StaticGraph::changePlot()
{
    generateTimeVec();
    ReadData();

    plot->graph(0)->setData(VecTime,VecData);
    plot->xAxis->setRange(VecTime.first(),VecTime.last());
    plot->replot();
}

void StaticGraph::ReadData()
{
    QFile fichier;
    if(TrueDenoised)
    {
        QString fileName;
        fileName=QFileDialog::getOpenFileName(this,"Ouvrir fichier binaire");
        fichier.setFileName(fileName);
    }
    else
    {
        fichier.setFileName("C:/Users/ddrevon/Desktop/Spike_Sotring_Offline_GUI(changement_structure_plotcluster)/DATA/checkerboard_20px_20c_45min.raw");
    }
    if (!fichier.open(QIODevice::ReadOnly))
        return;
    //lecture header
    QDataStream flux(&fichier);
    flux.setByteOrder(QDataStream::LittleEndian);
    //skip header
    quint8 charRead;
    QString endOfHeader="abcde";
    while(endOfHeader!="EOH\r\n")
    {
        flux.operator >>(charRead);
        endOfHeader.remove(0,1);
        endOfHeader.append(charRead);
    }
    int posEndOfHeader=fichier.pos();

    //read data
    VecData.clear();
    int time1;
    int time2;
    if(sptime1->value()<sptime2->value())
    {
        time1=sptime1->value();
        time2=sptime2->value();
    }
    else
    {
        time1=sptime2->value();
        time2=sptime1->value();
    }
    long begining=posEndOfHeader+time1*20*nbChannels*2;
    fichier.seek(begining);
    double durationMs=time2-time1;
    int nbDataPerChannel=durationMs*20;
    double currentVoltage;
    ushort currentUshort;
    if(TrueDenoised)
    {
        long i=0;
        while(i<nbDataPerChannel&&!flux.atEnd())
        {
            for(int j=0;j<nbChannels;j++)
            {
                flux.operator >>(currentUshort);
                currentVoltage=(currentUshort-32767)*0.1;
                if(j==spChannel->value())
                    VecData.append(currentVoltage);
            }
            i++;
        }
    }
    else
    {
        for(long i=0;i<nbDataPerChannel;i++)
        {
            for(int j=0;j<nbChannels;j++)
            {
                flux.operator >>(currentUshort);
                currentVoltage=(currentUshort-32767)*0.142;
                if(j==spChannel->value())
                    VecData.append(currentVoltage);
            }
        }
    }
    fichier.close();
}

void StaticGraph::slotBtDenoisedTrue()
{
    if(TrueDenoised)
    {
        labDenoised->setText("Denoised=false");
        TrueDenoised=false;

    }
    else
    {
        labDenoised->setText("Denoised=true");
        TrueDenoised=true;
    }
}









