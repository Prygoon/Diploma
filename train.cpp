#include "train.h"

Train::Train(Locomotive *locomotive, QMap<Railcar, double> *carriages) :
    locomotive(locomotive), carriages(carriages)
{

}

Train::Train(Train &other) :
    locomotive(other.locomotive), carriages(other.carriages)
{

}

void Train::addRailcar(const Railcar &railcar, const double percent) {

    this->carriages->insert(railcar, percent);
}
