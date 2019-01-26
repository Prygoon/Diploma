#include "locoeditform.h"
#include "ui_locoeditform.h"

LocoEditForm::LocoEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocoEditForm)
{
    ui->setupUi(this);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

LocoEditForm::~LocoEditForm()
{
    delete ui;
}

void LocoEditForm::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->loco_type_lineEdit, 1);
    mapper->addMapping(ui->mass_lineEdit, 2);
    mapper->addMapping(ui->thrust_force_lineEdit, 3);
    mapper->addMapping(ui->constr_velocity_lineEdit, 4);
    mapper->addMapping(ui->calc_velocity_lineEdit, 5);
}

void LocoEditForm::on_buttonBox_accepted()
{
    mapper->submit();
}

void LocoEditForm::on_buttonBox_rejected()
{
    close();
}

QDataWidgetMapper *LocoEditForm::getMapper() const
{
    return mapper;
}
