#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QDialog>

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

signals:
    void showMainWindow();  // Сигнал для первого окна на открытие

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // INPUTWINDOW_H
