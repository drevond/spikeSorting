#include "clusteringcontroller.h"

ClusteringController::ClusteringController(QObject *parent) :
    QObject(parent)
{
    runCluster= new ClusteringObject;
    runCluster->moveToThread(&threadCluster);

    //------------connections---------------------
    connect(&threadCluster, &QThread::finished, runCluster, &QObject::deleteLater);
    connect(this,SIGNAL(operate()), runCluster,SLOT(RunClustering()));
    qRegisterMetaType<ListSpikes>("ListSpikes");
    connect(this,SIGNAL(EmitLoadSpikes(ListSpikes)),runCluster,SLOT(SlotLoadSpike(ListSpikes)));
    //connect(runCluster, SIGNAL(resultReady()), this, SLOT(handleResults(QString)));
    connect(runCluster,SIGNAL(emitNbClusters(int)),this,SIGNAL(emitNbClusters(int)));

    threadCluster.start();
}

ClusteringController::~ClusteringController()
{
    threadCluster.quit();
    threadCluster.wait();
}

void ClusteringController::handleResults(const QString &)
{

}

void ClusteringController::treatClusterRequest(int cluster)
{
    //ListSpikes ClusterToSend;
    //ClusterToSend.append((
    emitSpikesOfCluster(*runCluster->listClustersData[cluster]);
}
