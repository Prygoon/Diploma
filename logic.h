#ifndef LOGIC_H
#define LOGIC_H

#include <QWidget>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <math.h>

// ИНФО ЛОКОМОТИВ, ИЗ БАЗЫ написано капсом специально для дебилов
//#define CALC_THUST_FORCE  992000
//#define locoConstrVelocity  100
//#define locoCalcVelocity  24.6 // CALC_VELOCITY
//#define locoMass 552


#define trackCount 3 // количество участков для проверки расчетного
#define massAccuracy  50 // шаг снижения массы для рассчетов
#define lenStation 0 // приемно-отправочные пути, если задано
#define EPS 1e-12  // Его не надо подписывать
#define testSpeed 50 // временно для проверки
#define railcarsTypeCount 2 // количество типов вагонов

class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = nullptr, const QJsonObject *dataJson = nullptr);

    QVector<double> getPointS() const;
    QVector<double> getPointV() const;
    QVector<double> getPointT() const;
    double getDistanse() const;
    int getLocoConstrVelocity() const;
    double getLocoCalcVelocity() const;

public slots:
    void onCalcSignalReceived();

private:
    const QJsonObject *dataJson;

    // константы, мб в дефайн
    const double g = 9.81;


    // фз кто это :)
    QStandardItemModel *model;
    QStandardItem *item;

    // вспомогательные
    QVector <QVector <double>> arCalcTrack; // массив для определения расчетного подъема
    QVector <double> calcVelParam; // кусок таблицы для расчетной скорости
    QVector <double> fW0; // столбец таблицы, тяга
    QVector <double> w0xbt; // столбец таблицы, торможение


    // расчитываем, можно выводить
    double trainMass; // масса состава
    double mainIp; //расчетный подъем
    double distanse;

    double bigTable[11][15];  // таблица, основные значения. подправить размерность

    QVector <double> pointS; // график, путь
    QVector <double> pointV; // график, скорость
    QVector <double> pointT; // график, время



    // исходные данные
    // вагоны
    QVector<int> railcarAxleCounts; // Массив количества осей
    QVector<int> railcarMasses; // Массив масс типов вагонов
    QVector<double> railcarPercents; // Массив долей вагонов в составе
    QVector<double> railcarCoefsK; // Коэффициенты
    QVector<double> railcarCoefsA; // Коэффициенты
    QVector<double> railcarCoefsB; // Коэффициенты
    QVector<double> railcarCoefsC; // Коэффициенты
    QVector<int> railcarCount; // Массив количества вагонов каждого типа

    // массив для пути
    QVector <QVector <double> > arTrack; // общий, в целом не нужен
    QVector <double> arIp; // подъем
    QVector <double> arLen; // длина

    // локомотив
    int locoCalcThrustForce; // Расчетная сила тяги
    int locoMass;  // Масса
    int locoConstrVelocity; // Конструкционная скорость
    double locoCalcVelocity;  // Расчетная скорость


    // расчеты пошли
    double lagranz(QVector<double> X, QVector<double> Y, double t);
    double w0ll(const double v); // основное удельное сопротивление поезда
    double w0l(const double v); // основное удельное сопротивление локомотива
    double lenTrain(const double Q); // длинна поезда
    double pathSum (const double vMax, const double vMin, const double ip);  // пройденный путь для крутых
    double pathPoint (const double vMax, const double vMin, const double Fwosrip);  // точки пути
    double timePoint (const double vMax, const double vMin, const double Fwosrip); // точки времени

    QVector<double> tableS (double trainMass, int locoMass1); // вектор, переименовать бы

    void calcVelParamUpdate();
    void railcarsCountUpdate();
    void trainMassUpdate();
    //void showTable();

    void findMainIp(); // поиск расчетного подъема

    void setValues(); // установить исходные данные

};

#endif // LOGIC_H
