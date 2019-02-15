#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#define SQL_QUERY_FOR_RAILCAR_MODEL "SELECT type, axle_count, mass, percent FROM railcars rc INNER JOIN railcars_map rc_m ON rc.id = rc_m.railcar_id;"

#include <QDialog>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include "inputeditform.h"
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
    QSqlQueryModel *railcarsMapModel;

    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView */
    void setupModel(const QString &tableName, const QStringList &headers);
    void showTableView();

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна

signals:
    void showMainWindow();  // Сигнал для первого окна на открытие

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_addRailcar_clicked();
};

#endif // INPUTWINDOW_H
