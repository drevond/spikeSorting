#ifndef STATICGRAPH_H
#define STATICGRAPH_H
#include "qcustomplot.h"
#include <QtWidgets>
class StaticGraph : public QWidget
{
    Q_OBJECT
public:
    StaticGraph();
private:
    int nbChannels;
    int nbChannelsW;
    QGridLayout *mainLayout;
    QCustomPlot *plot;
    QSpinBox *spChannel;
    QSpinBox *sptime1;
    QSpinBox *sptime2;
    QPushButton *btDenoisedTrue;
    QLabel *labDenoised;
    QLabel *labchannel;
    QLabel *labtime;
    QPushButton *btUpdate;
    QVector<double> VecTime;
    QVector<double> VecData;
    bool TrueDenoised;
    void generateTimeVec();

public slots:
    void changePlot();
    void ReadData();
    void slotBtDenoisedTrue();
signals:
    void emitchangeFilter(int nbFilter);

};

#endif // STATICGRAPH_H
