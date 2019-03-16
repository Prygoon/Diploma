#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QtXlsx>
#include "inputeditform.h"
//#include "tracksectionproxymodel.h"
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

    /* Объекты для взаимодействия с информацией в базе данных
     * и моделью представления таблицы базы данных */
    DataBase *db;
    QSqlRelationalTableModel *railcarsMapModel;
    QSqlRelationalTableModel *trackSectionModel;
    //TrackSectionProxyModel *proxyModel;

    //QFileDialog *fileDialog;

    QString *projectTitle;
    int projectId;

    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView */
    void setupRailcarTableModel(const QString &tableName, const QStringList &headers);
    void showRailcarTableView();

    void setupRailcarEditForm(QWidget *sender);
    void setupTracSectionEditForm(QWidget *sender);

    void setupTrackSectionModel(const QString &tableName, const QStringList &headers);
    void showTrackSectionTableView();

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    void setProjectId();

signals:
    void showMainWindow();  // Сигнал для первого окна на открытие

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_addRailcar_clicked();
    void on_addTrackSection_pushButton_clicked();
    void on_railcars_tableView_doubleClicked(const QModelIndex &index);
    void on_trackSection_tableView_doubleClicked(const QModelIndex &index);

    void onDeleteSignalRecieved();
    //void deleteTrackSection();
    void onSubmitSignalReceived();
    void onRevertSignalReceived();
    void on_excel_pushButton_clicked();
};

#endif // INPUTWINDOW_H
