#include "readdata.h"

ReadData::ReadData(QFile *fileToRead, int nbOfChannels)
{
    nbChannels=nbOfChannels;
    fichier=new QFile(fileToRead->fileName());
    if (!fichier->open(QIODevice::ReadOnly))
        return;
    flux=new QDataStream(fichier);
    flux->setByteOrder(QDataStream::LittleEndian);
    skipheader();
}

QList<QVector<double> *> ReadData::GetData(QTime time, int durationMs)
{
    QList<QVector<double> *> channelsData;
    //init channels
    for(int i=0;i<nbChannels;i++)
    {
        channelsData.append(new QVector<double>);
    }
    //read data
    int timeInMsecs=abs(time.msecsTo(QTime(0,0)));
    long begining=posEndOfHeader+timeInMsecs*20*nbChannels*2;
    fichier->seek(begining);
    int nbDataPerChannel=durationMs*20;
    double currentVoltage;
    ushort currentUshort;
    for(long i=0;i<nbDataPerChannel;i++)
    {
        for(int j=0;j<nbChannels;j++)
        {
            flux->operator >>(currentUshort);
            currentVoltage=(currentUshort-32767)*0.142;
            channelsData[j]->append(currentVoltage);
        }
    }
    return channelsData;
}

QVector<double> ReadData::getSpikeChannel(QTime time, int nbFrameA, int nbFrameB, int channel,int FrameSpike)
{
    QVector<double> res;
    int timeInMsecs=abs(time.msecsTo(QTime(0,0)));
    long frameTime=timeInMsecs*20+FrameSpike-nbFrameB;
    if(frameTime<0)
        frameTime=0;
    long CurrentPos=posEndOfHeader+frameTime*nbChannels*2+channel*2;
    double currentVoltage;
    ushort currentUshort;
    for(int i=0;i<nbFrameB+nbFrameA+1;i++)
    {
        fichier->seek(CurrentPos);
        flux->operator >>(currentUshort);
        currentVoltage=(currentUshort-32767)*0.142;
        res.append(currentVoltage);
        CurrentPos+=(nbChannels-1)*2;
    }
    return res;
}

void ReadData::skipheader()
{
    //skip header
    quint8 charRead;
    QString endOfHeader="abcde";
    while(endOfHeader!="EOH\r\n")
    {
        flux->operator >>(charRead);
        header.append(charRead);
        endOfHeader.remove(0,1);
        endOfHeader.append(charRead);
    }
    posEndOfHeader=fichier->pos();
}
