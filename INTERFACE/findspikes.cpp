#include "findspikes.h"
#include "time.h"

FindSpikes::FindSpikes(QWidget *parent) :
    QWidget(parent)
{
    nbChannels=12;
    nbGenerated=0;
    btGenerateSpike=new QPushButton("Generate Spikes");
    teValues=new QTextEdit;
    layoutSpike=new QGridLayout;
    LabelNbSpikes=new QLabel("nombre de spikes");
    layoutSpike->addWidget(btGenerateSpike,1,1);
    layoutSpike->addWidget(LabelNbSpikes,2,1);
    layoutSpike->addWidget(teValues,1,2);
    setLayout(layoutSpike);
    connect(btGenerateSpike,SIGNAL(clicked()),this,SLOT(GenerateSpikes()));
}

void FindSpikes::GenerateSpikes()
{
    qsrand(QTime::currentTime().msec());
    QVector<double> *randomData=new QVector<double>;//durée spikes 6.5s=130points (/10 ici)
    //add spike to clusters
    int number=1;
    for(int spike=0;spike<number;spike++)
    {
        for(int channel=0;channel<nbChannels;channel++)
        {
            //generate spikes randomly
            randomData->clear();
            for(int i=0;i<13*nbChannels;i++)//generate random values of spike
            {
                int value=rand()%100;
                randomData->append(value);
                //teValues->append(QString::number(value));
            }
        }
        nbGenerated++;
        Spikes.append(randomData);
    }
    LabelNbSpikes->setText(QString::number(nbGenerated));
}

void FindSpikes::clearSpikes()
{
    Spikes.clear();
    nbGenerated=0;
}
