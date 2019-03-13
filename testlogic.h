#ifndef TESTLOGIC_H
#define TESTLOGIC_H

#include <QWidget>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVector>
#include <math.h>

// ИНФО ЛОКОМОТИВ, ИЗ БАЗЫ написано капсом специально для дебилов
#define CALC_THUST_FORCE  992000
#define CONSTRUCTION_VELOCITY  100
#define CALC_VELOCITY  24.6 // CALC_VELOCITY
#define MASS 552


#define trackCount 3 // количество участков для проверки расчетного
#define massAccuracy  50 // шаг снижения массы для рассчетов
#define lenStation 0 // приемно-отправочные пути, если задано
#define EPS 1e-12  // Его не надо подписывать
#define testSpeed 70 // временно для проверки
#define railcarsTypeCount 2 // количество типов вагонов


namespace Ui {
class TestLogic;
}

class TestLogic : public QWidget
{
    Q_OBJECT

public:
    explicit TestLogic(QWidget *parent = nullptr);
    ~TestLogic();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestLogic *ui;

    const double g = 9.81;

    QStandardItemModel *model;
    QStandardItem *item;

    QVector <QVector <double>> arCalcTrack;
    double trainMass; // масса состава
    double mainIp; //расчетный подъем
    QVector <double> calcVelParam; // кусок таблицы для расчетной скорости
    QVector <double> fW0; // столбец таблицы, тяга
    QVector <double> w0xbt; // столбец таблицы, торможение

    QVector <double> pointS;
    QVector <double> pointV;

    double bigTable[11][15];  // таблица


    // массив для пути
    QVector <QVector <double> > arTrack;
    QVector <double> arIp;
    QVector <double> arLen;


    // где-то тут должен быть массив данных о вагончиках
    QVector <double> railcarsCount;  // пока только количество
    QVector <double> railcarsMass; // и масса
    QVector <double> railcarsProportions;  // и пропорции понадобились
    // добавить длину как минимум, мб и коэффициенты сюда, но не факт

    double w0ll(const double v); // основное удельное сопротивление поезда
    double w0l(const double v); // основное удельное сопротивление локомотива
    double lenTrain(const double Q); // длинна поезда
    double pathSum (const double vMax, const double vMin, const double ip);  // пройденный путь для крутых
    double pathPoint (const double vMax, const double vMin, const double Fwosrip);  // точки

    QVector<double> tableS (double trainMass, int locoMass); // вектор

    void calcVelParamUpdate();
    void railcarsCountUpdate();
    void trainMassUpdate();
    void showTable();

    void setValues();

};

#endif // TESTLOGIC_H
