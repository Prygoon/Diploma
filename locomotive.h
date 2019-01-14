#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H
#include <QString>
#include <QVector>

class Locomotive
{
private:
    QString *type; //Название локомотива
    int mass; //Масса локомотива P
    int constructionVelocity; //Конструкционная скорость локомотива v_констр
    double calcVelocity; //Расчетная скорость локомотива v_р
    QVector<int> currentVelocity; //Текущая скорость локомотива v
    QVector<double> unitTractionModeMotionResist; //Удельное сопротивление движению локомотива в режиме w_0'
    QVector<double> tractionModeMotionResist; //Сопротивление движению локомотива в режиме тяги W_0'
    QVector<double> unitIdleModeMotionResist; //Удельное сопротивление движению локомотива на холостом ходу w_х'
    QVector<double> idleModeMotionResist; //Сопротивление движению локомотива на холостом ходу W_х'
    QVector<double> adhesionCoefficient; //Коэффициент сцепления локомотива φ_кр

public:
    Locomotive(QString *type, int mass, int constructionVelocity, double calcVelocity);

    QString getType() const;
    int getMass() const;
    int getConstructionVelocity() const;
    double getCalcVelocity() const;
};

#endif // LOCOMOTIVE_H
