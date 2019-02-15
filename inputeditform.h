#ifndef INPUTEDITFORM_H
#define INPUTEDITFORM_H

#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QDataWidgetMapper>
#include <QAbstractItemModel>
#include "database.h"
#include <QSqlRelationalTableModel>

namespace Ui {
class InputEditForm;
}

class InputEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit InputEditForm(QWidget *parent = nullptr);
    ~InputEditForm();

    void setModel(QAbstractItemModel *mapModel);

private:
    Ui::InputEditForm *ui;

    DataBase *db;

    //QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *railcar_label;
    QSpacerItem *horizontalSpacer;
    QComboBox *railcar_comboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *mass_label;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *mass_lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *percent_label;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *percent_lineEdit;

    QComboBox* railcarType_comboBox;
    QLabel* railcarMass_label;
    QLineEdit* railcarMass_lineEdit;
    QLabel* railcarPercent_label_;
    QLineEdit* railcarPercent_lineEdit;

    QDataWidgetMapper* mapper;

};

#endif // INPUTEDITFORM_H
