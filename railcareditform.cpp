#include "railcareditform.h"
#include "ui_railcareditform.h"

RailcarEditForm::RailcarEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RailcarEditForm)
{
    ui->setupUi(this);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

RailcarEditForm::~RailcarEditForm()
{
    delete ui;
}

void RailcarEditForm::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->railcar_type_lineEdit, 1);
    mapper->addMapping(ui->axle_count_lineEdit, 2);
    mapper->addMapping(ui->k_lineEdit, 3);
    mapper->addMapping(ui->a_lineEdit, 4);
    mapper->addMapping(ui->b_lineEdit, 5);
    mapper->addMapping(ui->c_lineEdit, 6);
}

QDataWidgetMapper *RailcarEditForm::getMapper() const
{
    return mapper;
}

void RailcarEditForm::on_buttonBox_accepted()
{
    mapper->submit();
}

void RailcarEditForm::on_buttonBox_rejected()
{
    close();
}
