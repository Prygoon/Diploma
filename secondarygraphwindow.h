#ifndef SECONDARYGRAPHWINDOW_H
#define SECONDARYGRAPHWINDOW_H

#include <QDialog>
#include <QDir>

namespace Ui {
class SecondaryGraphWindow;
}

class SecondaryGraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondaryGraphWindow(QString *senderName, QWidget *parent = nullptr);
    ~SecondaryGraphWindow();

private:
    Ui::SecondaryGraphWindow *ui;

    QString *senderName;

    /* Диаграммы */
    void buildFW0VGraph(const QVector<double> &FW0, const QVector<double> &velocity);
    void buildW0xVGraph(const QVector<double> &W0x, const QVector<double> &velocity);
    void buildW0xbtVGraph(const QVector<double> &W0xbt, const QVector<double> &velocity);

    /* Тяговая характеристика */
    void buildFVGraph(const QVector<double> &force, const QVector<double> &velocity);

    void enableButton();
    void closeEvent(QCloseEvent *event); // Переопределим нажатие на крестик

signals:
    void enableShowDiagGraphButton();
    void enableShowTractionGraphButton();

private slots:
    void on_ClosePushButton_clicked();

    void on_saveGraphPushButton_clicked();

public slots:
    void onBuildDiagGraphSignalReceived(const QVector<QVector<double>> &data);
    void onBuildTractionGraphSignalReceived(const QVector<QVector<double>> &data);
};

#endif // SECONDARYGRAPHWINDOW_H
