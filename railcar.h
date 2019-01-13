#ifndef RAILCAR_H
#define RAILCAR_H
#include <QString>
#include <QDebug>


class Railcar
{
private:

    QString *type;
    double proportion; //доля вагонов данного типа
    int axelCount;
    double axelLoad; // w0" = k + (a + b * v + c * v * v)/(axelLoad/axelCount)
    double k;
    double a;
    double b;
    double c;

public:
    Railcar(QString *type, double proportion, int axelCount, double axelLoad);

    Railcar(const Railcar &other);
    Railcar &operator=(const Railcar &other);
    bool operator==(const Railcar &other) const;


    double unitMotionResist(int velocity); //Удельное сопротивление движению
    QString getType() const;
};

inline uint qHash(const Railcar &key, uint seed) {
    return qHash(key.getType(), seed);
}

/*
QDebug operator<<(QDebug dbg, const Railcar &data);*/
#endif // RAILCAR_H
