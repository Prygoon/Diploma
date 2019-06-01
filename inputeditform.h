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
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QAbstractItemModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QSqlRelationalDelegate>
#include "tracksectionproxymodel.h"

namespace Ui {
class InputEditForm;
}

class InputEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit InputEditForm(QString *sendersName, QWidget *parent = nullptr);
    ~InputEditForm();

    void setRailcarModel(QAbstractItemModel *railcarMapModel);
    void setTrackSectionModel(QAbstractItemModel *trackSectionModel);

    QDataWidgetMapper *getMapper() const;
    void hideDeleteButton();
    void showDeleteButton();
    void disableSaveButton();
    void enableSaveButton();
    void createRailcarBlankForm();
    void createTrackSectionBlankForm();

    void setWIndex(QModelIndex *value);
    QModelIndex *getWIndex() const;

    QString *getSenderName() const;

private:
    Ui::InputEditForm *ui;

    //Общий лэйаут
    //QVBoxLayout *generalVerticalLayout;

    //Лэйаут ввода типа вагона
    QHBoxLayout *railcarHorizontalLayout;
    QLabel *railcar_label;
    QSpacerItem *horizontalSpacer;
    QComboBox *railcar_comboBox;

    //Лэйаут ввода массы вагона
    QHBoxLayout *railcarMassHorizontalLayout;
    QLabel *railcarMass_label;
    QSpacerItem *railcarMassHorizontalSpacer;
    QLineEdit *railcarMass_lineEdit;

    //Лэйаут ввода доли вагона
    QHBoxLayout *railcarPercentHorizontalLayout;
    QLabel *railcarPercent_label;
    QSpacerItem *railcarPercentHorizontalSpacer;
    QLineEdit *percent_lineEdit;

    //Лэйаут ввода порядкового номера участка пути
    //QHBoxLayout *trackSectionIndexHorizontalLayout;
    //QLabel *trackSectionIndex_label;
    //QSpacerItem *trackSectionIndexHorizontalSpacer;
    //QLineEdit *trackSectionIndex_lineEdit;

    //Лэйаут ввода уклона участка пути
    QHBoxLayout *trackSectionSlopeHorizontalLayout;
    QLabel *trackSectionSlope_label;
    QSpacerItem *trackSectionSlopeHorizontalSpacer;
    QLineEdit *trackSectionSlope_lineEdit;

    //Лэйаут ввода длины участка пути
    QHBoxLayout *trackSectionLengthHorizontalLayout;
    QLabel *trackSectionLength_label;
    QSpacerItem *trackSectionLengthHorizontalSpacer;
    QLineEdit *trackSectionLength_lineEdit;

    //Лэйаут ввода длины кривой участка пути
    QHBoxLayout *trackSectionCurveLengthHorizontalLayout;
    QLabel *trackSectionCurveLength_label;
    QSpacerItem *trackSectionCurveLengthHorizontalSpacer;
    QLineEdit *trackSectionCurveLength_lineEdit;

    //Лэйаут ввода порядкового номер участка пути
    QHBoxLayout *trackSectionCurveRadiusHorizontalLayout;
    QLabel *trackSectionCurveRadius_label;
    QSpacerItem *trackSectionCurveRadiusHorizontalSpacer;
    QLineEdit *trackSectionCurveRadius_lineEdit;

    QDataWidgetMapper* mapper;
    QValidator* validator;
    QModelIndex* wIndex;
    QString *senderName;

    //Методы установки формы ввода таблицы вагонов
    void setupRaicarLayout();
    void setupRailcarMassLayout();
    void setupRailcarPercentLayout();
    void setupRailcarForm();

    //Методы установки формы ввода таблицы участков пути
    void setupTrackSectionIndexLayout();
    void setupTrackSectionSlopeLayout();
    void setupTrackSectionLengthLayout();
    void setupTrackSectionCurveLengthLayout();
    void setupTrackSectionCurveRadiusLayout();
    void setupTrackSectionForm();

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    bool isRailcarFormEmpty();
    bool isTrackSectionFormEmpty();

signals:
    void deleteButtonSignal();
    void submitTableModel();
    void revertTableModel();

    //void writeProjectId();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_delete_pushButton_clicked();
    void onRailcarFormTextEdited();
    void onTrackSectionFormTextEdited();
};

#endif // INPUTEDITFORM_H
