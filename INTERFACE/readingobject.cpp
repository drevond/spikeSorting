#include "readingobject.h"

// ----Constructeur---
readingObject::readingObject(QString fileToRead) :
    isPause(false)
{
    nbChannels=256;
    Speed=1;
    file=new QFile(fileToRead);
    QString a=fileToRead;
    DataFile=new QDataStream(file);
    DataFile->setByteOrder(QDataStream::LittleEndian);
    //ouverture de fichier en lecture seule
    if(!file->open(QIODevice::ReadOnly))
        return;
    lireHeader();

}
//--- PROCESS ---
void readingObject::reading()
{
    posBegin=file->pos();
    ushort nbCourant;
    isStop=false;
    while(!DataFile->atEnd())
    {
        if(isPause==true||isStop==true)
        {
            QMutex *mutex=new QMutex;
            mutex->lock();
            QwaitPause.wait(mutex);
            mutex->unlock();
        }
        posCurrent=file->pos();
        DataFile->operator >>(nbCourant);
        bufferCourant<<nbCourant;
        if(bufferCourant.count()==sizeBuffer)
        {
            emit EmitSizeBufferReady(bufferCourant);
            bufferCourant.clear();
            int timeToWait=50/Speed;
            QThread::msleep(timeToWait);
        }
    }
}

void readingObject::setSizeBuffer(int timeSizeToSend)
{
    sizeBuffer=timeSizeToSend*20*nbChannels;
}

void readingObject::lireHeader()
{
    quint8 charRead;
    QString header;
    QString endOfHeader="abcde";
    int sizeHeader=0;
    while(endOfHeader!="EOH\r\n")
    {
        sizeHeader++;
        DataFile->operator >>(charRead);
        header.append(QChar(charRead));
        endOfHeader.remove(0,1);
        endOfHeader.append(charRead);
    }
}
