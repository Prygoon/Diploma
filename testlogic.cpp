#include "testlogic.h"
#include "ui_testlogic.h"

TestLogic::TestLogic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestLogic)
{
    ui->setupUi(this);
}

TestLogic::~TestLogic()
{
    delete ui;
}
