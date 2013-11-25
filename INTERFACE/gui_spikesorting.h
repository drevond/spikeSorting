#ifndef GUI_SPIKESORTING_H
#define GUI_SPIKESORTING_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "readingobject.h"
#include "plotdata.h"
#include "plotcluster.h"
#include "clusteringcontroller.h"
#include "plotfitting.h"
#include "staticgraph.h"
#include "tabhandlefiltering.h"

namespace Ui {
class GUI_SpikeSorting;
}

class GUI_SpikeSorting : public QMainWindow
{
    Q_OBJECT
    //definitions
    typedef QVector<double> vector;
    struct spike
    {
        int frameSpike;
        int ChannelMax;
        QTime timeSpike;
    };
    typedef QList<ushort> myArray;
    typedef QList<QVector<double> *> ListSpikes;
    typedef QList<spike> cluster;
    typedef QList<cluster *> listClusters;
    

public:
    explicit GUI_SpikeSorting(QString fileToRead,QWidget *parent = 0,int NbOfChannels=20);
    ~GUI_SpikeSorting();


private:
    Ui::GUI_SpikeSorting *ui;
    //plot data
    int nbChannels;
    int nbChannelsPlot;
    int nbFramesPlot;
    int amplitudeWindow;
    plotData *PlotData;
    StaticGraph *staticGraph;
    tabHandleFiltering *TabHandleFiltering;
    QScrollArea *scrollarea;


    //threads
    readingObject *ReadingTh;
    QThread *thReading;
    bool docOpened;
    //clusters
    PlotCluster *plotCluster;
    ClusteringController *clustering;
    QPushButton *btRunClusteringRand;
    //fiting data
    plotFitting *PlotFittingData;
    //parameters
    //fonctions utiles
    void setIcon();
    void lireHeader();
    void FillTabs(QString filetoread);
    void fillLayout();
    void makeConnections();

    //signals
signals:
    void emitClearNewSpikes();
    void emitBeginComputations(int time,int durationS);
    void emitLoadClusters(listClusters listClusters);
    void emitChangeFilter(int nbfilter);
    void emitEndofDenoising();
    void EmitGuiApplyFilter(int beginingTime,int duration,int nbFilter);
    void emitGuiGetdata(QTime time,int nbframeSpikeAfter,int nbframeSpikeBefore,int channel,int Frame);
    void emitSpikeChannelReceived(vector SpikeChannel);

private slots:
    void OuvrirDocument();
    void PauseReading();
    void stopReading();
    void slotStart();
    //replayer
    void slotChangeSizeToSend(int sizeToSend);
    void slotModifyTimeCursor(int Time);
    void slotEnableIncomingData();
    void slotChangeSpeed(double speed);
    //clustering
    void slotUpdateClusterView();
    void slotRunClustering();
};


#endif // GUI_SPIKESORTING_H
