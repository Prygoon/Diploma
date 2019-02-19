#ifndef INPUTEDITFORM_H
#define INPUTEDITFORM_H

#include <QDialog>
#include <QCloseEvent>
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
#include <QSqlRelationalDelegate>

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

    QDataWidgetMapper *getMapper() const;
    void hideDeleteButton();
    void showDeleteButton();
    void disableSaveButton();
    void enableSaveButton();
    void createBlankForm();

    void setWIndex(QModelIndex *value);
    QModelIndex *getWIndex() const;

private:
    Ui::InputEditForm *ui;

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
    QValidator* validator;
    QModelIndex* wIndex;

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    bool isAllLineEditsEmpty();

signals:
    void deleteLocoSignal();
    void submitTableModel();
    void revertTableModel();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_delete_pushButton_clicked();
    void onTextEdited();

};

#endif // INPUTEDITFORM_H
