#ifndef CLUSTERINGCONTROLLER_H
#define CLUSTERINGCONTROLLER_H

#include <QObject>
#include "clusteringobject.h"
class ClusteringController : public QObject
{
    Q_OBJECT
    typedef QList<QVector<double> *> ListSpikes;
    QThread threadCluster;

public:
    explicit ClusteringController(QObject *parent = 0);
    ~ClusteringController();

signals:
    void operate();
    void EmitLoadSpikes(ListSpikes Spikes);
    void emitNbClusters(int nbClusters);
    void emitSpikesOfCluster(ListSpikes cluster);

public slots:
    void handleResults(const QString &);
    void treatClusterRequest(int cluster);

private:
    ClusteringObject *runCluster;
};

#endif // CLUSTERINGCONTROLLER_H
