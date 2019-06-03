#ifndef SECONDARYGRAPHWINDOW_H
#define SECONDARYGRAPHWINDOW_H

#include <QDialog>

namespace Ui {
class SecondaryGraphWindow;
}

class SecondaryGraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondaryGraphWindow(QWidget *parent = nullptr);
    ~SecondaryGraphWindow();

private:
    Ui::SecondaryGraphWindow *ui;

    void buildFW0VGraph(const QVector<double> &FW0, const QVector<double> &velocity);
    void buildW0xVGraph(const QVector<double> &W0x, const QVector<double> &velocity);
    void buildW0xbtVGraph(const QVector<double> &W0xbt, const QVector<double> &velocity);

    void closeEvent(QCloseEvent *event); // Переопределим нажатие на крестик

signals:
    void enableShowButton();

private slots:
    void on_ClosePushButton_clicked();

public slots:
    void onBuildDiagGraphSignalReceived(const QVector<QVector<double>> &data);

};

#endif // SECONDARYGRAPHWINDOW_H
