#define g 9.81
#include "locomotive.h"

Locomotive::Locomotive(QString *type, int calcThustForce, int mass, int constructionVelocity, double calcVelocity) :
    TYPE(type),
    CALC_THUST_FORCE(calcThustForce),
    MASS(mass),
    CONSTRUCTION_VELOCITY(constructionVelocity),
    CALC_VELOCITY(calcVelocity)
{
    double currentUnitTractionModeMotionResist;
    double currentTractionModeMotionResist;
    double currentUnitIdleModeMotionResist;
    double currentIdleModeMotionResist;
    double currentAdhesionCoefficient;

    for (int i = 0; i < this->CONSTRUCTION_VELOCITY; i++) {

        this->currentVelocity->push_back(i);

        currentUnitTractionModeMotionResist = 1.9 + 0.01 * i + 0.0003 * i * i; //w_0’ = 1,9 + 0,01v + 0,0003v^2
        this->unitTractionModeMotionResist->push_back(currentTractionModeMotionResist);

        currentTractionModeMotionResist = currentUnitTractionModeMotionResist * this->MASS * g; //W_0' = w_0' * P * g
        this->tractionModeMotionResist->push_back(currentTractionModeMotionResist);

        currentUnitIdleModeMotionResist = 2.4 + 0.011 * i + 0.00035 * i * i; //w_x = 2,4 + 0,011v + 0,00035v^2
        this->unitIdleModeMotionResist->push_back(currentUnitIdleModeMotionResist);

        currentIdleModeMotionResist = currentUnitIdleModeMotionResist * this->MASS * g; //W_x = w_x * P * g
        this->idleModeMotionResist->push_back(currentIdleModeMotionResist);

        currentAdhesionCoefficient = 0.27 * (i + 100)/(5 * i + 100); //φ_кр = 0,27(v + 100)/(5v + 100)
        this->adhesionCoefficient->push_back(currentAdhesionCoefficient);

    }
}

QString Locomotive::getType() const {
    return *TYPE;
}

int Locomotive::getCALC_THUST_FORCE() const
{
    return CALC_THUST_FORCE;
}

int Locomotive::getMASS() const {
    return MASS;
}

double Locomotive::getCALC_VELOCITY() const {
    return CALC_VELOCITY;
}

int Locomotive::getCONSTRUCTION_VELOCITY() const {
    return CONSTRUCTION_VELOCITY;
}
