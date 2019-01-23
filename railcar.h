#ifndef RAILCAR_H
#define RAILCAR_H

#include <QDebug>


class Railcar
{
private:
    int id;
    const QString *TYPE; //Тип вагона
    const int AXLE_COUNT; //Количество осей вагона
    const int MASS; //Масса вагона
    double k; //
    double a; //Коэффициенты для расчета
    double b; //удельного сопротивления
    double c; //
    const double PROPORTION; //доля вагонов данного типа

public:
    Railcar(QString *type, int mass, int axleCount, double proportion);

    Railcar(const Railcar &other);
    Railcar &operator=(const Railcar &other);
    bool operator==(const Railcar &other) const;


    double unitMotionResist(int velocity); //Удельное сопротивление движению w0" = k + (a + b * v + c * v * v)/(mass/axelCount)
    QString getType() const;
};

inline uint qHash(const Railcar &key, uint seed) {
    return qHash(key.getType(), seed);
}

/*
QDebug operator<<(QDebug dbg, const Railcar &data);*/
#endif // RAILCAR_H
