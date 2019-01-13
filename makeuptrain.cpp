#include "makeuptrain.h"

MakeUpTrain::MakeUpTrain(QSet<Railcar> carriages) :
    carriages(carriages)
{

}

void MakeUpTrain::addRailcar(const Railcar &railcar) {

    this->carriages.insert(railcar);
}
