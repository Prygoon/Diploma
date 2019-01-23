#ifndef MAKEUPTRAIN_H
#define MAKEUPTRAIN_H
#include <QSet>
#include "railcar.h"
#include "locomotive.h"

class Train
{
private:
    Locomotive *locomotive;
    QSet<Railcar> *carriages;

public:
    Train(Locomotive *locomotive, QSet<Railcar> *carriages);
    Train(Train &other);

    void addRailcar(const Railcar &railcar);
};

#endif // MAKEUPTRAIN_H
