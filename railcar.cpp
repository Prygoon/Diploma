#include "railcar.h"

Railcar::Railcar(QString *type, double proportion, int axelCount, double axelLoad) :
    type(type), proportion(proportion), axelCount(axelCount), axelLoad(axelLoad)
{

}

double Railcar::unitMotionResist(int velocity) {
    double result;

    result = this->k + (this->a + this->b * velocity + this->c * velocity * velocity) \
            /(this->axelLoad/this->axelCount); // w0" = k + (a + b * v + c * v * v)/(axelLoad/axelCount)

    return result;
}

QString Railcar::getType() const
{
    return *type;
}

Railcar::Railcar(const Railcar &other){
    type = other.type;
    proportion = other.proportion;
}

/*
Railcar& Railcar::operator=(const Railcar &other){
    Railcar *ret = new Railcar();
    ret->type = other.type;
    return *ret;
}*/

bool Railcar::operator==(const Railcar &other) const{
    return type == other.type;
}

/*
QDebug operator<<(QDebug dbg, const Railcar &data){
    dbg.nospace() << "Railcar(" << data.type << ")";
    return dbg.maybeSpace();
}*/


