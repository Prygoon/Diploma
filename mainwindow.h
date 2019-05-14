#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QSqlTableModel>

#include "database.h"
#include "inputwindow.h"
#include "locomotivedbwindow.h"
#include "railcardbwindow.h"
#include "logic.h"
//#include "testlogic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    InputWindow *wInputWindow;
    LocomotiveDbWindow *wLocomotiveDbWindow;
    RailcarDbWindow *wRailcarDbWindow;

    Logic *logic;

private slots:
    void on_action_new_triggered();
    void on_action_close_triggered();
    void on_action_loco_triggered();
    void on_action_railcars_triggered();

    void on_pushButtonCalc_clicked();
    void on_pushButtonTest_clicked();

signals:
    void calc();

};

#endif // MAINWINDOW_H

