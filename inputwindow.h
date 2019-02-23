#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QSqlQueryModel>
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

    /* Объекты для взаимодействия с информацией в базе данных
     * и моделью представления таблицы базы данных */
    DataBase *db;
    QSqlRelationalTableModel *railcarsMapModel;
    QSqlRelationalTableModel *trackModel;

    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView */
    void setupRailcarTableModel(const QString &tableName, const QStringList &headers);
    void showRailcarTableView();

    void setupInputEditForm();
    void setupTrackModel(const QString &tableName, const QStringList &headers);
    void showTrackTableView();

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна

signals:
    void showMainWindow();  // Сигнал для первого окна на открытие

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_addRailcar_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);

    void deleteLoco();
    void submitModel();
    void revertModel();
};

#endif // INPUTWINDOW_H
