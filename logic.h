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

    QVector<double> *getPointS() const;
    QVector<double> *getPointV() const;
    QVector<double> *getPointT() const;
    double getDistanse() const;
    int getLocoConstrVelocity() const;
    double getLocoCalcVelocity() const;
    QVector<double> *getPointF() const;
    QVector<double> *getPointVF() const;
    QVector<double> *getFW0Fin() const;
    QVector<double> *getW0xbtFin() const;
    QVector<double> *getW0xFin() const;
    double getMaxSpeed() const;
    QVector<double> *getPointVHH() const;
    QVector<double> *getPointHH() const;
    QVector<double> *getPointBT() const;
    QVector<double> *getPointVBT() const;
    QVector<int> *getTrackSectionLengths() const;
    QVector<double> *getPointSTor() const;
    QVector<double> *getPointVTor() const;
    QVector<QVector<double> > *getLittleTable_ptr() const;

public slots:
    void onCalcSignalReceived();

private:
    const QJsonObject *dataJson;

    // константы, мб в дефайн
    const double g = 9.81;

    // вспомогательные, коэффициенты
    QVector <QVector <double>> arCalcTrack; // массив для определения расчетного подъема
    QVector <double> calcVelParam; // кусок таблицы для расчетной скорости

    QVector <double> *fW0; // столбец таблицы, тяга
    QVector <double> *w0xbt; // столбец таблицы, торможение

    double okr = 0; //  расчетный тормозной коэффициент состава

    int lenghtRailcars[2] = {15, 20}; // Длины вагонов
    double stepV; //шаг скоростей
    double lenStation; // приемно-отправочные пути, если задано
    double maxSpeed;  // вынести как прямую линию сверху.
    double thrustFuel;
    double noThrustFuel;

    double k_hh;    // коэффициенты для
    double a_hh;  // основого удельного на ХХ
    double b_hh;  // постоянные или меняются (???)

    double k_tt;    // коэффициенты для
    double a_tt;  // основого удельного торможение
    double b_tt;  // как минимум 2 вида под колодки

    double k_okr; // коэффициент, расчетные силы нажатия тормозных колодок, чугун или композит

    double locoA;   // какие-то коэффициенты для
    double locoB;  // основное удельное сопротивление локомотива
    double locoC; //

    double timeThrust;
    double timeAll;

        double FwosrIp; // для расчета пути
            int moveMode = 0; // режим движения. 0 - тяга, 1 - ХХ вниз 2 -  тормоз, 2 - ХХ вверх

    int testSpeed; // временно для проверки

    // расчитываем, можно выводить
    double trainMass; // масса состава
    double mainIp; //расчетный подъем
    double distanse;

    QVector<QVector<double> > littleTable; // таблица, основные значения
    QVector<QVector<double> > *littleTable_ptr;

    QVector <double> *pointS; // график, путь
    QVector <double> *pointV; // график, скорость
    QVector <double> *pointT; // график, время

    // два графика точками
    QVector <double> *pointHH; // график,  холостой
    QVector <double> *pointVHH; // график, скорость к холостому
    QVector <double> *pointBT; // график, торможение
    QVector <double> *pointVBT; // график, скорость к торможению


    QVector <double> *pointF; // график, тяга
    QVector <double> *pointVF; // график, скорость к тяге

    QVector <double> *partlocoTractionThrust; // тяга
    QVector <double> *partlocoTractionVelocity; // скорость

    QVector <double> *fW0Fin; // график, удельная тяга
    QVector <double> *w0xbtFin; // график, удельное торможение
    QVector <double> *w0xFin; // график, удельный хх

    // ЭТО ВТОРОЙ ГРАФИК, ТОРМОЖЕНИЕ
    QVector <double> *pointSTor;
    QVector <double> *pointVTor;
    // ВОТ ОН ТУТ

    // исходные данные
    // вагоны
    QVector<int> *railcarAxleCounts; // Массив количества осей
    QVector<int> *railcarMasses; // Массив масс типов вагонов
    QVector<double> *railcarPercents; // Массив долей вагонов в составе
    QVector<double> *railcarCoefsK; // Коэффициенты
    QVector<double> *railcarCoefsA; // Коэффициенты
    QVector<double> *railcarCoefsB; // Коэффициенты
    QVector<double> *railcarCoefsC; // Коэффициенты
    QVector<int> *railcarCount; // Массив количества вагонов каждого типа

    // массив для пути
    //QVector <QVector <double> > *arTrack; // общий, в целом не нужен
    QVector<double> *slopes; // подъемы
    QVector<int> *trackSectionLengths; // длины

    // локомотив
    int locoConstrVelocity; // Конструкционная скорость
    double locoCalcThrustForce; // Расчетная сила тяги
    double locoMass;  // Масса
    double locoCalcVelocity;  // Расчетная скорость
    double locoLen; // Длина локомотива
    QVector <double> *locoTractionThrust; // тяга
    QVector <double> *locoTractionVelocity; // скорость


    // расчеты пошли
    double lagranz(QVector<double> *X, QVector<double> *Y, double t);
    double w0ll(const double v); // основное удельное сопротивление поезда
    double w0l(const double v); // основное удельное сопротивление локомотива
    double lenTrain(const double Q); // длинна поезда
    double pathSum (const double vMax, const double vMin, const double ip);  // пройденный путь для крутых
    double pathPoint (const double vMax, const double vMin, const double Fwosrip);  // точки пути
    double timePoint (const double vMax, const double vMin, const double Fwosrip); // точки времени

    QVector<double> *littleTableW0 (double trainMass); // вектор, переименовать бы,

    void okrUpdate(double trainMass); // расчетный тормозной коэффициент состава
    void FinalTable(double currentV); // формирование  подробных данных по тяге и удельным
    void moveModeFwosrIp (int moveMode);
    void calcVelParamUpdate();
    void railcarsCountUpdate();
    //void showTable();


    void setValues(); // установить исходные данные
    void setCoeffitient();

};

#endif // LOGIC_H
