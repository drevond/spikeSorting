#ifndef TEST_H
#define TEST_H
#include "fonctionsutiles.h"
#include <QtWidgets>
#include "CALCUL/librairieAlglib/ap.h"
using namespace alglib;
class test:public QWindow
{
    Q_OBJECT
public:
    test();
    void seeMatrix(real_2d_array matrix,int nColSee,int NrowSee);
    QTextEdit *showMatrix;
    void startTest();
public slots:
    void slotPlot(real_2d_array matrix, int nbrow, int nbcol, QString name);

};

#endif // TEST_H
