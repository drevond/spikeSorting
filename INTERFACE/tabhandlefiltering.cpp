#include "tabhandlefiltering.h"

tabHandleFiltering::tabHandleFiltering(QWidget *parent) :
    QWidget(parent)
{
    mainLayout=new QGridLayout;
    teShowFiles=new QTextEdit;
    FileHandleFiltering=new QFile("textFile handle filtering.txt");
    FluxHandleFiltering=new QTextStream(FileHandleFiltering);
    btFilter=new QPushButton("Apply Filtering");
    TimeBegin=0;//en sec
    Duration=100;//en msec
    labTime=new QLabel(QString::number(TimeBegin)+"ms");
    labDuration=new QLabel(QString::number(Duration)+"ms");
    btchangeTime=new QPushButton("change Time Begin");
    btchangeDuration=new QPushButton("change duration ");
    spFilter=new QSpinBox;
    labFilter=new QLabel("Choose filter");

    //----------------LAYOUT-----------------
    mainLayout->addWidget(teShowFiles,0,0,3,3);
    mainLayout->addWidget(btFilter,0,3,2,2);
    mainLayout->addWidget(btchangeTime,2,3,1,1);
    mainLayout->addWidget(btchangeDuration,2,4,1,1);
    mainLayout->addWidget(labTime,1,3,1,1,Qt::AlignBottom);
    mainLayout->addWidget(labDuration,1,4,1,1,Qt::AlignBottom);
    mainLayout->addWidget(labFilter,3,2,1,1);
    mainLayout->addWidget(spFilter,3,3,1,1);
    //
    setLayout(mainLayout);

    //connections
    connect(btchangeTime,SIGNAL(clicked()),this,SLOT(changeTimeBegin()));
    connect(btchangeDuration,SIGNAL(clicked()),this,SLOT(changeDuration()));
    connect(btFilter,SIGNAL(clicked()),this,SLOT(slotSendInfoForFiltering()));
}

void tabHandleFiltering::changeDuration()
{
    QString textTime=QInputDialog::getText(this,
                                           "change Time begining",
                                           "enter a valid time in milliseconds to select the duration of the data to be filtered",QLineEdit::Normal);
    Duration=textTime.toInt(0,10);
    labDuration->setText(QString::number(Duration)+"ms");
}

void tabHandleFiltering::changeTimeBegin()
{
    QString textTime=QInputDialog::getText(this,
                                           "change Time begining",
                                           "enter a valid time in milliseconds to select the time of the begining of the data to be filtered",QLineEdit::Normal);
    TimeBegin=textTime.toInt(0,10);
    labTime->setText(QString::number(TimeBegin)+"ms");
}

void tabHandleFiltering::slotSendInfoForFiltering()
{
    btFilter->setEnabled(false);
    EmitApplyFilter(TimeBegin,Duration,spFilter->value());
}

void tabHandleFiltering::slotFinishFiltering()
{
    FileHandleFiltering->open(QIODevice::WriteOnly);
    FileHandleFiltering->seek(FileHandleFiltering->size());
    QString text="Filtering begining at time :\n "+ QString::number(TimeBegin) +"\nending at time : \n "+QString::number(TimeBegin + Duration)+"\n\n";
    FluxHandleFiltering->operator <<(text);
    FileHandleFiltering->close();
    ReadHandleFile();
    btFilter->setEnabled(true);
}

void tabHandleFiltering::ReadHandleFile()
{
    FileHandleFiltering->open(QIODevice::ReadOnly);
    FileHandleFiltering->seek(0);
    QString text=FluxHandleFiltering->readAll();
    teShowFiles->setText(text);
    FileHandleFiltering->close();
}


