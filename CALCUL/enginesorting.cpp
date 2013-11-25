#include "enginesorting.h"

engineSorting::engineSorting(QFile *FileToRead, int nbchannelsW, int nbchannelstot)
{
    nbChannelsW=nbchannelsW;
    readDataRaw=new ReadData(FileToRead);
    QFile fileDenoised("dataDenoised");
    readDataDenoised=new ReadData(FileToRead);
    pth=5;
    nbChannelsTot=nbchannelstot;
    fileSpikes=new QFile("SpikeTimes");
    fileSpikesChannel=new QFile("SpikesChannels");
    //initialize list clusters.
    for(int i=0;i<nbChannelsTot;i++)
    {
        ListClusters.append(new cluster);
    }
    test=0;

}


//clusterization
void engineSorting::buildPreclusters(QTime time)
{
    nbFrameSpikeB=1.5*20;//2ms
    nbFrameSpikeA=(int)3*20;//3ms
    fileSpikesChannel->open(QIODevice::ReadOnly);
    QDataStream inChannel(fileSpikesChannel);
    fileSpikes->open(QIODevice::ReadOnly);
    QDataStream inspike(fileSpikes);
    //go to where it begins.
    fileSpikes->seek(beginSpike);
    spike *currentSpike;
    int channelMax;
    QVector<double> *vec;
    int a=0;
    while(!inChannel.atEnd())
    {
        //create spike
        inChannel.operator >>(channelMax);
        currentSpike=new spike;
        currentSpike->ChannelMax=channelMax;
        currentSpike->timeSpike=time;
        inspike.operator >>(currentSpike->frameSpike);
        ListClusters[channelMax]->append(*currentSpike);
    }
    fileSpikes->close();
    fileSpikesChannel->close();
}

void engineSorting::findSpikesForPreClustering(int threshold,QTime Time)
{
    int compteur=0;
    channelsData.clear();
    channelsData=readDataDenoised->GetData(Time,1000);//data stored in channel data containing 1sec of data.
    fileSpikes->open(QIODevice::WriteOnly);
    fileSpikesChannel->open(QIODevice::WriteOnly);
    fluxOutSpike=new QDataStream(fileSpikes);
    fluxOutSpikeChannel=new QDataStream(fileSpikesChannel);
    //write at the end of the file
    beginSpike=test;
    fileSpikes->seek(beginSpike);
    fileSpikesChannel->seek(fileSpikesChannel->size());
    //detect spikes
    QVector<double> listMaxi;
    QVector<double> listChannel;
    double currentValue;
    double nextValue;
    double formerValue;
    for(int frame=1;frame<channelsData[0]->count()-1;frame++)
    {
        listChannel.clear();
        listMaxi.clear();
        for(int channel=0;channel<channelsData.count();channel++)
        {
            currentValue=abs(channelsData[channel]->operator [](frame));
            nextValue=abs(channelsData[channel]->operator [](frame+1));
            formerValue=abs(channelsData[channel]->operator [](frame-1));
            //check if it is a minimum local and if it is above the threshold
            if(currentValue>threshold&&currentValue>nextValue&&currentValue>formerValue&&channel!=83)
            {
                listMaxi.append(currentValue);
                listChannel.append(channel);
            }
        }
        //if there is a spike
        if(!listChannel.empty())
        {
            compteur++;
            fluxOutSpike->operator <<(frame);
            int channelMax=listChannel[fonctionsUtiles::getIndexMax(listMaxi)];
            fluxOutSpikeChannel->operator <<(channelMax);
            test=fileSpikes->pos();
        }
    }
    fileSpikes->close();
    fileSpikesChannel->close();
}

void engineSorting::clusterization(int algo)
{
    emitClusters(ListClusters);
}

//denoising
void engineSorting::DenoiseData(int timeBeginS,int durationS)
{
    QString filename="whitenedData"+QString::number(timeBeginS)+"_"+QString::number(durationS);
    fileDataDenoised->setFileName(filename);
    QTime time;
    //for saving Data
    fileDataDenoised->open(QIODevice::WriteOnly);
    QDataStream fluxOut(fileDataDenoised);
    ushort Value=0;
    //header
    QByteArray Header=readDataRaw->header.toUtf8();
    fluxOut<<Header;
    fluxOut.setByteOrder(QDataStream::LittleEndian);

    //-----------------build whiteningMatrix-----------------------
    QFile fileWhiteningMatrix("whiteningMatrix");
    QDataStream fluxMatrix(&fileWhiteningMatrix);
    real_2d_array algWhiteningMatrix;
    if(fileWhiteningMatrix.open(QIODevice::ReadOnly))
    {
        double value;
        QList<QVector<double> *> QwhiteningMatrix=fonctionsUtiles::InitialiserQlist(nbChannelsTot);
        for(int i=0;i<nbChannelsTot;i++)
        {
            for(int j=0;j<nbChannelsTot;j++)
            {
                fluxMatrix.operator >>(value);
                QwhiteningMatrix[i]->append(value);
            }
        }
        algWhiteningMatrix=fonctionsUtiles::convert(QwhiteningMatrix);
    }
    else
    {
        algWhiteningMatrix=BuildWhiteningMatrix(10);//10 sec of data to build the whitening matrix
        fileWhiteningMatrix.open(QIODevice::WriteOnly);
        for(int i=0;i<algWhiteningMatrix.rows();i++)
        {
            for(int j=0;j<algWhiteningMatrix.cols();j++)
            {
                fluxMatrix<<algWhiteningMatrix.operator ()(i,j);
            }
        }
    }
    fileWhiteningMatrix.close();
    emitMatrix(algWhiteningMatrix,20,20,"whiteningMatrix");

    //------------------Denoise And Whiten Data-----------------------
    //time begin
    for(int t=0;t<6;t++)
    {
        time.setHMS(0,0,t+1);
        TimeOfComputation(time,1000);
        //------------------HIGH-PASS filter data
        for(int channel=0;channel<nbChannelsTot;channel++)
        {
            dataFiltered=filtre.filterData(*channelsData[channel]);
            channelsData[channel]->clear();
            channelsData[channel]->operator <<(dataFiltered);
        }
        //-------------------WHITEN data
        //convert data
        real_2d_array vectors=fonctionsUtiles::convert(channelsData);
        real_2d_array vectorsT=fonctionsUtiles::TransposeMatrix(vectors);
        //transpose
        //apply whitening
        real_2d_array DataWhitened=fonctionsUtiles::MatriceProduct(algWhiteningMatrix,vectorsT);
        //transpose again
        real_2d_array DataDenoisedTransposed=fonctionsUtiles::TransposeMatrix(DataWhitened);
        //convert again
        channelsData=fonctionsUtiles::convertInv(DataDenoisedTransposed);
        //--------------------SAVE data.
        for(int i=0;i<nbChannelsTot;i++)
        {
            for(int j=0;j<nbChannelsTot;j++)
            {
                double voltage=channelsData[j]->operator [](i);
                Value=(ushort)(voltage/0.1+32767);//gain=0.1
                fluxOut<<Value;
            }
        }
    }
    fileDataDenoised->close();
}


real_2d_array engineSorting::BuildWhiteningMatrix(int timeSecToBuild)
{
    for(int i=0;i<nbChannelsTot;i++)
    {
        whiteningMatrix.append(new QVector<double>);
    }
    //getData timesectobuild every sec
    for(int tempsCourant=0;tempsCourant<timeSecToBuild;tempsCourant++)
    {
        QTime time;
        time.setHMS(0,tempsCourant+1,0);
        //select data in channelsData
        TimeOfComputation(time,100);
        //initialize whitening matrix

        //make a copy of channels data
        QVector<int> timeSpikes;
        QList<QVector<double> *> dataWithoutSpikes;
        for(int channel=0;channel<nbChannelsTot;channel++)
        {
            QVector<double> *vec=new QVector<double>(*channelsData[channel]);
            dataWithoutSpikes.append(vec);
        }
        //---------------------HIGH PASS filter and find spikes.
        for(int channel=0;channel<nbChannelsTot;channel++)
        {
            //dataFiltered=filtre.filterData(*channelsData[channel]);
            //channelsData[channel]->clear();
            //channelsData[channel]->operator <<(dataFiltered);
            timeSpikes.operator +=(findSpikesForDenoising(*dataWithoutSpikes[channel],70));
        }
        int FramesSpikeBefore=2;
        int FramesSpikeAfter=2;
        fonctionsUtiles::MakeSet(&timeSpikes,FramesSpikeBefore,FramesSpikeAfter);

        //-------------------------REMOVE Spikes to keep only noise
        //--------put zero where there is spike
        for(int i=0;i<timeSpikes.count();i++)
        {
            int time=timeSpikes[i];
            int k=fonctionsUtiles::max(time-FramesSpikeBefore,0);
            int end=fonctionsUtiles::min(time+FramesSpikeAfter,dataWithoutSpikes[0]->count());
            while(k<end)
            {
            for(int j=0;j<nbChannelsTot;j++)
                {
                    dataWithoutSpikes[j]->operator [](k)=0;
                }
            k++;
            }
        }
        //---------remove zeros in the data
        int i=0;
        int pointCourant=0;
        int size=dataWithoutSpikes[0]->count();
        while(i<size)
        {
            if(dataWithoutSpikes[0]->operator [](pointCourant)==0)
            {
                for(int channel=0;channel<dataWithoutSpikes.count();channel++)
                {
                    dataWithoutSpikes[channel]->remove(pointCourant);
                }
                pointCourant--;
            }
            pointCourant++;i++;
        }
        //----------------------STORE new data in whiteningMatrix
        for(int channel=0;channel<nbChannelsTot;channel++)
        {
            whiteningMatrix[channel]->operator +=(*dataWithoutSpikes[channel]);
        }
    }
    //-----------------------BUILD whitening matrix
    //convertions
    real_2d_array DataMatrix=fonctionsUtiles::convert(whiteningMatrix);
    //----calcul de la matrice de covariance
    real_2d_array Covariance;
    covm(DataMatrix,Covariance);
    //----calcul de l'inverse de la racine carrée de la covarianceM(whitening matrix)
    real_2d_array Pass,PassInv;//passage matrix
    real_1d_array diag;
    //single value decomposition
    rmatrixsvd(Covariance,Covariance.rows(),Covariance.cols(),2,2,2,diag,PassInv,Pass);
    //change elements in the diagonal
    for(int i=0;i<diag.length();i++)
    {
        diag[i]=1/sqrt(diag[i]);
    }
    //make matrix
    real_2d_array matrixDiag=fonctionsUtiles::BuildMatrixDiag(diag);
    real_2d_array R2dwhiteningMatrix;
    R2dwhiteningMatrix=fonctionsUtiles::MatriceProduct(fonctionsUtiles::MatriceProduct(Pass,matrixDiag),PassInv);
    return R2dwhiteningMatrix;
}

QVector<int> engineSorting::findSpikesForDenoising(QVector<double> DataChannel,int threshold)
{
    //set threshold
    /*double mediane=fonctionsUtiles::median(dataFiltered);
    //vector of deviations
    QVector<double> deviations;
    for(int i=0;i<dataFiltered.count();i++)
    {
        deviations.append(abs(dataFiltered[i]-mediane));
    }
    //threshold=pth*fonctionsUtiles::median(deviations);
*/
    QVector<int> result;
    double currentValue;
    double nextValue;
    double formerValue;
    for(int i=1;i<DataChannel.count()-1;i++)
    {
        currentValue=abs(DataChannel[i]);
        nextValue=abs(DataChannel[i+1]);
        formerValue=abs(DataChannel[i-1]);
        //detect Spike
        if(currentValue>threshold&&currentValue>nextValue&&currentValue>formerValue)
        {
            int timeSpike=i;
            result.append(timeSpike);
        }
    }
    return result;
}


//slots
void engineSorting::slotBeginClusterization(int timeMn,int durationS)
{
    QTime time(0,timeMn,0);
    beginSpike=0;
    for(int i=0;i<durationS;i++)
    {
        //find spikes
        findSpikesForPreClustering(50,time);
        //-----------------------------clusterization
        buildPreclusters(time);
        time=time.addSecs(1);
    }
    clusterization(0);
}

void engineSorting::slotFilterData(int timeBegining, int duration,int nbFilter)
{
    filtre.SetFilter(nbFilter);
    DenoiseData(timeBegining,duration);
    emit EmitEndOfDenoising();
}

void engineSorting::slotGetSpike(QTime time,int nbframeSpikeAfter,int nbframeSpikeBefore,int channel,int Frame)
{
    QVector<double> spikeChannel=readDataDenoised->getSpikeChannel(time,nbframeSpikeAfter,nbframeSpikeBefore,channel,Frame);
    emit emitSpikeChannel(spikeChannel);
}

//autres
void engineSorting::TimeOfComputation(QTime time,int durationMs)
{
    channelsData=readDataRaw->GetData(time,durationMs);
}
