#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QFileDialog>
#include <QtXlsx>
#include "inputeditform.h"
#include "tracksectionproxymodel.h"
#include "database.h"

namespace Ui {
class InputWindow;
}

class InputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InputWindow(QWidget *parent = nullptr);
    ~InputWindow();

private:
    Ui::InputWindow *ui;

    InputEditForm *wInputEditForm;

    QJsonObject *dataJson;
    /* Объекты для взаимодействия с информацией в базе данных
     * и моделью представления таблицы базы данных */
    DataBase *db;
    QSqlTableModel *locomotiveModel;
    QSqlRelationalTableModel *railcarsMapModel;
    QSqlTableModel *trackSectionModel;
    TrackSectionProxyModel *proxyModel;

    QValidator *validator;

    //QFileDialog *fileDialog;
    QString dataDir;

    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView */
    void setupLocomotiveTableModel(const QString &tableName);
    void showLocomotiveComboBox();

    void setupRailcarTableModel(const QString &tableName, const QStringList &headers);
    void showRailcarTableView();

    void setupRailcarEditForm(QWidget *sender);
    void setupTracSectionEditForm(QWidget *sender);

    void setupTrackSectionModel(const QString &tableName, const QStringList &headers);
    void showTrackSectionTableView();

    void setupParamsComboboxes();

    void setupParamsValidators();

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    //void setProjectId();

    void addLocomotiveToJson();
    void addRailcarMapToJson();
    void addTrackSectionsToJson();
    void addParamsToJson();

    bool allRailcarsTypeSum();
    bool trackSectionsEnough();
    void inputCheck();

    QJsonObject objectFromString(const QString &strJson);

signals:
    void showMainWindow();  // Сигнал для первого окна на открытие
    void buildGraph(const QJsonObject &dataJson);

private slots:
    void onTextEdited(const QString &arg1);
    void onDeleteSignalRecieved();
    //void deleteTrackSection();
    void onSubmitSignalReceived();
    void onRevertSignalReceived();

    void on_fuelCheckBox_stateChanged(int arg1);
    void on_addRailcarPushButton_clicked();
    void on_addTrackSectionPushButton_clicked();
    void on_excelPushButton_clicked();
    void on_buildGraphPushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_railcarsTableView_doubleClicked(const QModelIndex &index);
    void on_trackSectionTableView_doubleClicked(const QModelIndex &index);
    void on_railcarsModelDataChanged();
    void on_trackSectionModelDataChanged();
};

#endif // INPUTWINDOW_H
