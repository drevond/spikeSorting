#ifndef READDATA_H
#define READDATA_H
#include <QtWidgets>
class ReadData
{
public:
    int nbChannels;
    ReadData(QFile *fileToRead,int nbOfChannels=256);
    QList<QVector<double> *> GetData(QTime time, int durationMs);
    QVector<double> getSpikeChannel(QTime time, int nbFrameA, int nbFrameB, int channel, int FrameSpike);
    QString header;
private:
    QFile *fichier;
    QDataStream *flux;
    int posEndOfHeader;
    QTime data;
    void skipheader();
};


#endif // READDATA_H
