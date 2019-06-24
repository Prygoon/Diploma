#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //disableSecondaryButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::disableSecondaryButtons()
//{
//    ui->pushButtonShowDiag->setDisabled(true);
//    ui->pushButtonTraction->setDisabled(true);
//    ui->pushButtonTable->setDisabled(true);
//}

void MainWindow::enableSecondaryButtons()
{
    ui->pushButtonShowDiag->setEnabled(true);
    ui->pushButtonTraction->setEnabled(true);
    ui->pushButtonTable->setEnabled(true);
    ui->pushButtonSaveGraph->setEnabled(true);
}

void MainWindow::buildVsGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(0)->setData(*logic->getPointS(), *logic->getPointV());
}

void MainWindow::buildVCalcGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(1)->setData({0, logic->getDistance()}, {logic->getLocoCalcVelocity(), logic->getLocoCalcVelocity()});
    ui->mainGraph->graph(1)->setPen(QPen(Qt::red));
}

void MainWindow::buildTimeGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(2)->setData(*logic->getPointS(), *logic->getPointT());
    ui->mainGraph->graph(2)->setPen(QPen(Qt::green));
}

void MainWindow::buildMaxVelocityGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(3)->setData({0, logic->getDistance()}, {logic->getMaxSpeed(), logic->getMaxSpeed()});
    ui->mainGraph->graph(3)->setPen(QPen(Qt::red));
}

void MainWindow::drawTrackSections()
{
    QVector<int> *localTrackSectionLengths = logic->getTrackSectionLengths();
    QVector<double> *localSlopes = logic->getSlopes();
    int currentTrackSectionPosition = 0;
    int previousTrackSectionPosition = 0;

    for (int i = 0; i < localTrackSectionLengths->size(); i++) {
        currentTrackSectionPosition += localTrackSectionLengths->at(i);
        QCPItemLine *upperVerticalLine = new QCPItemLine(ui->mainGraph);
        upperVerticalLine->start->setCoords(currentTrackSectionPosition, 0);
        upperVerticalLine->end->setCoords(currentTrackSectionPosition, 100);
        upperVerticalLine->setPen(QPen(Qt::gray));

        QCPItemLine *lowerVerticalLine = new QCPItemLine(ui->mainGraph);
        lowerVerticalLine->start->setCoords(currentTrackSectionPosition, -10);
        lowerVerticalLine->end->setCoords(currentTrackSectionPosition, 0);

        if(i > 0) {
            previousTrackSectionPosition += localTrackSectionLengths->at(i - 1);
        }
        //if(i < localSlopes->size() - 1) {
        if(localSlopes->at(i) > 0) {
            QCPItemLine *risingDiagonal = new QCPItemLine(ui->mainGraph);
            risingDiagonal->start->setCoords(previousTrackSectionPosition, -10);
            risingDiagonal->end->setCoords(currentTrackSectionPosition, 0);
            drawTrackSectionText(localTrackSectionLengths->at(i), localSlopes->at(i), previousTrackSectionPosition);

        } else if (localSlopes->at(i) < 0) {
            QCPItemLine *descendingDiagonal = new QCPItemLine(ui->mainGraph);
            descendingDiagonal->start->setCoords(previousTrackSectionPosition, 0);
            descendingDiagonal->end->setCoords(currentTrackSectionPosition, -10);
            drawTrackSectionText(localTrackSectionLengths->at(i), localSlopes->at(i), previousTrackSectionPosition);

        } else if (localSlopes->at(i) == 0.0) {
            QCPItemLine *zeroSlopeLine = new QCPItemLine(ui->mainGraph);
            zeroSlopeLine->start->setCoords(previousTrackSectionPosition, -5);
            zeroSlopeLine->end->setCoords(currentTrackSectionPosition, -5);
            drawTrackSectionText(localTrackSectionLengths->at(i), localSlopes->at(i), previousTrackSectionPosition);

        }
        //}
    }
}

void MainWindow::drawTrackSectionText(int length, double slope, int position)
{
    int lowerMargin;
    int upperMargin;
    double leftMargin;
    double rightMargin;
    QFont localQFont = QFont(font().family(), 7);
    if(slope != 0.0) {
        lowerMargin = -7;
        upperMargin = -3;
    } else {
        lowerMargin = -7;
        upperMargin = -2;
    }

    if(slope < 0) {
        leftMargin = 4;
        rightMargin = 1.25;
    } else if (slope > 0) {
        leftMargin = 1.25;
        rightMargin = 4;
    } else {
        leftMargin = 2;
        rightMargin = 2;
    }

    if(length <= 700) {
        localQFont.setPointSize(5);
    }

    QCPItemText *lengthTextLabel = new QCPItemText(ui->mainGraph);
    lengthTextLabel->position->setCoords(position + length / leftMargin, lowerMargin);
    lengthTextLabel->setText(QString::number(length));
    lengthTextLabel->setFont(localQFont);

    QCPItemText *slopeTextLabel = new QCPItemText(ui->mainGraph);
    slopeTextLabel->position->setCoords(position + length / rightMargin, upperMargin);
    slopeTextLabel->setText(toFormattedString(slope));
    slopeTextLabel->setFont(localQFont);
}

void MainWindow::outputResults()
{
    ui->distanceResultLabel->setText(QString::number(logic->getDistance()).append(" метров"));
    ui->calcSlopeLabel->setText(QString("Расчётный\nподъем (").append(QString::number(logic->getNumIp() + 1)).append(")"));
    ui->calcSlopeResultLabel->setText(QString::number(logic->getMainIp()).append(" ‰").append("\n").append(QString::number(logic->getMainLen())).append(" метров"));
    ui->massResultLabel->setText(QString::number(logic->getTrainMass()).append(" тонн"));
    ui->timeResultabel->setText(QString::number(logic->getTimeAll()).append(" минут"));
    if(logic->getFuelMode()) {
        ui->unitFuelResultlabel->setText(QString::number(logic->getSpecfuelCons()).append("<br/>кг/(10<sup>4</sup> · т·км)"));
        ui->fuelResultLabel->setText(QString::number(logic->getFuelCons()).append(" кг"));
    } else {
        ui->unitFuelResultlabel->setText("-");
        ui->fuelResultLabel->setText("-");
    }
}

QString MainWindow::toFormattedString(qreal num)
{
    QString str = QString::number(num, 'f', 2);
    str.remove(QRegExp("0+$")); // Remove any number of trailing 0's
    str.remove(QRegExp("\\.$")); // If the last character is just a '.' then remove it
    return str;
}

void MainWindow::buildHHVsGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(4)->setData(*logic->getPointHH(), *logic->getPointVHH());
    ui->mainGraph->graph(4)->setLineStyle(QCPGraph::lsNone);
    ui->mainGraph->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::magenta, 1));

}

void MainWindow::buildBTVsGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(5)->setData(*logic->getPointBT(), *logic->getPointVBT());
    //ui->mainGraph->graph(5)->setPen(QPen(Qt::darkYellow));
    ui->mainGraph->graph(5)->setLineStyle(QCPGraph::lsNone);
    ui->mainGraph->graph(5)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::darkYellow, 2));
}

void MainWindow::buildFinalBTVsGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(6)->setData(*logic->getPointSTor(), *logic->getPointVTor());
    ui->mainGraph->graph(6)->setPen(QPen(Qt::darkBlue));
    qDebug() << *logic->getPointSTor() << endl << *logic->getPointVTor();
}

void MainWindow::on_action_new_triggered()
{
    wInputWindow = new InputWindow();
    wInputWindow->setParent(this, Qt::Window);
    wInputWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wInputWindow, &InputWindow::showMainWindow, this, &MainWindow::show);

    connect(wInputWindow, static_cast<void (InputWindow::*)(QJsonObject const&)>(&InputWindow::buildGraph), this, &MainWindow::onBuildGraphSignalReceived);
    wInputWindow->show();
    this->hide();
}

void MainWindow::on_action_close_triggered()
{
    close();
}

void MainWindow::on_action_loco_triggered()
{
    wLocomotiveDbWindow = new LocomotiveDbWindow();
    wLocomotiveDbWindow->setParent(this, Qt::Window);
    wLocomotiveDbWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wLocomotiveDbWindow, &LocomotiveDbWindow::showMainWindow, this, &MainWindow::show);
    wLocomotiveDbWindow->show();
    this->hide();
}

void MainWindow::on_action_railcars_triggered()
{
    wRailcarDbWindow = new RailcarDbWindow();
    wRailcarDbWindow->setParent(this, Qt::Window);
    wRailcarDbWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wRailcarDbWindow, &RailcarDbWindow::showMainWindow, this, &MainWindow::show);
    wRailcarDbWindow->show();
    this->hide();
}

void MainWindow::onBuildGraphSignalReceived(const QJsonObject &dataJson)
{
    //QJsonObject localDataJson = dataJson;
    logic = new Logic(this, &dataJson);
    connect(this, &MainWindow::calc, logic, &Logic::onCalcSignalReceived);
    emit calc();

    if(logic->getDistance() < 50000) {
        ui->mainGraph->setFixedWidth((static_cast<int>(50000 / 48 + 50)));
    } else {
        ui->mainGraph->setFixedWidth((static_cast<int>(logic->getDistance() / 48 + 50)));
    }

    ui->mainGraph->clearGraphs();
    ui->mainGraph->clearItems();
    //ui->mainGraph->setInteractions(/*QCP::iRangeDrag |*/ QCP::iRangeZoom);
    //ui->mainGraph->axisRect()->setRangeZoom(Qt::Horizontal);
    //    ui->mainGraph->axisRect()->setRangeDrag(Qt::Horizontal);

    //ui->mainGraph->graph()->

    buildVsGraph();
    buildVCalcGraph();
    buildTimeGraph();
    buildMaxVelocityGraph();
    buildHHVsGraph();
    buildBTVsGraph();
    buildFinalBTVsGraph();

    drawTrackSections();

    ui->mainGraph->xAxis->setRange(0, logic->getDistance() + 50);
    ui->mainGraph->xAxis->setLabel("S, м");
    ui->mainGraph->xAxis->setTicks(false);
    ui->mainGraph->yAxis->setRange(-10, logic->getLocoConstrVelocity() + 10);
    ui->mainGraph->yAxis->grid()->setZeroLinePen(QPen(Qt::SolidLine | Qt::black));
    ui->mainGraph->yAxis->setLabel("v, км/ч  t, мин");

    //ui->mainGraph->legend->setVisible(true);

    ui->mainGraph->replot();
    //qDebug() << dataJson;
    //delete logic;
    enableSecondaryButtons();
    outputResults();
}

void MainWindow::on_pushButtonShowDiag_clicked()
{
    secondaryData = new QVector<QVector<double>>;

    secondaryData->push_back(*logic->getPointVF());
    secondaryData->push_back(*logic->getFW0Fin());
    secondaryData->push_back(*logic->getW0xFin());
    secondaryData->push_back(*logic->getW0xbtFin());

    QString *buttonName = new QString(ui->pushButtonShowDiag->objectName());
    wSecondaryGraphWindow = new SecondaryGraphWindow(buttonName, this);
    wSecondaryGraphWindow->setAttribute(Qt::WA_DeleteOnClose);
    wSecondaryGraphWindow->show();
    ui->pushButtonShowDiag->setDisabled(true);

    connect(wSecondaryGraphWindow, &SecondaryGraphWindow::enableShowDiagGraphButton, this, &MainWindow::onEnableShowDiagGraphButtonReceived);
    connect(this, static_cast<void (MainWindow::*)(QVector<QVector<double>> const&)>(&MainWindow::buildDiagGraph), wSecondaryGraphWindow, &SecondaryGraphWindow::onBuildDiagGraphSignalReceived);

    emit buildDiagGraph(*secondaryData);
    delete secondaryData;
}

void MainWindow::on_pushButtonTraction_clicked()
{
    secondaryData = new QVector<QVector<double>>;

    secondaryData->push_back(*logic->getPointVF());
    secondaryData->push_back(*logic->getPointF());

    QString *buttonName = new QString(ui->pushButtonTraction->objectName());
    wSecondaryGraphWindow = new SecondaryGraphWindow(buttonName, this);
    wSecondaryGraphWindow->setAttribute(Qt::WA_DeleteOnClose);
    wSecondaryGraphWindow->show();

    ui->pushButtonTraction->setDisabled(true);

    connect(wSecondaryGraphWindow, &SecondaryGraphWindow::enableShowTractionGraphButton, this, &MainWindow::onEnableShowTractionGraphButtonReceived);
    connect(this, static_cast<void (MainWindow::*)(QVector<QVector<double>> const&)>(&MainWindow::buildTractionGraph), wSecondaryGraphWindow, &SecondaryGraphWindow::onBuildTractionGraphSignalReceived);

    emit buildTractionGraph(*secondaryData);
    delete secondaryData;

}

void MainWindow::on_pushButtonTable_clicked()
{
    secondaryData = logic->getLittleTable_ptr();

    wUnitResultantForceTableWindow = new UnitResultantForceTableWindow(this, secondaryData);
    wUnitResultantForceTableWindow->setAttribute(Qt::WA_DeleteOnClose);
    wUnitResultantForceTableWindow->show();

    ui->pushButtonTable->setDisabled(true);

    connect(wUnitResultantForceTableWindow, &UnitResultantForceTableWindow::enableShowTableButton, this, &MainWindow::onEnableShowTableButtonReceived);

    //delete secondaryData;
}

void MainWindow::onEnableShowDiagGraphButtonReceived()
{
    ui->pushButtonShowDiag->setEnabled(true);
}

void MainWindow::onEnableShowTractionGraphButtonReceived()
{
    ui->pushButtonTraction->setEnabled(true);
}

void MainWindow::onEnableShowTableButtonReceived()
{
    ui->pushButtonTable->setEnabled(true);
}

void MainWindow::on_pushButtonSaveGraph_clicked()
{
    qDebug() << QDir::homePath();
    int width;
    QString saveFileName = QFileDialog::getSaveFileName(this, "Сохранить файл", QDir::homePath(), "*.png");
    qDebug() << saveFileName;

    if(logic->getDistance() < 50000) {
        width = static_cast<int>(50000 / 24 + 50);
    } else {
        width = static_cast<int>(logic->getDistance() / 24 + 50);
    }

    if(saveFileName != "") {
        if(!saveFileName.contains(".png")){
            ui->mainGraph->savePng(saveFileName.append(".png"), width, 720);
        } else {
            ui->mainGraph->savePng(saveFileName, width, 720);
        }
    }
}

void MainWindow::on_action_about_triggered()
{

    QMessageBox::about(this, "О программе.", "<h2>О программе.</h2>"
                                             "Версия alpha 1.0, 2019 г.<br/>"
                                             "Программа тяговых расчетов разработана на языке C++ на основе открытого ПО.<br/>"
                                             "Использованные модули: QtCore, QtGui, QtSql, QtXlsx.");

    /*"Правила добавления Excel файлов:"
                       "1.  Таблицы должны быть расположены горизонтально, на первом листе, в диапазоне A1:AX50."
                       "2.  Для тяговых характеристик: первая строка – скорость, вторая строка – значение тяги."
                       "Для профиля пути: первая строка – уклон, вторая строка – длина элемента, третья и четвертые строки (не обязательно) – длина и радиус кривой.");*/
}

void MainWindow::on_action_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_action_tutorial_triggered()
{
    QMessageBox::about(this, "Инструкция.", "<h1>Инструкция.</h1>"
                                            "<h2>Правила добавления Excel файлов:</h2>" \
                                            "1.  Таблицы должны быть расположены горизонтально, на первом листе, в диапазоне A1:AX50.<br/>" \
                                            "2.  Для тяговых характеристик: первая строка – скорость, вторая строка – значение тяги.<br/>" \
                                            "Для профиля пути: первая строка – уклон, вторая строка – длина элемента, третья и четвертые строки (не обязательно) – длина и радиус кривой.");
}
