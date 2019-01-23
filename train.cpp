#include "train.h"

Train::Train(Locomotive *locomotive, QSet<Railcar> *carriages) :
    locomotive(locomotive), carriages(carriages)
{

}

Train::Train(Train &other) :
    locomotive(other.locomotive), carriages(other.carriages)
{

}

void Train::addRailcar(const Railcar &railcar) {

    this->carriages->insert(railcar);
}
