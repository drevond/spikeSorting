#include "INTERFACE/gui_spikesorting.h"
#include "CALCUL/enginesorting.h"
#include <QApplication>
struct spike
{
    int frameSpike;
    int ChannelMax;
    QTime timeSpike;
};
typedef QList<spike> cluster;
typedef QList<cluster *> listClusters;
typedef QVector<double> vector;

int main(int argc, char *argv[])
{
    QString filename="/home/dorian/spikesorting/DATA/2013_07_12_primate_11_480_15-100.raw";
    QFile *fileRead=new QFile(filename);
    int nbChannels=256;
    QApplication a(argc, argv);
    GUI_SpikeSorting *interfaceSorting=new GUI_SpikeSorting(filename);
    engineSorting *engineCell=new engineSorting(fileRead);
    QThread *computations=new QThread;
    engineCell->moveToThread(computations);
    //--------------CONNECTIONS--------------------------
    QObject::connect(interfaceSorting,SIGNAL(emitBeginComputations(int,int)),engineCell,SLOT(slotBeginClusterization(int,int)));
    qRegisterMetaType<listClusters>("listClusters");
    QObject::connect(engineCell,SIGNAL(emitClusters(listClusters)),interfaceSorting,SIGNAL(emitLoadClusters(listClusters)));
    computations->start();
    test Testing;
    qRegisterMetaType<real_2d_array>("real_2d_array");
    QObject::connect(engineCell,SIGNAL(emitMatrix(real_2d_array,int,int,QString)),&Testing,SLOT(slotPlot(real_2d_array,int,int,QString)));
    QObject::connect(interfaceSorting,SIGNAL(EmitGuiApplyFilter(int,int,int)),engineCell,SLOT(slotFilterData(int,int,int)));
    QObject::connect(engineCell,SIGNAL(EmitEndOfDenoising()),interfaceSorting,SIGNAL(emitEndofDenoising()));
    QObject::connect(interfaceSorting,SIGNAL(emitGuiGetdata(QTime,int,int,int,int)),engineCell,SLOT(slotGetSpike(QTime,int,int,int,int)),Qt::DirectConnection);
    qRegisterMetaType<vector>("vector");
    QObject::connect(engineCell,SIGNAL(emitSpikeChannel(vector)),interfaceSorting,SIGNAL(emitSpikeChannelReceived(vector)));

    //Testing.startTest();
    interfaceSorting->setMaximumWidth(2000);
    interfaceSorting->showMaximized();
    return a.exec();
}
