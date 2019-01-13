#include "inputwindow.h"
#include "ui_inputwindow.h"

InputWindow::InputWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
}

InputWindow::~InputWindow()
{
    delete ui;
}

void InputWindow::on_buttonBox_accepted()
{
    this->close();
    emit showMainWindow();
}

void InputWindow::on_buttonBox_rejected()
{
    this->close();
    emit showMainWindow();
}
