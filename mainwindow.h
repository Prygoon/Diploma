#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <inputwindow.h>

namespace Ui {
class MainWindow;
}
// test
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionClose_triggered();

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    InputWindow *wInputWindow;
};

#endif // MAINWINDOW_H

