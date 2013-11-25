#ifndef READINGOBJECT_H
#define READINGOBJECT_H
#include <QtWidgets>

class readingObject:public QObject
{
    Q_OBJECT
    typedef QList<ushort> myArray;
public:
    explicit readingObject(QString fileToRead);

    QDataStream *DataFile;
    QFile *file;
    bool isPause;
    bool isStop;
    QWaitCondition QwaitPause;

    void setSizeBuffer(int timeSizeToSend);

    int posBegin;
    double Speed;
    QList<ushort> bufferCourant;

private:
    int posCurrent;
    int sizeBuffer;
    int nbChannels;
    void lireHeader();

signals:
    void EmitSizeBufferReady(const myArray dataToPlot);
    void emitShowValue(QString value);

public slots:
    void reading();
};

#endif // READINGOBJECT_H
