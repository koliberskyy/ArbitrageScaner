#include <QCoreApplication>
#include "arbitragescaner.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ArbitrageScaner scaner;
    return a.exec();
}
