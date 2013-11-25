#ifndef PLOTFITTING_H
#define PLOTFITTING_H
#include "INTERFACE/fonctionsutilesI.h"
#include "qcustomplot.h"
#include <QtWidgets>

class plotFitting : public QWidget
{
    Q_OBJECT
    typedef QList<ushort> myArray;
    typedef QList<QVector<double> *> ListSpikes;
    struct spike
    {
        QList<QVector<double> *> spikesChannels;
        int timeSpike;
        int ChannelMax;
    };
    typedef QList<spike> cluster;
    typedef QList<cluster *> listClusters;

public:
    plotFitting();
    QGridLayout *mainLayout;
    QCustomPlot *IsiGraph;
    QCustomPlot *CrossCorrelogrammGraph;
    QCustomPlot *PredictionGraph;
    QCustomPlot *FittingValuesGraph;
    QPushButton *btMerge;
    QPushButton *btSplit;
    QPushButton *btreRunClustering;
    QTableWidget *ClusterList;
    QList<int> listIndexSelected;

};
#endif //PLOTFITTING_H
