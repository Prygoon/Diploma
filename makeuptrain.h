#ifndef MAKEUPTRAIN_H
#define MAKEUPTRAIN_H
#include <QSet>
#include "railcar.h"

class MakeUpTrain
{
private:
    QSet<Railcar> carriages;

public:
    MakeUpTrain(QSet<Railcar> carriages);
    void addRailcar(const Railcar &railcar);
};

#endif // MAKEUPTRAIN_H
