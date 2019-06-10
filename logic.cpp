#include "logic.h"

Logic::Logic(QObject *parent, const QJsonObject *dataJson) :
    QObject(parent),
    dataJson(dataJson)
{
    qDebug() << *this->dataJson;
}

void Logic::setValues()
{
    slopes = new QVector<double>();
    trackSectionLengths = new QVector<int>();
    // чистим, если вдруг что затесалось. остальное тоже можно почистить, мб понадобится при перерасчетах
    slopes->clear();
    trackSectionLengths->clear();

    /* Параметры */
    maxSpeed = dataJson->value("params").toObject().value("speed_limit").toDouble();
    lenStation = dataJson->value("params").toObject().value("po_length").toInt();
    if(!(dataJson->value("params").toObject().value("thrust_fuel").isUndefined() && dataJson->value("params").toObject().value("nothrust_fuel").isUndefined())) {
        thrustFuel = dataJson->value("params").toObject().value("thrust_fuel").toDouble();
        noThrustFuel = dataJson->value("params").toObject().value("nothrust_fuel").toDouble();
        fuelMode = true;
    }

    /* Локомотив */
    locoTractionThrust = new QVector<double>();
    locoTractionVelocity = new QVector<double>();

    QJsonObject localLocomotiveJson = dataJson->value("locomotive").toObject();
    //qDebug() << localLocomotiveJson;
    locoCalcThrustForce = localLocomotiveJson.value("calc_thrust_force").toDouble() * 1000; // Расчетная сила тяги, перевод
    locoMass = localLocomotiveJson.value("mass").toDouble(); // Масса
    locoConstrVelocity = localLocomotiveJson.value("construction_velocity").toInt(); // Конструкционная скорость
    locoCalcVelocity = localLocomotiveJson.value("calc_velocity").toDouble(); // Расчетная скорость
    locoLen = localLocomotiveJson.value("length").toDouble(); // Длина

    QJsonArray localThrustForceArray = localLocomotiveJson.value("traction").toObject().value("thrust_force").toArray();
    QJsonArray localVelocityArray = localLocomotiveJson.value("traction").toObject().value("velocity").toArray();
    int n = localThrustForceArray.size();
    for (int i = 0; i < n; i++) {
        locoTractionThrust->push_back(localThrustForceArray.at(i).toDouble());
        locoTractionVelocity->push_back(localVelocityArray.at(i).toDouble());
    }

    if (maxSpeed > locoConstrVelocity) {
        maxSpeed = locoConstrVelocity;
    }

    //locoTractionThrust = {1458.0,1336.0,1154.0,830.0,613.0,491.0,410.0,351.0,312.0,274.0,239.0};
    //locoTractionVelocity = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    /* Вагоны */
    railcarAxleCounts = new QVector<int>();
    railcarMasses = new QVector<int>();
    railcarPercents = new QVector<double>();
    railcarCoefsK = new QVector<double>();
    railcarCoefsA = new QVector<double>();
    railcarCoefsB = new QVector<double>();
    railcarCoefsC = new QVector<double>();

    QJsonObject localRailcarsJson = dataJson->value("railcars").toObject();
    QJsonArray localRailcarsTypes = localRailcarsJson.value("types").toArray();
    QVector<QVector<QVariant>> localCoefs; // Массив массивов коэфициентов
    foreach (QJsonValue railcarType, localRailcarsTypes) {
        railcarAxleCounts->push_back(railcarType.toObject().value("axle_count").toInt());
        railcarMasses->push_back(railcarType.toObject().value("mass").toInt());
        railcarPercents->push_back(railcarType.toObject().value("percent").toDouble());
        localCoefs.push_back(railcarType.toObject().value("coefs").toArray().toVariantList().toVector());
    }

    foreach (QVector<QVariant> item, localCoefs){
        railcarCoefsK->push_back(item.at(0).toDouble());
        railcarCoefsA->push_back(item.at(1).toDouble());
        railcarCoefsB->push_back(item.at(2).toDouble());
        railcarCoefsC->push_back(item.at(3).toDouble());
    }

    /* Профиль пути*/
    QJsonObject localTrackSectionsJson = dataJson->value("trackSection").toObject();
    QVector<QVariant> localSlopes = localTrackSectionsJson.value("slopes").toArray().toVariantList().toVector(); // Уклоны
    QVector<QVariant> localLengths = localTrackSectionsJson.value("lengths").toArray().toVariantList().toVector(); // Длины
    QVector<QVariant> localCurveLengths = localTrackSectionsJson.value("curve_lengths").toArray().toVariantList().toVector(); // Длины кривых
    QVector<QVariant> localCurveRadiuses = localTrackSectionsJson.value("curve_radiuses").toArray().toVariantList().toVector(); // Радиусы кривых

    // надо так же для курвы, но пока ее нет
    foreach (QVariant item, localSlopes){
        slopes->push_back(item.toDouble());
    }
    foreach (QVariant item, localLengths){
        trackSectionLengths->push_back(item.toInt());
    }

    // рандомный ввод профиля для теста
    /* slopes->push_back(0);
    trackSectionLengths->push_back(6000);
    for (int i = 1; i < 7; i++){
        slopes->push_back((qrand() % 160 - 52) * 0.1);
        trackSectionLengths->push_back(qrand() % 15000);
    }
    slopes->push_back(6);
    trackSectionLengths->push_back(6000);
    slopes->push_back(0);
    trackSectionLengths->push_back(2500);*/
    // конец рандомного ввода, внести из исходных данных

    //arTrack.push_back(arIp);
    //arTrack.push_back(trackSectionLengths);
    //qDebug() << arTrack;



}

void Logic::setCoeffitient()
{
    //  bool Kolodki;
    if(dataJson->value("params").toObject().value("brake_pads").toString() == "Чугунные") {
        //do smthng

        k_tt =  0.27;    // коэффициенты для
        a_tt =  100;  // основого удельного торможение
        b_tt = 5;  // как минимум 2 вида под колодки

        k_okr = 68.5; // коэффициент, расчетные силы нажатия тормозных колодок, чугун или композит

    } else if (dataJson->value("params").toObject().value("brake_pads").toString() == "Композитные") {
        //do smthng else

        k_tt =  0.36;    // коэффициенты для
        a_tt =  150;  // основого удельного торможение
        b_tt = 2;  // как минимум 2 вида под колодки

        k_okr = 41.5; // коэффициент, расчетные силы нажатия тормозных колодок, чугун или композит


    }
    qDebug() << "Колодки" << dataJson->value("params").toObject().value("brake_pads").toString();

    if(dataJson->value("params").toObject().value("path").toString() == "Звеньевой") {
        //do
        locoA = 1.9;   // какие-то коэффициенты для
        locoB = 0.01;  // основное удельное сопротивление локомотива
        locoC = 0.0003; //


        k_hh =  2.4;    // коэффициенты для
        a_hh =  0.011;  // основого удельного на ХХ
        b_hh = 0.00035;  // постоянные или меняются (???)


    } else if (dataJson->value("params").toObject().value("path").toString() == "Безстыковой") {
        //or not to to
        locoA = 1.9;   // какие-то коэффициенты для
        locoB = 0.008;  // основное удельное сопротивление локомотива
        locoC = 0.00025; //


        k_hh =  2.4;    // коэффициенты для
        a_hh =  0.009;  // основого удельного на ХХ
        b_hh = 0.00035;  // постоянные или меняются (???)


    }
    qDebug() << "Путь" << dataJson->value("params").toObject().value("path").toString();

    stepV = 0.01;
    testSpeed = 50;

}


void Logic::onCalcSignalReceived()
{
    pointS = new QVector<double>();
    pointV = new QVector<double>();
    pointT = new QVector<double>();

    pointHH = new QVector<double>();
    pointVHH = new QVector<double>();
    pointBT = new QVector<double>();
    pointVBT = new QVector<double>();

    pointF = new QVector<double>();
    pointVF = new QVector<double>();

    partlocoTractionThrust = new QVector<double>();
    partlocoTractionVelocity = new QVector<double>();

    fW0Fin = new QVector<double>();
    w0xbtFin = new QVector<double>();
    w0xFin = new QVector<double>();

    pointSTor = new QVector<double>();
    pointVTor = new QVector<double>();

    fW0 = new QVector<double>(); // столбец таблицы, тяга
    w0xbt = new QVector<double>();

    setValues();
    setCoeffitient();

    // НАХОЖДЕНИЕ РАСЧЕТНОГО НАЧАЛО
    // тааак. тут ничего интересного, объявляем и ищем три самых жЫрных подъема
    // вспомогательные массивы для нахождения расчетного
    QVector <double> *arIpTemp = new QVector<double>();


    QVector <double> arCalcIp;
    QVector <double> arCalcLen;
    QVector <double> arCalcNum;
    double maxIp;
    int numIp = 0;
    arIpTemp->append(*slopes);


    for (int i = 0; i < trackCount; i++)
    {
        maxIp = -1000;
        for (int j = 0; j < arIpTemp->count(); j++) {
            if (arIpTemp->at(j) > maxIp){
                numIp = j;
                maxIp = arIpTemp->at(j);
            }
        }
        if (arCalcNum.count()>1 && fabs(maxIp-arCalcIp[i-1])<EPS) {
            if (arCalcLen[i-1] < trackSectionLengths->at(numIp))
                arCalcLen[i-1] = trackSectionLengths->at(numIp);
            i--;
        } else {
            arCalcNum.push_back(numIp);
            arCalcIp.push_back(maxIp);
            arCalcLen.push_back(trackSectionLengths->at(numIp));
        }
        arIpTemp->replace(numIp, 0);
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
                trainMass = trainMass - massAccuracy;
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
    qDebug() << "Ip[" << numIp << "]=" << slopes->at(numIp);  // расчетный, убрать дебаг, вывести в окне
    // вот тут нашли, запомнили его
    // НАХОЖДЕНИЕ РАСЧЕТНОГО КОНЕЦ



    // обновим данные таблицы, это мелкая, ее можно вывести

    trainMass = maxQ;
    qDebug() << "trainMass" << trainMass ; // масса, убрать дебаг, выевсти в окне
    railcarsCountUpdate();
    fW0 = littleTableW0(trainMass);

    //maxSpeed = 90; // пока без решения тормозной задачи, эта скорость ее итог

    // грустно считаем весь путь, сумма.
    distanse = 0;
    for (int i  =0; i < trackSectionLengths->length(); i++) {
        distanse += trackSectionLengths->at(i);
    }
    qDebug() << "Весь путь" << distanse ;  // убрать дебаг, вывести в окне

    mainIp = slopes->at(numIp);



    pointS->push_back(0);
    pointV->push_back(0);
    okrUpdate(trainMass);


    // ВАРИАНТ 1
    // какой-то большой Лагр...
    // до 15

    /*
    for (int i= 0; i < 2; i++)
    {
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    int s = 1 ; // s = speed
    // int deb = 0;

    // qDebug() << deb++ << "s=" << s << pointVF ;
    while (locoTractionVelocity->at(s) <= 15)
    {
        for (double k = partlocoTractionVelocity->at(0) ; k < partlocoTractionVelocity->at(1); k = k + stepV){
            FinalTable(k);
        }
        s++;
        partlocoTractionThrust->remove(0);
        partlocoTractionVelocity->remove(0);
        partlocoTractionThrust->push_back(locoTractionThrust->at(s));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(s));
        // qDebug() << deb++ << "s=" << s << pointVF ;
    }



    partlocoTractionThrust->clear();
    partlocoTractionVelocity->clear();

    // от 15 до 3 точки
    for (int i= s - 1; i < s + 3; i++)
    {
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    for (double k = partlocoTractionVelocity->at(0) ; k < partlocoTractionVelocity->at(3); k = k + stepV){
        FinalTable(k);
    }

    // qDebug() << deb++ << "s=" << s << pointVF ;

    s = s + 3;

    for (int i = s; i < locoTractionThrust->count(); i++)
    {
        //if (i != s) {
        partlocoTractionThrust->remove(0);
        partlocoTractionVelocity->remove(0);
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
        //      qDebug() << deb++ << "s=" << s << pointVF ;
        // }
        for (double k = partlocoTractionVelocity->at(2) ; k < partlocoTractionVelocity->at(3); k = k + stepV){
            FinalTable(k);
        }
    }

    // до конца
    for (double k = partlocoTractionVelocity->at(3) ; k < locoConstrVelocity; k = k + stepV){
        FinalTable(k);
    }*/

    // вариант 2
    /*for (int i= 0; i < 4; i++)
    {
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    for (double k = partlocoTractionVelocity->at(0) ; k < partlocoTractionVelocity->at(1); k = k + stepV){
        FinalTable(k);
    }

    for (int s = 4 ; s< locoTractionThrust->count() - 1; s++)
    {
        for (double k = partlocoTractionVelocity->at(1) ; k < partlocoTractionVelocity->at(2); k = k + stepV){
            FinalTable(k);
        }
        partlocoTractionThrust->remove(0);
        partlocoTractionVelocity->remove(0);
        partlocoTractionThrust->push_back(locoTractionThrust->at(s));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(s));
    }

    for (double k = partlocoTractionVelocity->at(2) ; k <= locoConstrVelocity; k = k + stepV){
        FinalTable(k);
    }*/


    // вариант 3
    /* for (int i= 0; i < 2; i++)
    {
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    for (int s = 1 ; s< locoTractionThrust->count() - 1; s++)
    {
        for (double k = partlocoTractionVelocity->at(0) ; k < partlocoTractionVelocity->at(1); k = k + stepV){
            FinalTable(k);
        }
        partlocoTractionThrust->remove(0);
        partlocoTractionVelocity->remove(0);
        partlocoTractionThrust->push_back(locoTractionThrust->at(s));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(s));
    }

    for (double k = partlocoTractionVelocity->at(0) ; k <= locoConstrVelocity; k = k + stepV){
        FinalTable(k);
    }*/

    // вариант 4
    for (int i= 0; i < 2; i++)
    {
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    int s = 1 ; // s = speed
    while (locoTractionVelocity->at(s-1) <= 15)
    {

        for (double k = partlocoTractionVelocity->at(0) ; k < partlocoTractionVelocity->at(1); k = k + stepV){
            FinalTable(k);
        }
        s++;
        partlocoTractionThrust->remove(0);
        partlocoTractionVelocity->remove(0);
        partlocoTractionThrust->push_back(locoTractionThrust->at(s));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(s));
    }

    partlocoTractionThrust->clear();
    partlocoTractionVelocity->clear();

    for (int i= s - 1; i < s + 3; i++)
    {
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    for (double k = partlocoTractionVelocity->at(0) ; k < partlocoTractionVelocity->at(1); k = k + stepV){
        FinalTable(k);
    }

    for (int i = s + 3 ; i < locoTractionThrust->count(); i++)
    {

        for (double k = partlocoTractionVelocity->at(1) ; k < partlocoTractionVelocity->at(2); k = k + stepV){
            FinalTable(k);
        }
        partlocoTractionThrust->remove(0);
        partlocoTractionVelocity->remove(0);
        partlocoTractionThrust->push_back(locoTractionThrust->at(i));
        partlocoTractionVelocity->push_back(locoTractionVelocity->at(i));
    }

    for (double k = partlocoTractionVelocity->at(2) ; k <= locoConstrVelocity; k = k + stepV){
        FinalTable(k);
    }



    // вспомогательные
    double S = 0; // прохождение всего пути
    double currentS = 0; // прохождение участка пути
    double currentSpeed = 0;
    int currentSector = 0;
    double addPoint;
    double addTimePoint;

    timeAll = 0;
    timeThrust = 0;


    double currentTime = 0;
    pointT->push_back(0);
    int moveMode = 0; // режим движения. 0 - тяга, 1 - ХХ вниз 2 -  тормоз, 2 - ХХ вверх

    // построение
    do {
        currentS = 0;
        // qDebug() << slopes->at(currentSector) << currentSector << moveMode << currentSpeed;
        if (slopes->at(currentSector) >= 0 || moveMode == 0) {
            moveMode = 0;
        } else {
            moveMode = 4;
        }
        // qDebug() << slopes->at(currentSector) << currentSector << moveMode << currentSpeed;
        do {
            if ((currentSpeed + stepV) > maxSpeed) {
                moveMode = 1;
            }
            // режим движения
            switch (moveMode) {
            case 0:
                FwosrIp = fW0Fin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                break;
            case 1:
                FwosrIp = w0xFin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                if (FwosrIp >= 0) {
                    moveMode = 2;
                    FwosrIp = w0xbtFin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                }
                if (currentSpeed <= maxSpeed - 10) {
                    moveMode = 0;
                }
                break;
            case 2:
                FwosrIp = w0xbtFin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                // qDebug() << FwosrIp << currentSpeed;
                if (currentSpeed <= maxSpeed - 10) {
                    moveMode = 3;
                }
                break;
            case 3:
                FwosrIp = w0xFin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                if (FwosrIp <= 0) {
                    moveMode = 0;
                    FwosrIp = fW0Fin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                }
                break;
            case 4:
                FwosrIp = w0xFin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                if (FwosrIp <= 0) {
                    moveMode = 1;
                } else {
                    moveMode = 3;
                }
                break;
            default:
                FwosrIp = fW0Fin->at(abs(static_cast<int>(currentSpeed/stepV))) - slopes->at(currentSector);
                break;
            }


            if (FwosrIp > 0) {
                stepV = fabs(stepV);
            } else {
                stepV = -fabs(stepV);
            }

            addPoint = pathPoint(currentSpeed + stepV, currentSpeed, FwosrIp);
            addTimePoint = timePoint(currentSpeed + stepV, currentSpeed, FwosrIp);

            currentS += addPoint;
            currentSpeed += stepV;

            if (currentS > trackSectionLengths->at(currentSector))
            {
                double tempStepV = stepV;
                currentS -= addPoint;
                currentSpeed -= stepV;
                do {
                    tempStepV = tempStepV - tempStepV/10;
                    addPoint = pathPoint(currentSpeed + tempStepV, currentSpeed, FwosrIp);
                } while ((currentS+addPoint) > trackSectionLengths->at(currentSector) || tempStepV > 0.0005);

                addTimePoint = timePoint(currentSpeed + tempStepV, currentSpeed, FwosrIp);
                currentS = trackSectionLengths->at(currentSector);
                currentSpeed += tempStepV;
            }


            if (moveMode == 0) {
                timeThrust += addTimePoint;
            }

            timeAll += addTimePoint;

            currentTime += addTimePoint;
            if (currentTime > 100) {
                currentTime += -100;
            }

            pointT->push_back(currentTime);
            pointV->push_back(currentSpeed);

            switch (moveMode) {
            case 1:
                pointHH->push_back(currentS + S);
                pointVHH->push_back(currentSpeed);
                break;
            case 2:
                pointBT->push_back(currentS + S);
                pointVBT->push_back(currentSpeed);
                break;
            case 3:
                pointHH->push_back(currentS + S);
                pointVHH->push_back(currentSpeed);
                break;
            default:
                break;
            }

            pointS->push_back(currentS + S);

            //   qDebug() << "curS" << currentS << arLen[currentSector] << currentSector;

        } while (currentS < trackSectionLengths->at(currentSector));
        currentSector++ ;
        S += currentS;

        //  qDebug() << "S" << S << distanse << currentSector;
    } while (S < distanse);

    //    qDebug() << "SHH" << pointHH;
    //  qDebug() << "VHH" << pointVHH;

    //    qDebug() << "SBT" << pointBT;
    //  qDebug() << "VBT" << pointVBT;


    littleTable_ptr = &littleTable;
    qDebug() << "TimeTr" << timeThrust;
    qDebug() << "Time" << timeAll;
    //   qDebug() << "S" << pointS;
    //   qDebug() << "T" << pointT;
    // закончили построение.


    pointSTor->push_back(distanse);
    pointVTor->push_back(0);

    // построение отрезка торможения
    //    S = 0;
    //    currentS = 0;
    //    currentSector = arIp.count() - 1;
    //    currentSpeed = 0;

    //    do {

    //        FwosrIp = w0xbtFin[abs(static_cast<int>(currentSpeed/stepV))] + arIp[currentSector];
    //       // FwosrIp = - deltaw0xbt[static_cast<int>(floor(currentSpeed / 10))] + arIp[currentSector];
    //        addPoint = pathPoint(currentSpeed, currentSpeed + stepV, FwosrIp);
    //        currentSpeed += stepV;
    //        currentS += addPoint;
    //        pointSTor.push_front(distanse - currentS - S);
    //        pointVTor.push_front(currentSpeed);
    //    } while (currentSpeed < maxSpeed);

    // qDebug() << "STor" << pointSTor;
    // qDebug() << "VTor" << pointVTor;

    if (fuelMode) {
        fuelCons = timeThrust * thrustFuel + (timeAll - timeThrust) * noThrustFuel;
        specfuelCons = fuelCons / trainMass / (distanse/1000) * 10000;
        qDebug() << "Расход / удельный расход топлива" << fuelCons << "/" << specfuelCons;
    }


}

QVector<QVector<double> > *Logic::getLittleTable_ptr() const
{
    return littleTable_ptr;
}

QVector<double> *Logic::getPointVTor() const
{
    return pointVTor;
}

QVector<double> *Logic::getPointSTor() const
{
    return pointSTor;
}

QVector<int> *Logic::getTrackSectionLengths() const
{
    return trackSectionLengths;
}

QVector<double> *Logic::getPointVBT() const
{
    return pointVBT;
}

QVector<double> *Logic::getPointBT() const
{
    return pointBT;
}

QVector<double> *Logic::getPointHH() const
{
    return pointHH;
}

QVector<double> *Logic::getPointVHH() const
{
    return pointVHH;
}

double Logic::getMaxSpeed() const
{
    return maxSpeed;
}

QVector<double> *Logic::getW0xFin() const
{
    return w0xFin;
}

QVector<double> *Logic::getW0xbtFin() const
{
    return w0xbtFin;
}

QVector<double> *Logic::getFW0Fin() const
{
    return fW0Fin;
}

QVector<double> *Logic::getPointVF() const
{
    return pointVF;
}

QVector<double> *Logic::getPointF() const
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

QVector<double> *Logic::getPointT() const
{
    return pointT;
}

QVector<double> *Logic::getPointV() const
{
    return pointV;
}

QVector<double> *Logic::getPointS() const
{
    return pointS;
}

double Logic::lagranz(QVector<double> *X, QVector<double> *Y, double t)
{
    double sum, prod;
    sum = 0;
    for (int j = 0; j < X->size(); j++){
        prod = 1;
        for (int i = 0; i < Y->size(); i++){
            if (i != j) {
                prod = prod * (t - X->at(i)) / (X->at(j) - X->at(i));
            }
        }
        sum = sum + Y->at(j) * prod;
    }
    return sum;
}

double Logic::w0ll(const double v)
{


    double w = 0; // считаем основное удельное сопротивление
    double qUnit; // масса, приходящаяся на одну колесную пару

    // FIXME цикл для каждого из вагонов
    for (int i = 0; i < railcarAxleCounts->count(); i++){
        qUnit = railcarMasses->at(i) / railcarAxleCounts->at(i);
        w = w + railcarPercents->at(i) * (railcarCoefsK->at(i) + (railcarCoefsA->at(i) + railcarCoefsB->at(i) * v + railcarCoefsC->at(i) * v * v) / qUnit);
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
    for (int i = 0; i < railcarAxleCounts->count(); i++) {
        if (railcarAxleCounts->at(i) == 4) {
            currentlen = lenghtRailcars[0];
        } else {
            currentlen = lenghtRailcars[1];
        }
        lenghtTrain += floor(railcarPercents->at(i) * Q / railcarMasses->at(i)) * currentlen; // вагоны не дробные, округляем-с
    }
    return lenghtTrain;
}

void Logic::okrUpdate(double trainMass)
{
    okr = 0;
    for (int i = 0; i < railcarAxleCounts->count(); i++)
    {
        okr += railcarAxleCounts->at(i) * railcarCount->at(i);
    }
    okr = k_okr * okr / trainMass / g;

}

double Logic::pathSum(const double vMax, const double vMin, const double ip)
{

    double Fwosr; // для расчетов, не знаю, как обозвать
    int numMax = static_cast<int>(floor(vMax / 10));
    int numMin = static_cast<int>(floor(vMin / 10));


    if (fabs(vMin - locoCalcVelocity) < EPS) {
        Fwosr = (fW0->value(numMax) + calcVelParam[6]) / 2;
    } else {
        Fwosr = (fW0->value(numMax) + fW0->value(numMin)) / 2;
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

QVector<double> *Logic::littleTableW0(double trainMass)
{
    littleTable_ptr = new QVector<QVector<double> >();
    QVector <double> *fW0cur = new QVector<double>();
    QVector <double> *w0xbtcur = new QVector<double>();

    okrUpdate(trainMass);
    if (littleTable.size() == 0) {
        for(int i = 0; i < locoConstrVelocity / 10 + 1; i++)
        {
            QVector<double> tempVector;

            for(int j = 0; j < 15; j++)
            {
                tempVector.push_back(0);
            }
            littleTable.push_back(tempVector);
        }
    }

    // построим табличку, см эксель

    for (int i = 0; i < locoConstrVelocity / 10 + 1; i++) {
        littleTable[i][0] = i * 10;
        littleTable[i][1] = lagranz(locoTractionVelocity, locoTractionThrust, littleTable[i][0]);

        // тяга
        littleTable[i][2] = w0l(littleTable[i][0]) * g * locoMass;
        littleTable[i][3] = w0ll(littleTable[i][0]) * g * trainMass;
        littleTable[i][4] = littleTable[i][2] + littleTable[i][3];
        littleTable[i][5] = 1000 * littleTable[i][1] - littleTable[i][4];
        littleTable[i][6] = littleTable[i][5] / ((locoMass+trainMass) * g);
        fW0cur->push_back(littleTable[i][6]); // в методичке написано, что это для диаграммы удельных, тяга
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
        w0xbtcur->push_back(littleTable[i][14]);
    }

    //littleTable_ptr = &littleTable;

    w0xbt = w0xbtcur;
    return fW0cur;
}

void Logic::FinalTable(double currentV)
{
    double calcMode[7];

    pointVF->push_back(currentV);

    calcMode[0] = lagranz(partlocoTractionVelocity, partlocoTractionThrust, currentV);

    pointF->push_back(calcMode[0]);

    calcMode[4] = w0ll(currentV) * g * trainMass;
    calcMode[1] = 1000 * calcMode[0] - (w0l(currentV) * g * locoMass + calcMode[4]);
    calcMode[1] = calcMode[1]/((locoMass+trainMass) * g);

    fW0Fin->push_back(calcMode[1]);

    calcMode[2] = calcMode[4] + (k_hh + a_hh * currentV + b_hh * currentV * currentV) * g * locoMass ;
    calcMode[2] = calcMode[2] / ((locoMass + trainMass) * g);

    w0xFin->push_back(-calcMode[2]);

    calcMode[3] = 1000 * okr * (k_tt * ((currentV + a_tt) / (b_tt * currentV + a_tt)));
    calcMode[5] = calcMode[2] + 0.5 * calcMode[3];
    calcMode[6] = calcMode[2] + calcMode [3];

    w0xbtFin->push_back(-calcMode[5]);
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
    railcarCount = new QVector<int>();
    railcarCount->clear();
    for (int i = 0; i < railcarAxleCounts->count(); i++) {
        railcarCount->push_back(static_cast<int>(floor(railcarPercents->at(i) * trainMass / railcarMasses->at(i)))); // вагоны не дробные, округляем-с
    }
    qDebug() << "Количество вагонов" << *railcarCount;
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
