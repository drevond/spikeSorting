#ifndef FONCTIONSUTILES_H
#define FONCTIONSUTILES_H
#include <QtWidgets>
class FonctionsUtilesI
{
public:
    FonctionsUtilesI();
    static void fillItems(int number, QTableWidget *listInit, QList<int> listIndex);
    static int* findNbRowCol(int number);
    static int max(int a,int b);
};

#endif // FONCTIONSUTILES_H
