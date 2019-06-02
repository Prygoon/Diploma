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
#define EPS 1e-12  // Его не надо подписывать




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

    // вспомогательные, коэффициенты
    QVector <QVector <double>> arCalcTrack; // массив для определения расчетного подъема
    QVector <double> calcVelParam; // кусок таблицы для расчетной скорости

    QVector <double> fW0; // столбец таблицы, тяга
    QVector <double> w0xbt; // столбец таблицы, торможение

    QVector <double> fW0Fin; // удельная тяга
    QVector <double> w0xbtFin; // удельное торможение
    QVector <double> w0xFin; // удельный хх

    double okr = 0; //  расчетный тормозной коэффициент состава

    int lenghtRailcars[2] = {15, 20}; // Длины вагонов
    double stepV ; //шаг скоростей
    double lenStation; // приемно-отправочные пути, если задано

    double k_hh ;    // коэффициенты для
    double a_hh ;  // основого удельного на ХХ
    double b_hh ;  // постоянные или меняются (???)

    double k_tt ;    // коэффициенты для
    double a_tt ;  // основого удельного торможение
    double b_tt ;  // как минимум 2 вида под колодки

    double k_okr; // коэффициент, расчетные силы нажатия тормозных колодок, чугун или композит


    int testSpeed; // временно для проверки



    // расчитываем, можно выводить
    double trainMass; // масса состава
    double mainIp; //расчетный подъем
    double distanse;

    double littleTable[30][15];  // таблица, основные значения.  30  от балды, с запасом)

    QVector <double> pointS; // график, путь
    QVector <double> pointV; // график, скорость
    QVector <double> pointT; // график, время


  //  QVector <double> pointF; // график, тяга
 //   QVector <double> pointVF; // график, скорость к тяге


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
    double locoLen; // Длина локомотива
    QVector <double> locoTractionThrust; // тяга
    QVector <double> locoTractionVelocity; // скорость


    // расчеты пошли
    double lagranz(QVector<double> X, QVector<double> Y, double t);
    double w0ll(const double v); // основное удельное сопротивление поезда
    double w0l(const double v); // основное удельное сопротивление локомотива
    double lenTrain(const double Q); // длинна поезда
    double okrUpdate(double trainMass); // расчетный тормозной коэффициент состава
    double pathSum (const double vMax, const double vMin, const double ip);  // пройденный путь для крутых
    double pathPoint (const double vMax, const double vMin, const double Fwosrip);  // точки пути
    double timePoint (const double vMax, const double vMin, const double Fwosrip); // точки времени

    QVector<double> littleTableW0 (double trainMass, int locoMass1); // вектор, переименовать бы

    void FinalTable(double currentV); // формирование  подробных данных по тяге и удельным
    void calcVelParamUpdate();
    void railcarsCountUpdate();
    //void showTable();


    void setValues(); // установить исходные данные
    void setCoeffitient();

};

#endif // LOGIC_H
