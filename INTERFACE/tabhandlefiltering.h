#ifndef TABHANDLEFILTERING_H
#define TABHANDLEFILTERING_H
#include <QtWidgets>

class tabHandleFiltering : public QWidget
{
    Q_OBJECT
public:
    explicit tabHandleFiltering(QWidget *parent = 0);

signals:
    void EmitApplyFilter(int beginingTime,int duration,int nbFilter);
public slots:    
    void changeTimeBegin();
    void changeDuration();
    void slotSendInfoForFiltering();
    void slotFinishFiltering();

private:
    QGridLayout *mainLayout;
    QTextEdit *teShowFiles;
    QFile *FileHandleFiltering;
    QTextStream *FluxHandleFiltering;
    QPushButton *btFilter;
    QPushButton *btchangeTime;
    QPushButton *btchangeDuration;
    QSpinBox *spFilter;
    QLabel *labFilter;
    int TimeBegin;
    int Duration;
    QLabel *labTime;
    QLabel *labDuration;
    void ReadHandleFile();

};

#endif // TABHANDLEFILTERING_H
