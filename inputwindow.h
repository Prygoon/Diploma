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
};

#endif // INPUTWINDOW_H
