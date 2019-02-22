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
    wLocomotiveDbWindow = new LocomotiveDbWindow(this);
    wLocomotiveDbWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wLocomotiveDbWindow, &LocomotiveDbWindow::showMainWindow, this, &MainWindow::show);
    wLocomotiveDbWindow->show();
    this->hide();
}

void MainWindow::on_action_railcars_triggered()
{
    wRailcarDbWindow = new RailcarDbWindow(this);
    wRailcarDbWindow->setAttribute(Qt::WA_DeleteOnClose, true);

    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wRailcarDbWindow, &RailcarDbWindow::showMainWindow, this, &MainWindow::show);
    wRailcarDbWindow->show();
    this->hide();
}
