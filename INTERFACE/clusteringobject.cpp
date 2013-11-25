#include "clusteringobject.h"

ClusteringObject::ClusteringObject()
{
    SizeSpike=13;
    nbChannels=12;
}

void ClusteringObject::RunClustering()
{
    qsrand(QTime::currentTime().msec());
    int nbClusters=qrand()%10 +1;
    listClustersData.clear();
    buildClusters(nbClusters);
    for(int i=0;i<Spikes.count();i++)//distribution au hasard
    {
        listClustersData[qrand()%nbClusters]->append(Spikes[i]);
    }
    emit emitNbClusters(nbClusters);//update TableListClustersSelected
    Spikes.clear();
}

void ClusteringObject::SlotLoadSpike(ListSpikes NewSpikes)
{
    Spikes.append(NewSpikes);
}

void ClusteringObject::buildClusters(int nbClusters)
{
    for(int i=0;i<nbClusters;i++)
    {
        listClustersData.append(new ListSpikes);
    }
}


