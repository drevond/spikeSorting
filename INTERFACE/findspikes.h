#ifndef FINDSPIKES_H
#define FINDSPIKES_H
#include <QWidget>
#include <QtWidgets>

class FindSpikes : public QWidget
{    
    Q_OBJECT
public:
    explicit FindSpikes(QWidget *parent = 0);
    
    QList<QVector<double> *> Spikes;
signals:
    void emitNewSpike(QVector<double> Spike);
public slots:
    void GenerateSpikes();
    void clearSpikes();

private :
    QDateTime now;
    int nbChannels;
    int nbGenerated;
    QTextEdit *teValues;
    QLabel *LabelNbSpikes;
    QPushButton *btGenerateSpike;
    QGridLayout *layoutSpike;

};

#endif // FINDSPIKES_H
