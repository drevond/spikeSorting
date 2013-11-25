#ifndef CLUSTERINGOBJECT_H
#define CLUSTERINGOBJECT_H

#include<QtWidgets>

class ClusteringObject:public QObject
{
    Q_OBJECT
    QThread threadCluster;
    typedef QList<QVector<double> *> ListSpikes;

public:
    ClusteringObject();
    QList<QList<QVector<double> *> *> listClustersData;

private:
    int nbChannels;
    int SizeSpike;
    ListSpikes Spikes;

    void buildClusters(int nbClusters);

public slots:
    void RunClustering();
    void SlotLoadSpike(ListSpikes NewSpikes);

signals:
    void emitNbClusters(int nbClusters);//to update the list of clustersSelected
};
#endif //CLUSTERINGOBJECT
