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
    //mapper->submit();
    emit submitTableModel();
}

void LocoEditForm::on_buttonBox_rejected()
{
    //mapper->revert();
    emit revertTableModel();
    close();
}

void LocoEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}

QModelIndex *LocoEditForm::getWIndex() const
{
    return wIndex;
}


QDataWidgetMapper *LocoEditForm::getMapper() const
{
    return mapper;
}

void LocoEditForm::hideDeleteButton()
{
    ui->delete_pushButton->setHidden(true);
}

void LocoEditForm::showDeleteButton()
{
    ui->delete_pushButton->setHidden(false);
}

void LocoEditForm::createBlankForm()
{
    ui->loco_type_lineEdit->setText("");
    ui->thrust_force_lineEdit->setText("");
    ui->mass_lineEdit->setText("");
    ui->constr_velocity_lineEdit->setText("");
    ui->calc_velocity_lineEdit->setText("");
}

void LocoEditForm::on_delete_pushButton_clicked()
{
    emit deleteLocoSignal();
    close();
}
