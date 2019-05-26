#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawVsGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(0)->setData(logic->getPointS(), logic->getPointV());
}

void MainWindow::drawVCalcGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(1)->setData({0, logic->getDistanse()}, {CALC_VELOCITY, CALC_VELOCITY});
    ui->mainGraph->graph(1)->setPen(QPen(Qt::red));
}

void MainWindow::drawTimeGraph()
{
    ui->mainGraph->addGraph();
    ui->mainGraph->graph(2)->setData(logic->getPointS(), logic->getPointT());
    ui->mainGraph->graph(2)->setPen(QPen(Qt::green));
}

void MainWindow::on_action_new_triggered()
{
    wInputWindow = new InputWindow(this);
    wInputWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wInputWindow, &InputWindow::showMainWindow, this, &MainWindow::show);
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

void MainWindow::on_pushButtonCalc_clicked()
{
    logic = new Logic(this);
    connect(this, &MainWindow::calc, logic, &Logic::onCalcSignalReceived);
    emit calc();

    ui->mainGraph->setFixedWidth((static_cast<int>(logic->getDistanse() / 48 + 50)));
    ui->mainGraph->clearGraphs();

    drawVsGraph();
    drawVCalcGraph();
    drawTimeGraph();

    ui->mainGraph->xAxis->setRange(0, logic->getDistanse() + 50);
    ui->mainGraph->yAxis->setRange(0, CONSTRUCTION_VELOCITY + 10);
    ui->mainGraph->replot();
    delete logic;
}

void MainWindow::on_pushButtonTest_clicked()
{

}
