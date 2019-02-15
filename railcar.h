#ifndef RAILCAR_H
#define RAILCAR_H

#include <QDebug>
#include <QAbstractItemModel>


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
    inline Railcar operator=(const Railcar &other);
    inline bool operator==(const Railcar& other) const;
    inline bool operator<(Railcar& other) const;


    double unitMotionResist(int velocity); //Удельное сопротивление движению w0" = k + (a + b * v + c * v * v)/(mass/axelCount)
    QString getType() const;
    int getId() const;
};

inline uint qHash(const Railcar &key, uint seed) {
    return qHash(key.getType(), seed);
}

bool operator<(const Railcar& rc1, const Railcar& rc2);

#endif // RAILCAR_H
