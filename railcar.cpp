#include "railcar.h"

int Railcar::getId() const
{
    return id;
}

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

bool Railcar::operator==(const Railcar &other) const {
    return TYPE == other.TYPE;
}

bool operator<(const Railcar &rc1, const Railcar &rc2)
{
    return rc1.getId() < rc2.getId();
}
