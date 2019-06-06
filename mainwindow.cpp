#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    disableSecondaryButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disableSecondaryButtons()
{
    ui->pushButtonShowDiag->setDisabled(true);
    ui->pushButtonTraction->setDisabled(true);
    ui->pushButtonTable->setDisabled(true);
}

void MainWindow::enableSecondaryButtons()
{
    ui->pushButtonShowDiag->setEnabled(true);
    ui->pushButtonTraction->setEnabled(true);
    ui->pushButtonTable->setEnabled(true);
}

void MainWindow::buildVsGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(0)->setData(*logic->getPointS(), *logic->getPointV());
}

void MainWindow::buildVCalcGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(1)->setData({0, logic->getDistanse()}, {logic->getLocoCalcVelocity(), logic->getLocoCalcVelocity()});
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
    ui->mainGraph->graph(3)->setData({0, logic->getDistanse()}, {logic->getMaxSpeed(), logic->getMaxSpeed()});
    ui->mainGraph->graph(3)->setPen(QPen(Qt::red));
}

void MainWindow::drawTrackSections()
{
    QVector<int> localTrackSectionLengths = *logic->getTrackSectionLengths();
    int currentTrackSectionPosition = 0;

    for (int i = 0; i < localTrackSectionLengths.size(); i++) {
        currentTrackSectionPosition += localTrackSectionLengths.at(i);
        QCPItemLine *upperVerticalLine = new QCPItemLine(ui->mainGraph);
        upperVerticalLine->start->setCoords(currentTrackSectionPosition, 0);
        upperVerticalLine->end->setCoords(currentTrackSectionPosition, 100);
        upperVerticalLine->setPen(QPen(Qt::DashLine));
        upperVerticalLine->setPen(QPen(Qt::gray));

        QCPItemLine *lowerVerticalLine = new QCPItemLine(ui->mainGraph);
        lowerVerticalLine->start->setCoords(currentTrackSectionPosition, -10);
        lowerVerticalLine->end->setCoords(currentTrackSectionPosition, 0);
        lowerVerticalLine->setPen(QPen(Qt::SolidLine));
        lowerVerticalLine->setPen(QPen(Qt::black));
    }
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

void MainWindow::on_pushButtonTest_clicked()
{

}

void MainWindow::onBuildGraphSignalReceived(const QJsonObject &dataJson)
{
    //QJsonObject localDataJson = dataJson;
    logic = new Logic(this, &dataJson);
    connect(this, &MainWindow::calc, logic, &Logic::onCalcSignalReceived);
    emit calc();

    if(logic->getDistanse() < 40000) {
        ui->mainGraph->setFixedWidth((static_cast<int>(40000 / 48 + 50)));
    } else {
        ui->mainGraph->setFixedWidth((static_cast<int>(logic->getDistanse() / 48 + 50)));
    }

    ui->mainGraph->clearGraphs();
    //ui->mainGraph->setInteractions(QCP::iRangeDrag |QCP::iRangeZoom);
    //ui->mainGraph->axisRect()->setRangeZoom(Qt::Horizontal);
    //ui->mainGraph->axisRect()->setRangeDrag(Qt::Horizontal);

    //ui->mainGraph->graph()->

    buildVsGraph();
    buildVCalcGraph();
    buildTimeGraph();
    buildMaxVelocityGraph();
    buildHHVsGraph();
    buildBTVsGraph();
    buildFinalBTVsGraph();

    drawTrackSections();

    ui->mainGraph->xAxis->setRange(0, logic->getDistanse() + 50);
    ui->mainGraph->xAxis->setLabel("S, м");
    ui->mainGraph->yAxis->setRange(-10, logic->getLocoConstrVelocity() + 10);
    ui->mainGraph->yAxis->grid()->setZeroLinePen(QPen(Qt::SolidLine | Qt::black));
    ui->mainGraph->yAxis->setLabel("v, км/ч");

    //ui->mainGraph->legend->setVisible(true);

    ui->mainGraph->replot();
    //qDebug() << dataJson;
    //delete logic;
    enableSecondaryButtons();
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
    connect(this, static_cast<void (MainWindow::*)(QVector<QVector<double>> const&)>(&MainWindow::buildTractionGraph), wSecondaryGraphWindow, &SecondaryGraphWindow::onBuildDiagTractionSignalReceived);

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
