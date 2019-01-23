#ifndef LOCOMOTIVEDBWINDOW_H
#define LOCOMOTIVEDBWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSqlTableModel>
#include <QDebug>
#include "database.h"

namespace Ui {
class LocomotiveDbWindow;
}

class LocomotiveDbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LocomotiveDbWindow(QWidget *parent = nullptr);
    ~LocomotiveDbWindow();

private slots:
    void on_pushButton_qiut_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::LocomotiveDbWindow *ui;

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
};


#endif // LOCOMOTIVEDBWINDOW_H
