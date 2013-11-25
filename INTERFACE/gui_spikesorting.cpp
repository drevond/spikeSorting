#include "gui_spikesorting.h"
#include "ui_gui_spikesorting.h"
#include <QtWidgets>
#include "qcustomplot.h"
#include "fonctionsutilesI.h"
#define test;

//---CONSTRUCTEUR ---
GUI_SpikeSorting::GUI_SpikeSorting(QString fileToRead, QWidget *parent, int NbOfChannels) :
    QMainWindow(parent),
    ui(new Ui::GUI_SpikeSorting)
{
    ui->setupUi(this);

    setIcon();
    ui->actionPause->setEnabled(false);
    resize(640,480);
    docOpened=false;
    //datafile
    nbChannels=256;
    nbChannelsPlot=NbOfChannels;
    nbFramesPlot=1000;
    amplitudeWindow=1;
    //----------------TABS-------------------
    FillTabs(fileToRead);
    docOpened=true;

    //---------------CONNECTIONS----------------------
    makeConnections();

    //----------------LAYOUTS---------------
    fillLayout();

#ifdef test
#endif
}

//---DESTRUCTEUR---
GUI_SpikeSorting::~GUI_SpikeSorting()
{
    delete ui;
}

void GUI_SpikeSorting::OuvrirDocument()
{
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,"Ouvrir fichier binaire");
    ReadingTh->file=new QFile(fileName);
    //ouverture de fichier en lecture seule
    if(!ReadingTh->file->open(QIODevice::ReadOnly))
        return;
    ReadingTh->DataFile->setDevice(ReadingTh->file);//fill the qdatastream
    lireHeader();
    docOpened=true;
}

//methods

//tab raw data
void GUI_SpikeSorting::slotStart()
{
    if(docOpened==true&&!thReading->isRunning())
    {
        PlotData->emitSizetoSend(PlotData->timeBetweenFrames);
        ui->actionPlay->setEnabled(false);
        ui->actionPause->setEnabled(true);
        thReading->start();
    }
}

void GUI_SpikeSorting::lireHeader()
{
    quint8 charRead;
    QString header;
    QString endOfHeader="abcde";
    int sizeHeader=0;
    while(endOfHeader!="EOH\r\n")
    {
        sizeHeader++;
        ReadingTh->DataFile->operator >>(charRead);
        header.append(QChar(charRead));
        endOfHeader.remove(0,1);
        endOfHeader.append(charRead);
    }
    ui->teReadFile->setText(header+"\nsize of header : " + QString::number(sizeHeader));
}

//fonctions utiles
void GUI_SpikeSorting::FillTabs(QString filetoread)
{
    //onglet cluster analysis
    plotCluster=new PlotCluster;
    clustering=new ClusteringController;

    //thread Reader
    thReading=new QThread;
    ReadingTh=new readingObject(filetoread);
    connect(thReading,SIGNAL(started()),ReadingTh,SLOT(reading()));
    ReadingTh->moveToThread(thReading);

    //widget of plots
    PlotData=new plotData;
    staticGraph= new StaticGraph;

    //Fitting Data
    PlotFittingData=new plotFitting;

    //tab filtering
    TabHandleFiltering=new tabHandleFiltering;
}

void GUI_SpikeSorting::fillLayout()
{
    QVBoxLayout *layoutRawData=new QVBoxLayout;
    layoutRawData->addWidget(PlotData);
    ui->OngletRawData->setLayout(layoutRawData);
    QVBoxLayout *layoutStaticGraph= new QVBoxLayout;
    layoutStaticGraph->addWidget(staticGraph);
    ui->OngletStaticGraph->setLayout(layoutStaticGraph);
    //layout of clusters
    QGridLayout *layoutClusters=new QGridLayout;
    layoutClusters->addWidget(plotCluster);
    ui->OngletClusterAnalysis->setLayout(layoutClusters);
    //layout fitting Data
    QGridLayout *layoutFittingData=new QGridLayout;
    layoutFittingData->addWidget(PlotFittingData);
    ui->OngletFittingData->setLayout(layoutFittingData);
    //layout filtering
    QGridLayout *layoutFiltering=new QGridLayout;
    layoutFiltering->addWidget(TabHandleFiltering);
    ui->ongletFiltering->setLayout(layoutFiltering);
    ui->ongletFiltering->setLayout(layoutFiltering);

}

void GUI_SpikeSorting::makeConnections()
{
    connect(ui->actionOuvrir,SIGNAL(triggered()),this,SLOT(OuvrirDocument()));
    //start reading
    connect(ui->actionPlay,SIGNAL(triggered()),this,SLOT(slotStart()));
    connect(ui->actionPause,SIGNAL(triggered()),this,SLOT(stopReading()));
    qRegisterMetaType<myArray>("myArray");

    //connetions raw data replayer
    connect(ReadingTh,SIGNAL(EmitSizeBufferReady(myArray)),PlotData,SLOT(SlotFillGraph(myArray)),Qt::QueuedConnection);
    connect(PlotData,SIGNAL(emitModifyTime(int)),this,SLOT(slotModifyTimeCursor(int)));
    connect(PlotData,SIGNAL(emitSizetoSend(int)),this,SLOT(slotChangeSizeToSend(int)));
    connect(PlotData,SIGNAL(emitEnableReading()),this,SLOT(slotEnableIncomingData()),Qt::DirectConnection);
    connect(PlotData,SIGNAL(emitChangeSpeed(double)),this,SLOT(slotChangeSpeed(double)));

    //connections for cluster
    connect(clustering,SIGNAL(emitNbClusters(int)),plotCluster,SLOT(SlotNbClusters(int)));
    connect(plotCluster,SIGNAL(emitClusterSelected(int)),clustering,SLOT(treatClusterRequest(int)),Qt::DirectConnection);
    qRegisterMetaType<ListSpikes>("ListSpikes");
    connect(plotCluster->btRunClustering,SIGNAL(clicked()),this,SLOT(slotRunClustering()));
    qRegisterMetaType<listClusters>("listClusters");
    connect(this,SIGNAL(emitLoadClusters(listClusters)),plotCluster,SLOT(slotLoadClusters(listClusters)));
    connect(plotCluster,SIGNAL(emitGetSpikeChannel(QTime,int,int,int,int)),this,SIGNAL(emitGuiGetdata(QTime,int,int,int,int)),Qt::DirectConnection);
    qRegisterMetaType<vector >("vector");
    connect(this,SIGNAL(emitSpikeChannelReceived(vector)),plotCluster,SLOT(slotReceiveSpikeChannel(vector)),Qt::DirectConnection);


    //tab filtering connections
    connect(this,SIGNAL(emitEndofDenoising()),TabHandleFiltering,SLOT(slotFinishFiltering()));
    connect(TabHandleFiltering,SIGNAL(EmitApplyFilter(int,int,int)),this,SIGNAL(EmitGuiApplyFilter(int,int,int)));
}

void GUI_SpikeSorting::setIcon()
{
    QStyle* style1 =  QApplication::style();
    QIcon iconPlay= style1->standardIcon(QStyle::SP_MediaPlay);
    ui->actionPlay->setIcon(iconPlay);

    QStyle* style2 =  QApplication::style();
    QIcon iconPause= style2->standardIcon(QStyle::SP_MediaPause);
    ui->actionPause->setIcon(iconPause);

    QStyle* style3 =  QApplication::style();
    QIcon iconOpen= style3->standardIcon(QStyle::SP_ArrowBack);
    //On applique notre icônes à notre contrôle
    ui->actionOuvrir->setIcon(iconOpen);
}

//slots
void GUI_SpikeSorting::stopReading()
{
    if(ReadingTh->isStop==false)
    {
        ReadingTh->isStop=true;
    }
    else
    {
        ReadingTh->isStop=false;
        ReadingTh->QwaitPause.wakeAll();
    }
}

void GUI_SpikeSorting::PauseReading()
{
    if(ReadingTh->isPause==false)
    {
        ReadingTh->isPause=true;
    }
    else
    {
        ReadingTh->isPause=false;
        ReadingTh->QwaitPause.wakeAll();
    }
}

void GUI_SpikeSorting::slotChangeSizeToSend(int sizeToSend)
{
    ReadingTh->setSizeBuffer(sizeToSend);
}

void GUI_SpikeSorting::slotEnableIncomingData()
{
    PauseReading();
}

void GUI_SpikeSorting::slotModifyTimeCursor(int Time)
{
    int pos=Time*PlotData->SamplingRate*nbChannels*2;//2=size of ushort.
    ReadingTh->bufferCourant.clear();
    ReadingTh->file->seek(ReadingTh->posBegin+pos);
}

void GUI_SpikeSorting::slotChangeSpeed(double speed)
{
    ReadingTh->Speed=speed;
}

//tab cluster
void GUI_SpikeSorting::slotUpdateClusterView()
{
    plotCluster->SlotUpdateView();
}

void GUI_SpikeSorting::slotRunClustering()
{
    emitBeginComputations(plotCluster->timeBeginingClustering,plotCluster->durationClusteringS);
    plotCluster->btRunClustering->setEnabled(false);
}

