#ifndef PLOTCLUSTER_H
#define PLOTCLUSTER_H
#include "qcustomplot.h"
#include <QtWidgets>

class PlotCluster : public QWidget
{
    Q_OBJECT
    typedef QList<ushort> myArray;
    typedef QList<QVector<double> *> ListSpikes;
    typedef QVector<double> vector;
    struct spike
    {
        int frameSpike;
        int ChannelMax;
        QTime timeSpike;
    };
    struct spikeData
    {
        QList<QVector<double> *> spikesChannels;
    };
    typedef QList<spikeData> ClusterData;
    typedef QList<spike> cluster;
    typedef QList<cluster *> listClusters;

public:
    PlotCluster(int nbOfChannels=256);
    int nbChannels;
    QPushButton *btUpdateView;
    QList<cluster *> listClustersToDraw;
    QPushButton *btRunClustering;
    int durationClusteringS;
    int timeBeginingClustering;
    QLabel *labDuration;
    QLabel *labTime;

    QStringList colors;

    //size spike
    int nbframeSpike;
    int nbframeSpikeB;
    int nbframeSpikeA;


private:
    QTableWidget *ListChannelToSelect;
    QList<int> listChannelSelected;
    //toolbox
    QGroupBox *gbParameters;
    QComboBox *cbFeature1;
    QComboBox *cbFeature2;
    QCheckBox *chbPreClustering;
    QPushButton *btMerge;
    QTableWidget *ListClustersToSelect;
    QList<int> listIndexClustersSelected;
    QGridLayout *examplesLayout;
    QGridLayout *mainLayout;
    QGridLayout *ToolLayout;
    //parameters
    QPushButton *btTimeClustering;
    QPushButton *btDurationClustering;

    //general
    int nrow;
    int ncol;
    int element;
    void fillItems(int number);

    //for features
    QCustomPlot *plotFeature;
    void plotFeatures();

    //for examples
    QVector<double> timeVector;
    QList<QCustomPlot *> listClutersExamplesPlot;
    void fillLayoutOfExemples();
    ClusterData buildSpikes(int clusterNb);
    QVector<double> SpikeChannelIncoming;
    QWaitCondition Pause;

    //utiles
    void buildToolLayout();
    void InitExamples();
    void initFeatureSpace();
    double computeMax(QVector<double> list);
    QList<QVector<double> *> convertSpikeToSpikeChannel(QVector<double> *spike);
    void findIndexClustersSelected();
    void findNbRowCol(int number);
    void findChannelsSelected();

public slots:
    void SlotUpdateView();
    void SlotNbClusters(int nbclusters);
    void slotLoadClusters(listClusters ListClusters);
    void slotChangeTimeClustering();
    void slotChangeDurationClustering();
    void slotReceiveSpikeChannel(vector SpikeChannel);
    //former  functions
    /*double computeMaxSpikeOnChannel(int feature, int cluster, int spikes);
    QVector<double> FeatureSpikesCluster(int feature,int cluster);
    void ReceiveSpike(ListSpikes Cluster);
*/

signals:
    void emitClusterSelected(int indexCluster);
    void emitGetSpikeChannel(QTime time,int nbframeSpikeAfter,int nbframeSpikeBefore,int channel,int Frame);
};

#endif // PLOTCLUSTER_H
