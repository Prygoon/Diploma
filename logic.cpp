#include "logic.h"

/*
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

const QString *TYPE; //Тип вагона
const int AXLE_COUNT; //Количество осей вагона
const int MASS; //Масса вагона
double k; //
double a; //Коэффициенты для расчета
double b; //удельного сопротивления
double c; //
const double PROPORTION; //доля вагонов данного типа

*/

Logic::Logic(QObject *parent, const QJsonObject *dataJson) :
    QObject(parent),
    dataJson(dataJson)
{
    qDebug() << dataJson->value("railcars").toObject();
}

void Logic::onCalcSignalReceived()
{
    setValues();


    // тааак. тут ничего интересного, объявляем и ищем три самых жЫрных подъема
    // вспомогательные массивы для нахождения расчетного
    QVector <double> arCalcIp;
    QVector <double> arCalcLen;
    QVector <double> arCalcNum;
    double maxIp;
    int numIp = 0;
    QVector <double> arIpTemp = arIp;


    for (int i = 0; i < trackCount; i++)
    {
        maxIp = -1000;
        for (int j = 0; j < arIpTemp.count(); j++) {
            if (arIpTemp[j] > maxIp){
                numIp = j;
                maxIp = arIpTemp[j];
            }
        }
        arCalcNum.push_back(numIp);
        arCalcIp.push_back(maxIp);
        arCalcLen.push_back(arLen[numIp]);
        arIpTemp[numIp] = 0;
    }
    arCalcTrack.push_back(arCalcNum);
    arCalcTrack.push_back(arCalcIp);
    arCalcTrack.push_back(arCalcLen);
    qDebug() << arCalcTrack;

    // вот тут закончили, выделили три штука.



    // ------- //
    double w0lCurrent; // основное удельное сопротивление локомотива при расчетной скорости (потом нет)
    w0lCurrent = w0l(CALC_VELOCITY);
    //qDebug() << w0lCurrent;

    // ------ //
    double w0llCurrent; //  основное удельное сопротивление состава при расчетной скорости (потом нет)
    w0llCurrent = w0ll(CALC_VELOCITY);

    // пока без алгоритма упрощения
    double Q[trackCount]; // масса для проверки
    double ip; // крутизна расчетного

    double qMax = 0.0;

    // вот тут мы ищем, какой из отобранных ранее нам подходит..
    for (int experiment = 0; experiment < trackCount; experiment++)
    {
        qDebug() << "-----EXPIRIENCE-----" << experiment;

        ip = arCalcTrack[1][experiment];
        trainMass = (CALC_THUST_FORCE - MASS * g * (w0lCurrent + ip)) / ((ip + w0llCurrent) * g);
        trainMass = ceil(trainMass / 50) * 50; // округление до 50

        railcarsCountUpdate();
        // ----- //
        // если задана длина П-О путей
        if (lenStation > 0) {
            double trainLenght;
            trainLenght = lenTrain(trainMass);
            while (lenStation < trainLenght){
                trainMass=trainMass - massAccuracy;
                trainLenght = lenTrain(trainMass);
            }
        }


        // ------- //

        bool checkMass = false; // проверка массы
        do {
            checkMass = true;

            double isec; // крутизна
            double lengsec;  // длинна

            Q[experiment] = trainMass;
            qDebug() << "trainMass = " << trainMass;
            calcVelParamUpdate();


            if (experiment != 0)
            {
                for (int k = 0; k < experiment; k++){
                    isec = arCalcTrack[1][experiment - 1 - k];
                    lengsec = arCalcTrack[2][experiment - 1 - k];
                    // qDebug() << "isec = " << isec;
                    //    qDebug() << "lengsec = " << lengsec;


                    fW0 = tableS(trainMass, MASS);
                    calcVelParamUpdate();
                    //  qDebug() << "calcVelParam= " << calcVelParam;

                    double minSpeed = ceil(CALC_VELOCITY / 10) * 10;  // для пути
                    double S = pathSum(minSpeed, CALC_VELOCITY, isec);
                    //   qDebug() << "Scalc= " << S << "/" << lengsec << "|" << minSpeed;

                    do {
                        if (minSpeed > testSpeed){
                            qDebug() << "Не въедет";
                            trainMass = trainMass - massAccuracy * 10;
                            if (trainMass > qMax)
                            {
                                checkMass = false;
                            } else {
                                Q[experiment] = 0;
                                qDebug() << "Не въедет совсем";
                            }
                            break;
                        }
                        S += pathSum(minSpeed + 10, minSpeed, isec);
                        minSpeed += 10;
                        //      qDebug() << "S= " << S << "/" << lengsec << "|" << minSpeed;
                    } while (S < lengsec);
                    if (S > lengsec)
                    {
                        qMax = trainMass;
                    }
                }
            } else {
                qMax = trainMass;
            }
        } while (checkMass == false);
        qDebug() << "END" << Q[experiment];
    }


    double maxQ = 0;
    // int numIp;
    for (int i = 0; i < trackCount; i++){
        if (maxQ < Q[i])
        {
            numIp = static_cast<int>(arCalcTrack[0][i]);
            maxQ = Q[i];
        }
    }
    qDebug() << "Ip[" << numIp << "]=" << arIp[numIp];
    // вот тут нашли, запомнили его


    // обновим данные таблицы, это финальные, уже для графиков используются. в теории тут уже можно
    // впилить обновленный метод с лагранжем. см. ниже комментарий
    trainMass = maxQ;
    qDebug() << "trainMass" << trainMass ;
    fW0 = tableS(trainMass, MASS);
    //showTable();
    // qDebug() << "Тормозная часть" << w0xbt;



    double maxSpeed = 90; // пока без решения тормозной задачи

    // грустно считаем весь путь, сумма.
    distanse = 0;
    for (int i  =0; i < arLen.length(); i++) {
        distanse += arLen[i];
    }
    qDebug() << "Весь путь" << distanse ;

    mainIp = arIp[numIp];

    double S = 0; // прохождение всего пути
    double currentS = 0; // прохождение участка пути

    pointS.push_back(0);
    pointV.push_back(0);

    // FIXME блок частично надо вытащить в настройки программы. тот же шаг.
    double stepV = 0.01; //шаг скоростей
    double currentSpeed = 0;
    int currentSector = 0;
    double addPoint;
    double addTimePoint;
    double FwosrIp;
    // конец FIXME блока


    QVector <double> deltaFW0;
    QVector <double> deltaw0xbt;
    QVector <double> xSpeed;

   /* for (int i  = 0; i < fW0.length() - 1; i++) {
        deltaFW0.push_back((fW0[i] + fW0[i + 1]) / 2);
        deltaw0xbt.push_back((w0xbt[i] + w0xbt[i + 1]) / 2); // тормоз пусть живет пока
    }*/
    // qDebug() << "Дельта тяга" << deltaFW0 ;
    // qDebug() << "Дельта тормоза" << deltaw0xbt ;

    // мм... а надо ли нам перерасчитывать именно F? Это ж куча формул. Может, попробовать лагранжить тягу?
    // а хотя фиг его знает.. надо прикинуть будет, что проще и дешевле по памяти. по идее один раз хардкорно
    // просчитать всю таблицу и потом обращаться напрямую. а то слишком много лагранжей при большом пути
    // причем которые повторяются.
    // пока для теста просто.

   // qDebug() << "тяга" << fW0 ;
  //  qDebug() << "тормоз" << w0xbt ;


    // задаем массив исков. вроде где-то было, надо перетащить в глобалку и в формирование данных

    for (int i  = 0; i < fW0.length(); i++) {
        xSpeed.push_back(i * 10);
    }

    double currentTime = 0;
    double lagrahzTest = 0; // убрать потом переменную, сразу в формулу можно. для вывода.
    pointT.push_back(0);
    // построение из расчетов, что всегда премся в тяге :)
    do {
        currentS = 0;
        do {
            lagrahzTest = lagranz(xSpeed, fW0, currentSpeed);
           // FwosrIp = deltaFW0[static_cast<int>(floor(currentSpeed / 10))] - arIp[currentSector];
            FwosrIp = lagrahzTest - arIp[currentSector];
            //  qDebug() << "1" << deltaFW0[static_cast<int>(floor(currentSpeed/10))] << FwosrIp;
            if (FwosrIp > 0) {
                stepV = fabs(stepV);
            } else {
                stepV = -fabs(stepV);
            }
            // qDebug() << "2" << stepV;
            if ((currentSpeed + stepV) > maxSpeed) {
                //currentSpeed -= stepV;
                //addPoint = pathPoint(currentSpeed, currentSpeed, FwosrIp);
                currentS = arLen[currentSector];
            } else {
                addPoint = pathPoint(currentSpeed + stepV, currentSpeed, FwosrIp);
                addTimePoint = timePoint(currentSpeed + stepV, currentSpeed, FwosrIp);
                currentSpeed += stepV;
                currentS += addPoint;
                currentTime += addTimePoint;
                if (currentTime > 100) {
                    currentTime += -100;
                }
            }


            pointT.push_back(currentTime);
            pointV.push_back(currentSpeed);
            if (currentS > arLen[currentSector])
            {
                currentS = arLen[currentSector];
            }
            pointS.push_back(currentS + S);

            //   qDebug() << "curS" << currentS << arLen[currentSector] << currentSector;

        } while (currentS < arLen[currentSector]);
        currentSector++ ;
        S += currentS;
        //  qDebug() << "S" << S << distanse << currentSector;
    } while (S < distanse);

   // qDebug() << "S" << pointS;
    //qDebug() << "T" << pointT;
    // закончили построение.


    // ЭТО ВТОРОЙ ГРАФИК, ТОРМОЖЕНИЕ
    QVector <double> pointSTor;
    QVector <double> pointVTor;
    // ВОТ ОН ТУТ

    pointSTor.push_back(distanse);
    pointVTor.push_back(0);

    // построение отрезка торможения
    S = 0;
    currentS = 0;
    currentSector = arIp.count() - 1;
    currentSpeed = 0;
    stepV = 10;


    do {
        lagrahzTest = lagranz(xSpeed, w0xbt, currentSpeed);
        FwosrIp = - lagrahzTest + arIp[currentSector];
       // FwosrIp = - deltaw0xbt[static_cast<int>(floor(currentSpeed / 10))] + arIp[currentSector];
        addPoint = pathPoint(currentSpeed, currentSpeed + stepV, FwosrIp);
        currentSpeed += stepV;
        currentS += addPoint;
        /*   if (arLen[currentSector] > currentS)
        {
           S += currentS;
           currentS = 0;
           currentSector--;
           // not tested :)
        }*/
        pointSTor.push_front(distanse - currentS - S);
        pointVTor.push_front(currentSpeed);
    } while (currentSpeed < maxSpeed);

   // qDebug() << "STor" << pointSTor;
   // qDebug() << "VTor" << pointVTor;

}

QVector<double> Logic::getPointT() const
{
    return pointT;
}

QVector<double> Logic::getPointV() const
{
    return pointV;
}

QVector<double> Logic::getPointS() const
{
    return pointS;
}

double Logic::lagranz(QVector<double> X, QVector<double> Y, double t)
{
    double sum, prod;
    sum = 0;
    for (int j = 0; j < X.size(); j++){
        prod = 1;
        for (int i = 0; i < Y.size(); i++){
            if (i != j) {
                prod = prod * (t - X[i]) / (X[j] - X[i]);
            }
        }
        sum = sum + Y[j] * prod;
    }
    return sum;
}

double Logic::w0ll(const double v)
{
    // из базы вагонов
    int AXLE_COUNT[2] = {4,8}; //Количество осей вагона
    //  int MASSR[2] = {88,176}; //Масса вагона
    double k[2] = {0.7,0.7}; //
    double a[2] = {3,6}; //Коэффициенты для расчета
    double b[2] = {0.1,0.038}; //удельного сопротивления
    double c[2] = {0.0025,0.0021}; //
    double perc[2] = {0.68,0.32}; // PROPORTION;

    double w = 0; // считаем основное удельное сопротивление
    double qUnit; // масса, приходящаяся на одну колесную пару


    // FIXME цикл для каждого из вагонов
    for (int i = 0; i < 2; i++){
        qUnit = railcarsMass[i] / AXLE_COUNT[i];
        w = w + perc[i] * (k[i] + (a[i] + b[i] * v + c[i] * v * v) / qUnit);
    }

    return w;
}

double Logic::w0l(const double v)
{
    double a = 1.9;   // какие-то коэффициенты для
    double b = 0.01;  // основное удельное сопротивление локомотива
    double c = 0.0003; //

    return (a + b * v + c * v * v);
}

double Logic::lenTrain(const double Q)
{
    // из базы вагонов
    //int MASSR[2] = {88, 176}; //Масса вагона
    //double perc[2] = {0.68, 0.32}; // PROPORTION;
    int lenght[2] = {15, 20}; // Длины вагонов, добавить в базу
    // из базы локомотива
    double locoLenght = 74.4; // Длина локомотива

    double lenghtTrain = locoLenght + 10; // +10 - запас длины на неточность установки
    for (int i = 0; i < 2; i++) {
        // подправить, количество отдельный метод
        lenghtTrain += floor(railcarsProportions[i] * Q / railcarsMass[i]) * lenght[i]; // вагоны не дробные, округляем-с
    }
    return lenghtTrain;
}

double Logic::pathSum(const double vMax, const double vMin, const double ip)
{

    double Fwosr; // для расчетов, не знаю, как обозвать
    int numMax = static_cast<int>(floor(vMax / 10));
    int numMin = static_cast<int>(floor(vMin / 10));


    if (fabs(vMin - CALC_VELOCITY) < EPS) {
        Fwosr = (fW0.value(numMax) + calcVelParam[6]) / 2;
    } else {
        Fwosr = (fW0.value(numMax) + fW0.value(numMin)) / 2;
    }
    double path = 4.17 * ((vMin * vMin - vMax * vMax) / (Fwosr - ip));
    path = path < 0 ?  0 : path;

    return path;

}

double Logic::pathPoint(const double vMax, const double vMin, const double Fwosrip)
{
    double path = 4.17 * ((vMax * vMax - vMin * vMin) / (Fwosrip));
    return path;
}

double Logic::timePoint(const double vMax, const double vMin, const double Fwosrip)
{
    double time = (vMax - vMin) / (2 * Fwosrip);
    return time;
}

QVector<double> Logic::tableS(double trainMass, int locoMass)
{
    QVector <double> fW0cur; //FIXME посчитать с помощью функции double TestLogic::lagranz(QVector<double> X, QVector<double> Y, double t)
    QVector <double> w0xbtcur; //FIXME см. fW0cur
    const double k_hh =  2.4;    // коэффициенты для
    const double a_hh =  0.011;  // основого удельного на ХХ
    const double b_hh = 0.0003;  // постоянные или меняются (???)

    const double k_tt =  0.36;    // коэффициенты для
    const double a_tt =  150;  // основого удельного торможение
    const double b_tt = 2;  // как минимум 2 вида под колодки

    double okr = 0; //  расчетный тормозной коэффициент состава
    const double k_okr = 41.5; // коэффициент к нему, расчетные силы нажатия тормозных колодок

    // построим табличку, см эксель

    QVector<double> F = {1458.0,1336.0,1154.0,830.0,613.0,491.0,410.0,351.0,312.0,274.0,239.0};
    QVector<double> V = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    // упрощено
    okr =  (k_okr * 4 * railcarsCount[0] + k_okr * 8 * railcarsCount[1])/trainMass/g;

    for (int i = 0; i < 11; i++) {
        bigTable[i][0] = i  * 10;
        bigTable[i][1] = F[i];
        // тяга
        bigTable[i][2] = w0l(bigTable[i][0]) * g * locoMass;
        bigTable[i][3] = w0ll(bigTable[i][0]) * g * trainMass;
        bigTable[i][4] = bigTable[i][2] + bigTable[i][3];
        bigTable[i][5] = 1000 * bigTable[i][1] - bigTable[i][4];
        bigTable[i][6] = bigTable[i][5] / ((locoMass+trainMass) * g);
        fW0cur.push_back(bigTable[i][6]); // в методичке написано, что это для диаграммы удельных, тяга
        // ХХ
        bigTable[i][7] = k_hh + a_hh * bigTable[i][0] + b_hh * bigTable[i][0] * bigTable[i][0];
        bigTable[i][8] = bigTable[i][7] * g * locoMass;
        bigTable[i][9] = bigTable[i][3] + bigTable[i][8];
        bigTable[i][10] = bigTable[i][9] / ((locoMass + trainMass) * g); // вторая для диаграммы
        // торможение
        bigTable[i][11] = k_tt * ((bigTable[i][0] + a_tt) / (b_tt * bigTable[i][0] + a_tt));
        bigTable[i][12] = okr * bigTable[i][11] * 1000;
        bigTable[i][13] = bigTable[i][10] + 0.5 * bigTable[i][12];
        bigTable[i][14] = bigTable[i][10] + bigTable[i][12];
        w0xbtcur.push_back(bigTable[i][14]);

    }

    w0xbt = w0xbtcur;
    return fW0cur;
}

void Logic::calcVelParamUpdate()
{
    // параметры для расчетной скорости
    calcVelParam.clear();
    calcVelParam.push_back(CALC_VELOCITY);
    calcVelParam.push_back(992);  // F Изменить
    calcVelParam.push_back(w0l(CALC_VELOCITY) * g * MASS);
    calcVelParam.push_back(w0ll(CALC_VELOCITY) * g * trainMass);
    calcVelParam.push_back(calcVelParam[2] + calcVelParam[3]);
    calcVelParam.push_back(1000*calcVelParam[1] - calcVelParam[4]);
    calcVelParam.push_back(calcVelParam[5] / ((MASS + trainMass) * g));

}

void Logic::railcarsCountUpdate()
{
    railcarsCount.clear();
    for (int i = 0; i < railcarsTypeCount; i++) {
        railcarsCount.push_back( floor(railcarsProportions[i] * trainMass / railcarsMass[i])); // вагоны не дробные, округляем-с
    }

    qDebug() << "Количество вагонов" << railcarsCount;
}

void Logic::trainMassUpdate()
{
    // пока пусто
}

//void Logic::showTable()
//{
//    model = new QStandardItemModel();
//    for (int i = 0; i < 15; i++) {
//        for (int j = 0; j < 11; j++) {
//            item = new QStandardItem(QString::number(bigTable[j][i], 'f', 2));
//            model->setItem(j, i, item);
//        }
//    }

//    ui->tableView->setModel(model);
//    //ui->tableView->resizeRowsToContents();
//    //ui->tableView->resizeColumnsToContents();

//    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    //ui->tableView->verticalHeader()->setMinimumSectionSize(50);
//    ui->tableView->horizontalHeader()->setMinimumSectionSize(50);

//}

void Logic::setValues()
{
    /* Локомотив */
    QJsonObject localLocomotiveJson = dataJson->value("locomotive").toObject();
    int localLocoCalcThrustForce = localLocomotiveJson.value("calc_thrust_force").toInt(); // Расчетная сила тяги
    int localLocoMass = localLocomotiveJson.value("mass").toInt(); // Масса
    int localConstrVelocity = localLocomotiveJson.value("construction_velocity").toInt(); // Конструкционная скорость
    double localCalcVelocity = localLocomotiveJson.value("calc_velocity").toDouble(); // Расчетная скорость

    /* Вагоны */
    QJsonObject localRailcarsJson = dataJson->value("railcars").toObject();
    QJsonArray localRailcarsTypes = localRailcarsJson.value("types").toArray();
    QVector<int> localAxleCounts; // Массив количества осей
    QVector<int> localMasses; // Массив масс типов вагонов
    QVector<double> localPercents; // Массив долей вагонов в составе
    QVector<QVector<QVariant>> localCoefs; // Массив массивов коэфициентов
    foreach (QJsonValue railcarType, localRailcarsTypes) {
        localAxleCounts.push_back(railcarType.toObject().value("axle_count").toInt());
        localMasses.push_back(railcarType.toObject().value("mass").toInt());
        localPercents.push_back(railcarType.toObject().value("percent").toDouble());
        localCoefs.push_back(railcarType.toObject().value("coefs").toArray().toVariantList().toVector());
    }

    /* Профиль пути*/
    QJsonObject localTrackSectionsJson = dataJson->value("trackSection").toObject();
    QVector<QVariant> localSlopes = localTrackSectionsJson.value("slopes").toArray().toVariantList().toVector(); // Уклоны
    QVector<QVariant> localLengths = localTrackSectionsJson.value("lengths").toArray().toVariantList().toVector(); // Длины
    QVector<QVariant> localCurveLengths = localTrackSectionsJson.value("curve_lengths").toArray().toVariantList().toVector(); // Длины кривых
    QVector<QVariant> localCurveRadiuses = localTrackSectionsJson.value("curve_radiuses").toArray().toVariantList().toVector(); // Радиусы кривых


    // временно, это надо из базы достать
    double MASSR[railcarsTypeCount] = {88, 176}; //Масса вагона
    double perc[railcarsTypeCount] = {0.68, 0.32}; // PROPORTION;
    // конец временно

    for (int i = 0; i < railcarsTypeCount; i++) {
        railcarsMass.push_back(MASSR[i]);
        railcarsProportions.push_back(perc[i]);
    }
    arIp.clear();
    arLen.clear();
    // пока рандомный ввод для теста
    arIp.push_back(0);
    arLen.push_back(6000);
    for (int i = 1; i < 7; i++){
        arIp.push_back((qrand() % 160 - 52) * 0.1);
        arLen.push_back(qrand() % 17000);
    }
    arIp.push_back(6);
    arLen.push_back(6000);
    arIp.push_back(0);
    arLen.push_back(2500);
    arTrack.push_back(arIp);
    arTrack.push_back(arLen);
    qDebug() << arTrack;
    // конец рандомного ввода, внести из исходных данных


}

double Logic::getDistanse() const
{
    return distanse;
}

