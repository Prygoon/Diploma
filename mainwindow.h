#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QSqlTableModel>

#include "database.h"
#include "inputwindow.h"
#include "locomotivedbwindow.h"
#include "railcardbwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_action_new_triggered();
    void on_action_close_triggered();
    void on_action_loco_triggered();

    void on_railcars_triggered();

private:
    Ui::MainWindow *ui;
    InputWindow *wInputWindow;
    LocomotiveDbWindow *wLocomotiveDbWindow;
    RailcarDbWindow *wRailcarDbWindow;

};

#endif // MAINWINDOW_H

