#define g 9.81
#include "locomotive.h"

Locomotive::Locomotive(QString type, int mass, int constructionVelocity, float calcVelocity) {

    double currentUnitTractionModeMotionResist;
    double currentTractionModeMotionResist;
    double currentUnitIdleModeMotionResist;
    double currentIdleModeMotionResist;
    double currentAdhesionCoefficient;

    this->type = type;
    this->mass = mass;
    this->constructionVelocity = constructionVelocity;
    this->calcVelocity = calcVelocity;

    for (int i = 0; i < this->constructionVelocity; i++) {
        this->currentVelocity().push_back(i);

        currentUnitTractionModeMotionResist = 1.9 + 0.01 * i + 0.0003 * i * i; //w_0’ = 1,9 + 0,01v + 0,0003v^2
        this->unitTractionModeMotionResist.push_back(currentTractionModeMotionResist);

        currentTractionModeMotionResist = currentUnitTractionModeMotionResist * this->mass * g; //W_0' = w_0' * P * g
        this->tractionModeMotionResist.push_back(currentTractionModeMotionResist);

        currentUnitIdleModeMotionResist = 2.4 + 0.011 * i + 0.00035 * i * i; //w_x = 2,4 + 0,011v + 0,00035v^2
        this->unitIdleModeMotionResist.push_back(currentUnitIdleModeMotionResist);

        currentIdleModeMotionResist = currentUnitIdleModeMotionResist * this->mass * g; //W_x = w_x * P * g
        this->idleModeMotionResist.push_back(currentIdleModeMotionResist);

        currentAdhesionCoefficient = 0.27 * (i + 100)/(5 * i + 100); //φ_кр = 0,27(v + 100)/(5v + 100)
        this->adhesionCoefficient.push_back(currentAdhesionCoefficient);

    }
}

QString Locomotive::getType() const
{
    return type;
}

int Locomotive::getMass() const
{
    return mass;
}

float Locomotive::getCalcVelocity() const
{
    return calcVelocity;
}

int Locomotive::getConstructionVelocity() const
{
    return constructionVelocity;
}
