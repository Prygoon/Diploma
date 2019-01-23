#include "railcar.h"

Railcar::Railcar(QString *type, int mass, int axleCount, double proportion) :
    TYPE(type), AXLE_COUNT(axleCount), MASS(mass), PROPORTION(proportion)
{

}

/* Конструктор копирования*/
Railcar::Railcar(const Railcar &other) :
    TYPE(other.TYPE), AXLE_COUNT(other.AXLE_COUNT), MASS(other.MASS), PROPORTION(other.PROPORTION)
{

}

double Railcar::unitMotionResist(int velocity) {
    double result;

    result = this->k + (this->a + this->b * velocity + this->c * velocity * velocity) \
            / (this->MASS / this->AXLE_COUNT); // w0" = k + (a + b * v + c * v * v)/(axelLoad/axelCount)

    return result;
}

QString Railcar::getType() const
{
    return *TYPE;
}

/*
Railcar& Railcar::operator=(const Railcar &other){
    Railcar *ret = new Railcar();
    ret->type = other.type;
    return *ret;
}*/

bool Railcar::operator==(const Railcar &other) const {
    return TYPE == other.TYPE;
}

/*
QDebug operator<<(QDebug dbg, const Railcar &data){
    dbg.nospace() << "Railcar(" << data.type << ")";
    return dbg.maybeSpace();
}*/


