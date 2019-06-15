#ifndef UNITRESULTANTFORCETABLEWINDOW_H
#define UNITRESULTANTFORCETABLEWINDOW_H

#include <QDialog>
#include <QTextDocument>
#include <QCloseEvent>
#include <QStandardItemModel>

namespace Ui {
class UnitResultantForceTableWindow;
}

class UnitResultantForceTableWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UnitResultantForceTableWindow(QWidget *parent = nullptr, QVector<QVector<double> > *data = nullptr);
    ~UnitResultantForceTableWindow();

private:
    Ui::UnitResultantForceTableWindow *ui;

    QVector<QVector<double> > *data;
    QStandardItemModel *model;
    QStandardItem *item;

    void closeEvent(QCloseEvent *event); // Переопределим нажатие на крестик

signals:
    void enableShowTableButton();
private slots:
    void on_pushButtonClose_clicked();
};

#endif // UNITRESULTANTFORCETABLEWINDOW_H
