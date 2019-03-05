#ifndef TESTLOGIC_H
#define TESTLOGIC_H

#include <QWidget>

namespace Ui {
class TestLogic;
}

class TestLogic : public QWidget
{
    Q_OBJECT

public:
    explicit TestLogic(QWidget *parent = nullptr);
    ~TestLogic();

private:
    Ui::TestLogic *ui;
};

#endif // TESTLOGIC_H
