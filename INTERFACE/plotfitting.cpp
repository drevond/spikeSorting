#include "plotfitting.h"

plotFitting::plotFitting()
{
    //widgets
    mainLayout=new QGridLayout;
    //graphs
    IsiGraph=new QCustomPlot;
    CrossCorrelogrammGraph=new QCustomPlot;
    PredictionGraph=new QCustomPlot;
    FittingValuesGraph=new QCustomPlot;    
    IsiGraph->plotLayout()->insertRow(0);
    IsiGraph->plotLayout()->addElement(0,0,new QCPPlotTitle(IsiGraph,"Interspike Interval"));
    FittingValuesGraph->plotLayout()->insertRow(0);
    FittingValuesGraph->plotLayout()->addElement(0,0,new QCPPlotTitle(FittingValuesGraph,"fitting Values"));
    CrossCorrelogrammGraph->plotLayout()->insertRow(0);
    CrossCorrelogrammGraph->plotLayout()->addElement(0,0,new QCPPlotTitle(CrossCorrelogrammGraph,"Cross Correlogramm"));
    PredictionGraph->plotLayout()->insertRow(0);
    PredictionGraph->plotLayout()->addElement(0,0,new QCPPlotTitle(PredictionGraph,"Prediction"));
    IsiGraph->xAxis->setVisible(false);
    IsiGraph->yAxis->setVisible(false);
    CrossCorrelogrammGraph->xAxis->setVisible(false);
    CrossCorrelogrammGraph->yAxis->setVisible(false);
    IsiGraph->setMaximumHeight(300);
    FittingValuesGraph->setMaximumHeight(300);
    CrossCorrelogrammGraph->setMaximumHeight(300);
    PredictionGraph->setMaximumHeight(300);
    //buttons
    btMerge=new QPushButton("Fusionner");
    btSplit=new QPushButton("Split cells");
    btreRunClustering=new QPushButton("reRun Clustering");
    //tablelist
    ClusterList=new QTableWidget(this);
    FonctionsUtilesI::fillItems(60,ClusterList,listIndexSelected);
    ClusterList->setMinimumSize(100,100);
    //layout
    mainLayout->addWidget(IsiGraph,0,0,2,2);
    mainLayout->addWidget(CrossCorrelogrammGraph,2,0,2,2);
    mainLayout->addWidget(PredictionGraph,0,2,2,2);
    mainLayout->addWidget(FittingValuesGraph,2,2,2,2);
    mainLayout->addWidget(btMerge,4,0,1,1);
    mainLayout->addWidget(btSplit,6,0,1,1);
    mainLayout->addWidget(btreRunClustering,5,0,1,1);
    mainLayout->addWidget(ClusterList,4,1,3,3);
    setLayout(mainLayout);
}
