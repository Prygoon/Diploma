#ifndef TRAIN_H
#define TRAIN_H
#include <QSet>
#include "railcar.h"
#include "locomotive.h"

class Train
{
private:
    Locomotive *locomotive;
    QMap<Railcar, double> *carriages;

public:
    Train(Locomotive *locomotive, QMap<Railcar, double> *carriages);
    Train(Train &other);

    void addRailcar(const Railcar &railcar, const double percent);
};

#endif // TRAIN_H
