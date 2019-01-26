#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    wInputWindow = new InputWindow(this);
    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wInputWindow, SIGNAL(showMainWindow()), this, SLOT(show()));
    //setLocoImage();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_new_triggered()
{
    wInputWindow->show();
    //this->hide();
}

void MainWindow::on_action_close_triggered()
{
    close();
}

void MainWindow::on_action_loco_triggered()
{
    wLocomotiveDbWindow = new LocomotiveDbWindow(this);
    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wLocomotiveDbWindow, SIGNAL(showMainWindow()), this, SLOT(show()));
    wLocomotiveDbWindow->show();
    this->hide();
}

void MainWindow::on_action_railcars_triggered()
{
    wRailcarDbWindow = new RailcarDbWindow(this);
    /* подключаем к слоту запуска главного окна по кнопке во окне ввода */
    connect(wRailcarDbWindow, SIGNAL(showMainWindow()), this, SLOT(show()));
    wRailcarDbWindow->show();
    this->hide();
}

/*void MainWindow::setLocoImage()
{
    QPixmap pix(":/resources/img/011-train.svg");
    int w = ui->image->width();
    int h = ui->image->height();
    ui->image->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
}*/
