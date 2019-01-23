#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H

#include <QVector>

class Locomotive
{
private:
    int id;
    const QString *TYPE; //Название локомотива
    const int CALC_THUST_FORCE; //Расчетная сила тяги локомотива Fкр
    const int MASS; //Масса локомотива P
    const int CONSTRUCTION_VELOCITY; //Конструкционная скорость локомотива v_констр
    const double CALC_VELOCITY; //Расчетная скорость локомотива v_р
    QVector<int> *currentVelocity; //Текущая скорость локомотива v
    QVector<double> *unitTractionModeMotionResist; //Удельное сопротивление движению локомотива в режиме w_0'
    QVector<double> *tractionModeMotionResist; //Сопротивление движению локомотива в режиме тяги W_0'
    QVector<double> *unitIdleModeMotionResist; //Удельное сопротивление движению локомотива на холостом ходу w_х'
    QVector<double> *idleModeMotionResist; //Сопротивление движению локомотива на холостом ходу W_х'
    QVector<double> *adhesionCoefficient; //Коэффициент сцепления локомотива φ_кр

public:
    Locomotive(QString *TYPE, int calcThustForce, int MASS, int CONSTRUCTION_VELOCITY, double CALC_VELOCITY);

    QString getType() const;

    int getMASS() const;
    int getCONSTRUCTION_VELOCITY() const;
    double getCALC_VELOCITY() const;
    int getCALC_THUST_FORCE() const;
};

#endif // LOCOMOTIVE_H
