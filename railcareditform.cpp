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

void RailcarEditForm::on_buttonBox_accepted()
{
    emit submitTableModel();
}

void RailcarEditForm::on_buttonBox_rejected()
{
    emit revertTableModel();
    close();
}

void RailcarEditForm::on_delete_pushButton_clicked()
{
    emit deleteRailcarSignal();
    close();
}

QDataWidgetMapper *RailcarEditForm::getMapper() const
{
    return mapper;
}

void RailcarEditForm::hideDeleteButton()
{
    ui->delete_pushButton->setHidden(true);
}

void RailcarEditForm::showDeleteButton()
{
    ui->delete_pushButton->setHidden(false);
}

void RailcarEditForm::createBlankForm()
{
    ui->railcar_type_lineEdit->setText("");
    ui->axle_count_lineEdit->setText("");
    ui->k_lineEdit->setText("");
    ui->a_lineEdit->setText("");
    ui->b_lineEdit->setText("");
    ui->c_lineEdit->setText("");
}

QModelIndex *RailcarEditForm::getWIndex() const
{
    return wIndex;
}

void RailcarEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}
