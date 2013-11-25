#include "plotcluster.h"
//---------------CONSTRUCTOR--------------

PlotCluster::PlotCluster(int nbOfChannels)
{
    nbframeSpikeA=30;
    nbframeSpikeB=20;
    nbframeSpike=nbframeSpikeA+nbframeSpikeB+1;
    nbChannels=nbOfChannels;
    findNbRowCol(nbChannels);
    //options
    gbParameters=new QGroupBox("Options");
    QVBoxLayout *OptionsLayout=new QVBoxLayout;
    mainLayout=new QGridLayout;
    examplesLayout=new QGridLayout;
    ToolLayout=new QGridLayout;

    //combo
    cbFeature1=new QComboBox;
    cbFeature2=new QComboBox;
    for(int i=0;i<nbChannels;i++)//fill comboFeature
    {
        cbFeature1->addItem("AmpMaxCh"+QString::number(i));
        cbFeature2->addItem("AmpMaxCh"+QString::number(i));
    }
    OptionsLayout->addWidget(cbFeature1);
    OptionsLayout->addWidget(cbFeature2);
    gbParameters->setLayout(OptionsLayout);

    //initialisation examples(examples layout + listChannelSelected)
    InitExamples();

    //Initialisation features space
    initFeatureSpace();

    //init tool
    buildToolLayout();
    //SlotUpdateView();
    fillLayoutOfExemples();
    mainLayout->addLayout(examplesLayout,0,0,2,2);
    mainLayout->addWidget(plotFeature,0,2,1,1);
    mainLayout->addLayout(ToolLayout,1,2,1,1);
    setLayout(mainLayout);

    //-------------connections
    connect(btUpdateView,SIGNAL(clicked()),this,SLOT(SlotUpdateView()));
    connect(btTimeClustering,SIGNAL(clicked()),this,SLOT(slotChangeTimeClustering()));
    connect(btDurationClustering,SIGNAL(clicked()),this,SLOT(slotChangeDurationClustering()));
}


//-----------------UPDATE ------------------------
void PlotCluster::SlotUpdateView()
{
    findIndexClustersSelected();
    /*
    //randomdata
    //load clustersToDraw
    listClustersDataToDraw.clear();
    for(int i=0;i<listIndexClustersSelected.count();i++)
    {
        int cluster=listIndexClustersSelected[i];
        emitClusterSelected(cluster);
        //wait for the incoming data.
    }
    */
    //update view of examples
    fillLayoutOfExemples();

    //update view of features Space
    plotFeatures();

}

//--------------PLOT EXAMPLES---------------------

void PlotCluster::fillLayoutOfExemples()
{
    int nbSpike=0;
    //remove formers graphs
    for(int channel=0;channel<nbChannels;channel++)
    {
        listClutersExamplesPlot[channel]->clearGraphs();
    }
    ClusterData clusterToDraw;
    //fill graphs
    for(int i=0;i<listIndexClustersSelected.count();i++)
    {
        int cluster=listIndexClustersSelected[i];
        //parameters of graphs
        QPen penCluster;
        QColor colorCluster=QColor(colors[cluster]);
        penCluster.setColor(colorCluster);
        //build spikes that need to be showed
        clusterToDraw=buildSpikes(cluster);
        //set data
        QVector<double> vecData;
        for(int spikeNb=0;spikeNb<clusterToDraw.count();spikeNb++)
        {
            for(int i=0;i<listChannelSelected.count();i++)
            {
                int channel=listChannelSelected[i];
                listClutersExamplesPlot[channel]->addGraph();
                vecData=*clusterToDraw[spikeNb].spikesChannels[0];
                listClutersExamplesPlot[channel]->graph(spikeNb)->setData(timeVector,vecData);
                listClutersExamplesPlot[channel]->graph(spikeNb)->setPen(penCluster);
            }
            nbSpike++;
        }
    }
    //enable visible for channels selected
    for(int i=0;i<nrow;i++)
    {
        for(int j=0;j<ncol;j++)
        {
            int index=i*ncol+j;
            QString a=ListChannelToSelect->item(i,j)->text();
            if(ListChannelToSelect->item(i,j)->isSelected())
            {
                listClutersExamplesPlot[index]->replot();
                listClutersExamplesPlot[index]->setVisible(true);
            }
            else
            {
                listClutersExamplesPlot[index]->setVisible(false);
            }
        }
    }
}

PlotCluster::ClusterData PlotCluster::buildSpikes(int clusterNb)
{
    ClusterData res;
    spike currentSpike;
    spikeData currentSpikeData;
    int channel;
    //determine channels selected
    findChannelsSelected();
    for(int spikeNb=0;spikeNb<listClustersToDraw[clusterNb]->count();spikeNb++)
    {
        currentSpike=listClustersToDraw[clusterNb]->at(spikeNb);
        currentSpikeData.spikesChannels.clear();
        for(int i=0;i<listChannelSelected.count();i++)
        {
            channel=listChannelSelected[i];
            SpikeChannelIncoming.clear();
            //read data and put it into vec
            emitGetSpikeChannel(currentSpike.timeSpike,nbframeSpikeA,nbframeSpikeB,channel,currentSpike.frameSpike);
            currentSpikeData.spikesChannels.append(new QVector<double>(SpikeChannelIncoming));
        }
        //add spike to cluster
        res.append(currentSpikeData);
    }
    return res;
}

//----------------GENERAL FUNCTIONS--------------------

void PlotCluster::fillItems(int number)
{
    findNbRowCol(number);
    //remove former elements
    int a=ListClustersToSelect->rowCount();
    for(int i=0;i<a-1;i++)
    {
        ListClustersToSelect->removeRow(0);
    }
    a=ListClustersToSelect->columnCount();
    for(int i=0;i<a-1;i++)
    {
        ListClustersToSelect->removeColumn(0);
    }
    //fill elements
    element=0;
    for(int j=0;j<ncol-1;j++)
    {
        ListClustersToSelect->insertColumn(j+1);
    }
    for(int i=0;i<nrow-1;i++)
    {
        ListClustersToSelect->insertRow(i+1);
    }
    //set items
    int i=0,j=0,k=0;
    while(j<ncol)
    {
        i=0;
        while(i<nrow&&element<number)
        {
            ListClustersToSelect->setItem(i,j,new QTableWidgetItem("cluster "+QString::number(element)));
            if(k<listIndexClustersSelected.count() && listIndexClustersSelected[k]==element)
            {
                ListClustersToSelect->item(i,j)->setSelected(true);
                k++;
            }
            i++;
            element++;
        }
        j++;
    }
}


//--------------PLOT FEATURES--------------------

void PlotCluster::plotFeatures()
{
    int feature1=cbFeature1->currentIndex();
    int feature2=cbFeature2->currentIndex();

    // set labels:
    plotFeature->xAxis->setLabel(cbFeature1->currentText());
    plotFeature->yAxis->setLabel(cbFeature2->currentText());
    plotFeature->clearGraphs();
    //set data of graphs
    for(int cluster=0;cluster<listIndexClustersSelected.count();cluster++)
    {
        plotFeature->addGraph(plotFeature->xAxis,plotFeature->yAxis);
        //parameters of graphs
        QColor colorCluster=QColor(colors[cluster]);
        QCPScatterStyle scatterFeature;
        scatterFeature.setShape(QCPScatterStyle::ssStar);
        scatterFeature.setPen(colorCluster);
        scatterFeature.setSize(5);
        //go to time t of spikes and get the value on the channel corresponding to tthe feature selected.
        //QVector<double> data1=FeatureSpikesCluster(feature1,cluster);
        //QVector<double> data2=FeatureSpikesCluster(feature2,cluster);
        //plotFeature->graph(cluster)->setData(data1,data2);
        plotFeature->graph(cluster)->setScatterStyle(scatterFeature);
        plotFeature->graph(cluster)->setLineStyle(QCPGraph::lsNone);
    }
    plotFeature->replot();
}





//-------------UTILES FUNCTIONS----------------
void PlotCluster::buildToolLayout()
{
    btMerge=new QPushButton("Merge");
    btMerge->setEnabled(false);
    btTimeClustering=new QPushButton("select time of begining for clustering");
    btDurationClustering=new QPushButton("select duration of clustering");
    durationClusteringS=3;//s
    timeBeginingClustering=0;//min
    labDuration=new QLabel(QString::number(durationClusteringS)+"secondes");
    labTime=new QLabel(QString::number(timeBeginingClustering)+"min");
    btRunClustering=new QPushButton("Run Clustering");
    btUpdateView=new QPushButton("update view");
    ListClustersToSelect=new QTableWidget(1,1,this);
    ListClustersToSelect->setItem(0,0,new QTableWidgetItem("aucun"));
    //place les widgets
    ToolLayout->addWidget(ListChannelToSelect,0,0,3,3);
    ToolLayout->addWidget(gbParameters,3,0,2,3);
    ToolLayout->addWidget(ListClustersToSelect,0,3,2,2);
    ToolLayout->addWidget(btMerge,2,3,1,1);
    ToolLayout->addWidget(btUpdateView,2,4,1,1);
    ToolLayout->addWidget(btRunClustering,3,3,1,1);
    ToolLayout->addWidget(btTimeClustering,4,3,1,1);
    ToolLayout->addWidget(btDurationClustering,4,4,1,1);
    ToolLayout->addWidget(labDuration,5,4,1,1);
    ToolLayout->addWidget(labTime,5,3,1,1);
    //colors
    colors<<QColor::colorNames();
    colors<<QColor::colorNames();
}

double PlotCluster::computeMax(QVector<double> list)
{
    double max=list[0];
    for(int i=0;i<list.count();i++)
    {
        if(max<list[i])
        {
            max=list[i];
        }
    }
    return max;
}


QList<QVector<double> *> PlotCluster::convertSpikeToSpikeChannel(QVector<double> *spike)
{
    QList<QVector<double> *> SpikeChannel;
    for(int channel=0;channel<nbChannels;channel++)
    {
        QVector<double> *vect=new QVector<double>;
        vect->operator <<(spike->mid(channel,13));
        SpikeChannel.append(vect);
    }
    return SpikeChannel;
}

void PlotCluster::findIndexClustersSelected()
{
    listIndexClustersSelected.clear();
    int i=0,j=0,courant=0;
    while(j<ncol)
    {
        i=0;
        while(i<nrow&&courant<element)
        {
            bool a=ListClustersToSelect->item(i,j)->isSelected();
            if(a)
            {
                listIndexClustersSelected.append(courant);
            }
            courant++;
            i++;
        }
        j++;
    }
}

void PlotCluster::findChannelsSelected()
{
    listChannelSelected.clear();
    QString name;
    int ncolumns=ListChannelToSelect->columnCount();
    int nrows=ListChannelToSelect->rowCount();
    int channel;
    for(int i=0;i<nrows;i++)
    {
        for(int j=0;j<ncolumns;j++)
        {
            name=ListChannelToSelect->item(i,j)->text();
            channel=i*nrows+j;
            if(ListChannelToSelect->item(i,j)->isSelected())
                listChannelSelected.append(channel);
        }
    }
}

void PlotCluster::findNbRowCol(int number)
{
    nrow=1;
    ncol=1;
    int u=nrow*nrow;//on cherche raw comme un carré parfait

    while(u<number)
    {
        nrow++;
        u=nrow*nrow;
    }
    int test=(nrow-1)*nrow-number;
    int test2=nrow*nrow-number;
    if(test>0&&test2-test>0)
    {
        ncol=nrow-1;
    }
    else
    {
        ncol=nrow;
    }
}

void PlotCluster::InitExamples()
{
    element=0;
    ListChannelToSelect=new QTableWidget(nrow,ncol);
    //Fill Examples Plot
    for(int i=0;i<nrow;i++)
    {
        for(int j=0;j<ncol;j++)
        {
            int currentCh=i*ncol+j;
            //listChannelSelected
            ListChannelToSelect->setItem(i,j,new QTableWidgetItem("channel"+QString::number(currentCh)));
            //listClusterPlot
            listClutersExamplesPlot.append(new QCustomPlot);
                //add title layout element:
            listClutersExamplesPlot[currentCh]->plotLayout()->insertRow(0);
            listClutersExamplesPlot[currentCh]->plotLayout()->addElement(0, 0, new QCPPlotTitle(listClutersExamplesPlot[currentCh], "channel "+QString::number(currentCh)));
                //set Axis range
            listClutersExamplesPlot[currentCh]->xAxis->setRange(0,7);
            listClutersExamplesPlot[currentCh]->yAxis->setRange(-100,50);
                // set axis labels:
            listClutersExamplesPlot[currentCh]->xAxis->setLabel("time (ms)");
            listClutersExamplesPlot[currentCh]->yAxis->setLabel("amplitude(µV)");
            examplesLayout->addWidget(listClutersExamplesPlot[currentCh],i,j);
        }
    }

    //generate time vector
    for(int i=0;i<nbframeSpike;i++)
    {
        timeVector.append(i/20.0);
    }

    ListChannelToSelect->item(0,0)->setSelected(true);
}

void PlotCluster::initFeatureSpace()
{
    plotFeature=new QCustomPlot;

    //plotFeature->addGraph(plotFeature->xAxis,plotFeature->yAxis);
    plotFeature->xAxis->setRange(0,100);
    plotFeature->yAxis->setRange(0,100);

    // set labels:
    plotFeature->xAxis->setLabel(cbFeature1->currentText());
    plotFeature->yAxis->setLabel(cbFeature2->currentText());

    //add title layout element:
    plotFeature->plotLayout()->insertRow(0);
    plotFeature->plotLayout()->addElement(0, 0, new QCPPlotTitle(plotFeature, "Space Of Features"));
}

//slots
void PlotCluster::SlotNbClusters(int nbclusters)
{
    //keep in memory the list of clusters selected and update listClusterToSelect
    findIndexClustersSelected();
    fillItems(nbclusters);
}


void PlotCluster::slotLoadClusters(listClusters ListClusters)
{
    listClustersToDraw=ListClusters;
    btRunClustering->setEnabled(true);
    SlotNbClusters(listClustersToDraw.count());
}

void PlotCluster::slotReceiveSpikeChannel(vector SpikeChannel)
{
    SpikeChannelIncoming=SpikeChannel;
}

void PlotCluster::slotChangeDurationClustering()
{
    QString textTime=QInputDialog::getText(this,
                                           "change Time cursor",
                                           "enter a valid time in milliseconds to select the duration of the data to be clustered",QLineEdit::Normal);
    durationClusteringS=textTime.toInt(0,10);
    labDuration->setText(QString::number(durationClusteringS)+"sec");
}

void PlotCluster::slotChangeTimeClustering()
{
    QString textTime=QInputDialog::getText(this,
                                        "change Time cursor",
                                        "enter a valid time in minutes to select the begining of the clustering for the data",QLineEdit::Normal);
    int Time=textTime.toInt(0,10);
    timeBeginingClustering=textTime.toInt(0,10);
    labTime->setText(QString::number(timeBeginingClustering)+"min");
}

//former  functions
/*//compute the maximum value of amplitude for a spike(belonging to a cluster)on one channel
double PlotCluster::computeMaxSpikeOnChannel(int feature,int cluster,int spikes )
{
    spike CurrentSpike=listClustersToDraw[cluster]->at(spikes);
    QList<QVector<double> *> dataSpike=CurrentSpike.spikesChannels;
    return computeMax(*dataSpike[feature]);
}
//Gives the data for the feature selected and the cluster selected
QVector<double> PlotCluster::FeatureSpikesCluster(int feature, int cluster)
{
    QVector<double> result;
    int nbSpikes=listClustersToDraw[cluster]->count();
    for(int spike=0;spike<nbSpikes;spike++)
    {
        result.append(computeMaxSpikeOnChannel(feature,cluster,spike));
    }
    return result;
}
//The size of result must be the number of spikes of the cluster selected
void PlotCluster::ReceiveSpike(ListSpikes Cluster)
{
    cluster currentCluster;
    spike currentSpike;
    for(int spikes=0;spikes<Cluster.count();spikes++)
    {
        currentSpike.spikesChannels=convertSpikeToSpikeChannel(Cluster[spikes]);
        currentCluster.append(currentSpike);
    }
    listClustersToDraw.append(&currentCluster);
}

*/
