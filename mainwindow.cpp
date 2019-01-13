#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wInputWindow = new InputWindow(this);

    //подключаем к слоту запуска главного окна по кнопке во окне ввода
    connect(wInputWindow, SIGNAL(showMainWindow()), this, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_actionNew_triggered()
{
    wInputWindow->show();
    this->hide();
}
