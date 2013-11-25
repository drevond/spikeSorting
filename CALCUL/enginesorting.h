#ifndef ENGINESORTING_H
#define ENGINESORTING_H

#include "readdata.h"
#include "filtering/filter.h"
#include "fonctionsutiles.h"
#include <math.h>
#include "test.h"

using namespace alglib;
class engineSorting:public QObject
{
    Q_OBJECT
    struct spike
    {
        int frameSpike;
        int ChannelMax;
        QTime timeSpike;
    };
    typedef QVector<double> vector;
    typedef QList<spike> cluster;
    typedef QList<cluster *> listClusters;

public:
    engineSorting(QFile *FileToRead,int nbchannelsW=10, int nbchannelstot=256);
    void TimeOfComputation(QTime time, int durationMs);

public slots:
    void slotBeginClusterization(int timeMn, int durationS);
    void slotFilterData(int timeBegining,int duration,int nbFilter);
    void slotGetSpike(QTime time, int nbframeSpikeAfter, int nbframeSpikeBefore, int channel, int Frame);

private:
    real_2d_array a;
    ReadData *readDataRaw;
    ReadData *readDataDenoised;
    int beginSpike;
    int test;
    filter filtre;
    QFile *fileDataDenoised;
    QFile *fileSpikes;
    QFile *fileSpikesChannel;
    QDataStream *fluxOutSpike;
    QDataStream *fluxOutSpikeChannel;
    int timeBegin;
    //parameters
    int pth;
    int nbFrameSpikeB;
    int nbFrameSpikeA;
    int nbChannelsTot;
    int nbChannelsW;
    int firstChannel;
    double threshold;
    //data
    QList<QVector<double> *> channelsData;
    QVector<double> dataFiltered;
    QList<cluster *> ListClusters;
    QList<QVector<double> *> whiteningMatrix;

    //----------------FONCTIONS
    //denoise data
    QVector<int> findSpikesForDenoising(QVector<double> DataChannel, int threshold);
    real_2d_array BuildWhiteningMatrix(int timeSecToBuild);
    void DenoiseData(int timeBeginS, int durationS);
    //clusterization
    void findSpikesForPreClustering(int threshold,QTime Time);
    void buildPreclusters(QTime time);
    void clusterization(int algo);
signals:
    void emitClusters(listClusters ClustersList);
    void emitMatrix(real_2d_array matrix,int nbcol,int nbrow,QString name);
    void EmitEndOfDenoising();
    void emitSpikeChannel(vector SpikeChannel);

};

#endif // ENGINESORTING_H
