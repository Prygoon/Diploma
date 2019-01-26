#ifndef RAILCARDBWINDOW_H
#define RAILCARDBWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSqlTableModel>
#include <QDebug>
#include "database.h"
#include "railcareditform.h"

namespace Ui {
class RailcarDbWindow;
}

class RailcarDbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RailcarDbWindow(QWidget *parent = nullptr);
    ~RailcarDbWindow();

private:
    Ui::RailcarDbWindow *ui;
    RailcarEditForm *wRailcarEditForm;

    /* В проекте используются объекты для взаимодействия с информацией в базе данных
     * и моделью представления таблицы базы данных */
    DataBase *db;
    QSqlTableModel *model;

    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView */
    void setupModel(const QString &tableName, const QStringList &headers);
    void showTableView();

signals:
    void showMainWindow();  // Сигнал для первого окна на открытие
private slots:
    void on_pushButton_quit_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
};

#endif // RAILCARDBWINDOW_H
