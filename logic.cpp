#include "logic.h"

Logic::Logic(QObject *parent, const QJsonObject *dataJson) :
    QObject(parent),
    dataJson(dataJson)
{
    qDebug() << dataJson->value("railcars").toObject();
}

void Logic::setValues()
{
    // чистим, если вдруг что затесалось. остальное тоже можно почистить, мб понадобится при перерасчетах
    arIp.clear();
    arLen.clear();


    /* Локомотив */
    QJsonObject localLocomotiveJson = dataJson->value("locomotive").toObject();
    qDebug() << localLocomotiveJson;
    locoCalcThrustForce = localLocomotiveJson.value("calc_thrust_force").toInt() * 1000; // Расчетная сила тяги, перевод
    locoMass = localLocomotiveJson.value("mass").toInt(); // Масса
    locoConstrVelocity = localLocomotiveJson.value("construction_velocity").toInt(); // Конструкционная скорость
    locoCalcVelocity = localLocomotiveJson.value("calc_velocity").toDouble(); // Расчетная скорость
    locoLen = localLocomotiveJson.value("length").toDouble(); // Длина

    QJsonArray localThrustForceArray = localLocomotiveJson.value("traction").toObject().value("thrust_force").toArray();
    QJsonArray localVelocityArray = localLocomotiveJson.value("traction").toObject().value("velocity").toArray();
    int n = localThrustForceArray.size();
    for (int i = 0; i < n; i++) {
        locoTractionThrust.push_back(localThrustForceArray.at(i).toDouble());
        locoTractionVelocity.push_back(localVelocityArray.at(i).toDouble());
    }

    //locoTractionThrust = {1458.0,1336.0,1154.0,830.0,613.0,491.0,410.0,351.0,312.0,274.0,239.0};
    //locoTractionVelocity = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    /* Вагоны */
    QJsonObject localRailcarsJson = dataJson->value("railcars").toObject();
    QJsonArray localRailcarsTypes = localRailcarsJson.value("types").toArray();
    QVector<QVector<QVariant>> localCoefs; // Массив массивов коэфициентов
    foreach (QJsonValue railcarType, localRailcarsTypes) {
        railcarAxleCounts.push_back(railcarType.toObject().value("axle_count").toInt());
        railcarMasses.push_back(railcarType.toObject().value("mass").toInt());
        railcarPercents.push_back(railcarType.toObject().value("percent").toDouble());
        localCoefs.push_back(railcarType.toObject().value("coefs").toArray().toVariantList().toVector());
    }

    foreach (QVector<QVariant> item, localCoefs){
        railcarCoefsK.push_back(item[0].toDouble());
        railcarCoefsA.push_back(item[1].toDouble());
        railcarCoefsB.push_back(item[2].toDouble());
        railcarCoefsC.push_back(item[3].toDouble());
    }

    /* Профиль пути*/
    QJsonObject localTrackSectionsJson = dataJson->value("trackSection").toObject();
    QVector<QVariant> localSlopes = localTrackSectionsJson.value("slopes").toArray().toVariantList().toVector(); // Уклоны
    QVector<QVariant> localLengths = localTrackSectionsJson.value("lengths").toArray().toVariantList().toVector(); // Длины
    QVector<QVariant> localCurveLengths = localTrackSectionsJson.value("curve_lengths").toArray().toVariantList().toVector(); // Длины кривых
    QVector<QVariant> localCurveRadiuses = localTrackSectionsJson.value("curve_radiuses").toArray().toVariantList().toVector(); // Радиусы кривых

    // надо так же для курвы, но пока ее нет
    foreach (QVariant item, localSlopes){
        arIp.push_back(item.toDouble());
    }
    foreach (QVariant item, localLengths){
        arLen.push_back(item.toDouble());
    }

    // рандомный ввод профиля для теста
  /*  arIp.push_back(0);
    arLen.push_back(6000);
    for (int i = 1; i < 7; i++){
        arIp.push_back((qrand() % 160 - 52) * 0.1);
        arLen.push_back(qrand() % 15000);
    }
    arIp.push_back(6);
    arLen.push_back(6000);
    arIp.push_back(0);
    arLen.push_back(2500);*/
    // конец рандомного ввода, внести из исходных данных

    arTrack.push_back(arIp);
    arTrack.push_back(arLen);
    qDebug() << arTrack;



}

void Logic::setCoeffitient()
{
  //  bool Kolodki;

    // FIXME
    stepV = 0.5;
    testSpeed = 50;
    lenStation = 0;

    k_hh =  2.4;    // коэффициенты для
    a_hh =  0.011;  // основого удельного на ХХ
    b_hh = 0.0003;  // постоянные или меняются (???)

    k_tt =  0.36;    // коэффициенты для
    a_tt =  150;  // основого удельного торможение
    b_tt = 2;  // как минимум 2 вида под колодки

     k_okr = 41.5; // коэффициент, расчетные силы нажатия тормозных колодок, чугун или композит

     locoA = 1.9;   // какие-то коэффициенты для
     locoB = 0.01;  // основное удельное сопротивление локомотива
     locoC = 0.0003; //

}


void Logic::onCalcSignalReceived()
{
    setValues();
    setCoeffitient();

    // НАХОЖДЕНИЕ РАСЧЕТНОГО НАЧАЛО
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
    w0lCurrent = w0l(locoCalcVelocity);
    //qDebug() << w0lCurrent;

    // ------ //
    double w0llCurrent; //  основное удельное сопротивление состава при расчетной скорости (потом нет)
    w0llCurrent = w0ll(locoCalcVelocity);

    // пока без алгоритма упрощения
    double Q[trackCount]; // масса для проверки
    double ip; // крутизна расчетного

    double qMax = 0.0;

    // вот тут мы ищем, какой из отобранных ранее нам подходит..
    for (int experiment = 0; experiment < trackCount; experiment++)
    {
        qDebug() << "-----EXPIRIENCE-----" << experiment;

        ip = arCalcTrack[1][experiment];
        trainMass = (locoCalcThrustForce - locoMass * g * (w0lCurrent + ip)) / ((ip + w0llCurrent) * g);
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


                    fW0 = littleTableW0(trainMass);
                    calcVelParamUpdate();
                    //  qDebug() << "calcVelParam= " << calcVelParam;

                    double minSpeed = ceil(locoCalcVelocity / 10) * 10;  // для пути
                    double S = pathSum(minSpeed, locoCalcVelocity, isec);
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
    qDebug() << "Ip[" << numIp << "]=" << arIp[numIp];  // расчетный, убрать дебаг, вывести в окне
    // вот тут нашли, запомнили его
    // НАХОЖДЕНИЕ РАСЧЕТНОГО КОНЕЦ



    // обновим данные таблицы, это мелкая, ее можно вывести

    trainMass = maxQ;
    qDebug() << "trainMass" << trainMass ; // масса, убрать дебаг, выевсти в окне
    fW0 = littleTableW0(trainMass);

    double maxSpeed = 90; // пока без решения тормозной задачи, эта скорость ее итог

    // грустно считаем весь путь, сумма.
    distanse = 0;
    for (int i  =0; i < arLen.length(); i++) {
        distanse += arLen[i];
    }
    qDebug() << "Весь путь" << distanse ;  // убрать дебаг, вывести в окне

    mainIp = arIp[numIp];



    pointS.push_back(0);
    pointV.push_back(0);

    // какой-то большой Лагр...
    okrUpdate(trainMass);

    // до 15


    for (int i= 0; i < 2; i++)
    {
        partlocoTractionThrust.push_back(locoTractionThrust[i]);
        partlocoTractionVelocity.push_back(locoTractionVelocity[i]);
    }
    /*for (double k = partlocoTractionVelocity[0] ; k < partlocoTractionVelocity[1]; k = k+stepV){
        FinalTable(k);
    }*/

    int s = 1 ; // s = speed
   // int deb = 0;

   // qDebug() << deb++ << "s=" << s << pointVF ;
    while (locoTractionVelocity[s] <= 15)
    {
        for (double k = partlocoTractionVelocity[0] ; k < partlocoTractionVelocity[1]; k = k+stepV){
            FinalTable(k);
        }
        s++;
        partlocoTractionThrust.remove(0);
        partlocoTractionVelocity.remove(0);
        partlocoTractionThrust.push_back(locoTractionThrust[s]);
        partlocoTractionVelocity.push_back(locoTractionVelocity[s]);
       // qDebug() << deb++ << "s=" << s << pointVF ;
    }



    partlocoTractionThrust.clear();
    partlocoTractionVelocity.clear();

    // от 15 до 3 точки
    for (int i= s - 1; i < s + 3; i++)
    {
        partlocoTractionThrust.push_back(locoTractionThrust[i]);
        partlocoTractionVelocity.push_back(locoTractionVelocity[i]);
    }

    for (double k = partlocoTractionVelocity[0] ; k < partlocoTractionVelocity[3]; k = k + stepV){
        FinalTable(k);
    }

   // qDebug() << deb++ << "s=" << s << pointVF ;

    s = s + 3;

    for (int i = s; i < locoTractionThrust.count(); i++)
    {
        //if (i != s) {
            partlocoTractionThrust.remove(0);
            partlocoTractionVelocity.remove(0);
            partlocoTractionThrust.push_back(locoTractionThrust[i]);
            partlocoTractionVelocity.push_back(locoTractionVelocity[i]);
      //      qDebug() << deb++ << "s=" << s << pointVF ;
       // }
        for (double k = partlocoTractionVelocity[2] ; k < partlocoTractionVelocity[3]; k = k + stepV){
            FinalTable(k);
        }
    }

    // до конца
    for (double k = partlocoTractionVelocity[3] ; k < locoConstrVelocity; k = k + stepV){
        FinalTable(k);
    }
    //qDebug () << pointVF;
    // конец большого Лагра



    // вспомогательные
    double S = 0; // прохождение всего пути
    double currentS = 0; // прохождение участка пути
    double currentSpeed = 0;
    int currentSector = 0;
    double addPoint;
    double addTimePoint;
    double FwosrIp;


    QVector <double> deltaFW0;
    QVector <double> deltaw0xbt;
    QVector <double> xSpeed;

    double currentTime = 0;
    pointT.push_back(0);
    int moveMode = 0; // режим движения. 0 - тяга, 1 - ХХ 2 -  тормоз.
    qDebug() << w0xFin ;

    // построение из расчетов, что всегда премся в тяге :)
    do {
        currentS = 0;
        do {
            if ((currentSpeed + stepV) > maxSpeed) {
                moveMode = 1;
            }
            // режим движения
            switch (moveMode) {
            case 0:
                FwosrIp = fW0Fin[abs(static_cast<int>(currentSpeed/stepV))] - arIp[currentSector];
                break;
            case 1:
                FwosrIp = w0xFin[abs(static_cast<int>(currentSpeed/stepV))] - arIp[currentSector];
                if (FwosrIp >= 0) {
                    moveMode = 2;
                    FwosrIp = w0xbtFin[abs(static_cast<int>(currentSpeed/stepV))] - arIp[currentSector];
                }
               // qDebug() << FwosrIp << currentSpeed;
                if (currentSpeed <= maxSpeed - 10) {
                    moveMode = 0;
                }
                break;
            case 2:
                FwosrIp = w0xbtFin[abs(static_cast<int>(currentSpeed/stepV))] - arIp[currentSector];
               // qDebug() << FwosrIp << currentSpeed;
                if (currentSpeed <= maxSpeed - 10) {
                    moveMode = 0;
                }
                break;
            default:
                FwosrIp = fW0Fin[abs(static_cast<int>(currentSpeed/stepV))] - arIp[currentSector];
                break;
            }


            if (FwosrIp > 0) {
                stepV = fabs(stepV);
            } else {
                stepV = -fabs(stepV);
            }

            /*if ((currentSpeed + stepV) > maxSpeed) {
                moveMode = 1;
                //currentSpeed -= stepV;
                //addPoint = pathPoint(currentSpeed, currentSpeed, FwosrIp);
                //currentS = arLen[currentSector];
            } // else {*/
                addPoint = pathPoint(currentSpeed + stepV, currentSpeed, FwosrIp);
                addTimePoint = timePoint(currentSpeed + stepV, currentSpeed, FwosrIp);
                currentSpeed += stepV;
                currentS += addPoint;
                currentTime += addTimePoint;
                if (currentTime > 100) {
                    currentTime += -100;
                }
          //  }


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
        moveMode = 0;
        //  qDebug() << "S" << S << distanse << currentSector;
    } while (S < distanse);




 //   qDebug() << "S" << pointS;
//   qDebug() << "T" << pointT;
    // закончили построение.







    pointSTor.push_back(distanse);
    pointVTor.push_back(0);

    // построение отрезка торможения
    S = 0;
    currentS = 0;
    currentSector = arIp.count() - 1;
    currentSpeed = 0;
    stepV = 10;


    do {

        FwosrIp = - w0xbtFin[abs(static_cast<int>(currentSpeed/stepV))] + arIp[currentSector];
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

QVector<double> Logic::getW0xFin() const
{
    return w0xFin;
}

QVector<double> Logic::getW0xbtFin() const
{
    return w0xbtFin;
}

QVector<double> Logic::getFW0Fin() const
{
    return fW0Fin;
}

QVector<double> Logic::getPointVF() const
{
    return pointVF;
}

QVector<double> Logic::getPointF() const
{
    return pointF;
}

double Logic::getLocoCalcVelocity() const
{
    return locoCalcVelocity;
}

int Logic::getLocoConstrVelocity() const
{
    return locoConstrVelocity;
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


    double w = 0; // считаем основное удельное сопротивление
    double qUnit; // масса, приходящаяся на одну колесную пару

    // FIXME цикл для каждого из вагонов
    for (int i = 0; i < railcarAxleCounts.count(); i++){
        qUnit = railcarMasses[i] / railcarAxleCounts[i];
        w = w + railcarPercents[i] * (railcarCoefsK[i] + (railcarCoefsA[i] + railcarCoefsB[i] * v + railcarCoefsC[i] * v * v) / qUnit);
    }

    return w;
}

double Logic::w0l(const double v)
{

    return (locoA + locoB * v + locoC * v * v);
}

double Logic::lenTrain(const double Q)
{

    int currentlen ;

    double lenghtTrain = locoLen + 10; // +10 - запас длины на неточность установки
    for (int i = 0; i < railcarAxleCounts.count(); i++) {
        if (railcarAxleCounts[i] == 4) {
            currentlen = lenghtRailcars[0];
        } else {
            currentlen = lenghtRailcars[1];
        }
        lenghtTrain += floor(railcarPercents[i] * Q / railcarMasses[i]) * currentlen; // вагоны не дробные, округляем-с
    }
    return lenghtTrain;
}

double Logic::okrUpdate(double trainMass)
{
    okr = 0;
    for (int i = 0; i < railcarAxleCounts.count(); i++)
    {
     okr += railcarAxleCounts[i] * railcarCount[i];
    }
    okr = k_okr * okr / trainMass / g;

}

double Logic::pathSum(const double vMax, const double vMin, const double ip)
{

    double Fwosr; // для расчетов, не знаю, как обозвать
    int numMax = static_cast<int>(floor(vMax / 10));
    int numMin = static_cast<int>(floor(vMin / 10));


    if (fabs(vMin - locoCalcVelocity) < EPS) {
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

QVector<double> Logic::littleTableW0(double trainMass)
{
    QVector <double> fW0cur;
    QVector <double> w0xbtcur;

    okrUpdate(trainMass);

    // построим табличку, см эксель

    for (int i = 0; i < locoConstrVelocity/10; i++) {
        littleTable[i][0] = i  * 10;
        littleTable[i][1] = lagranz(locoTractionVelocity, locoTractionThrust, littleTable[i][0]);

        // тяга
        littleTable[i][2] = w0l(littleTable[i][0]) * g * locoMass;
        littleTable[i][3] = w0ll(littleTable[i][0]) * g * trainMass;
        littleTable[i][4] = littleTable[i][2] + littleTable[i][3];
        littleTable[i][5] = 1000 * littleTable[i][1] - littleTable[i][4];
        littleTable[i][6] = littleTable[i][5] / ((locoMass+trainMass) * g);
        fW0cur.push_back(littleTable[i][6]); // в методичке написано, что это для диаграммы удельных, тяга
        // ХХ
        littleTable[i][7] = k_hh + a_hh * littleTable[i][0] + b_hh * littleTable[i][0] * littleTable[i][0];
        littleTable[i][8] = littleTable[i][7] * g * locoMass;
        littleTable[i][9] = littleTable[i][3] + littleTable[i][8];
        littleTable[i][10] = littleTable[i][9] / ((locoMass + trainMass) * g); // вторая для диаграммы
        // торможение
        littleTable[i][11] = k_tt * ((littleTable[i][0] + a_tt) / (b_tt * littleTable[i][0] + a_tt));
        littleTable[i][12] = okr * littleTable[i][11] * 1000;
        littleTable[i][13] = littleTable[i][10] + 0.5 * littleTable[i][12];
        littleTable[i][14] = littleTable[i][10] + littleTable[i][12];
        w0xbtcur.push_back(littleTable[i][14]);

    }

    w0xbt = w0xbtcur;
    return fW0cur;
}

void Logic::FinalTable(double currentV)
{
    double calcMode[7];

    pointVF.push_back(currentV);

    calcMode[0] = lagranz(partlocoTractionVelocity, partlocoTractionThrust, currentV);

    pointF.push_back(calcMode[0]);

    calcMode[4] = w0ll(currentV) * g * trainMass;
    calcMode[1] = 1000 * calcMode[0] - (w0l(currentV) * g * locoMass + calcMode[4]);
    calcMode[1] = calcMode[1]/((locoMass+trainMass) * g);

    fW0Fin.push_back(calcMode[1]);

    calcMode[2] = calcMode[4] + (k_hh + a_hh * currentV + b_hh * currentV * currentV) * g * locoMass ;
    calcMode[2] = calcMode[2] / ((locoMass + trainMass) * g);

    w0xFin.push_back(-calcMode[2]);

    calcMode[3] = 1000 * okr * (k_tt * ((currentV + a_tt) / (b_tt * currentV + a_tt)));
    calcMode[5] = calcMode[2] + 0.5 * calcMode[3];
    calcMode[6] = calcMode[2] + calcMode [3];

    w0xbtFin.push_back(-calcMode[5]);
}

void Logic::calcVelParamUpdate()
{
    // параметры для расчетной скорости
    calcVelParam.clear();
    calcVelParam.push_back(locoCalcVelocity);
    calcVelParam.push_back(locoCalcThrustForce);
    calcVelParam.push_back(w0l(locoCalcVelocity) * g * locoMass);
    calcVelParam.push_back(w0ll(locoCalcVelocity) * g * trainMass);
    calcVelParam.push_back(calcVelParam[2] + calcVelParam[3]);
    calcVelParam.push_back(1000*calcVelParam[1] - calcVelParam[4]);
    calcVelParam.push_back(calcVelParam[5] / ((locoMass + trainMass) * g));

}

void Logic::railcarsCountUpdate()
{
    railcarCount.clear();
    for (int i = 0; i < railcarAxleCounts.count(); i++) {
        railcarCount.push_back(static_cast<int>(floor(railcarPercents[i] * trainMass / railcarMasses[i]))); // вагоны не дробные, округляем-с
    }
    qDebug() << "Количество вагонов" << railcarCount;
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



double Logic::getDistanse() const
{
    return distanse;
}

