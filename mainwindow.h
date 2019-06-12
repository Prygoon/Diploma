#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QSqlTableModel>

#include "database.h"
#include "inputwindow.h"
#include "locomotivedbwindow.h"
#include "railcardbwindow.h"
#include "secondarygraphwindow.h"
#include "unitresultantforcetablewindow.h"
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
    SecondaryGraphWindow *wSecondaryGraphWindow;
    UnitResultantForceTableWindow *wUnitResultantForceTableWindow;

    QVector<QVector<double>> *secondaryData;
    //    QVector <double> pointF; // график, тяга
    //    QVector <double> pointVF; // график, скорость к тяге

    //    QVector <double> fW0Fin; // график, удельная тяга
    //    QVector <double> w0xbtFin; // график, удельное торможение
    //    QVector <double> w0xFin; // график, удельный хх

    Logic *logic;

    void disableSecondaryButtons();
    void enableSecondaryButtons();

    void buildVsGraph();
    void buildFinalBTVsGraph();
    void buildTimeGraph();
    void buildHHVsGraph();
    void buildBTVsGraph();
    void buildVCalcGraph();
    void buildMaxVelocityGraph();

    void drawTrackSections();
    void drawTrackSectionText(int length, double slopes, int position);

    void outputResults();

private slots:
    void on_action_new_triggered();
    void on_action_close_triggered();
    void on_action_loco_triggered();
    void on_action_railcars_triggered();

    void on_pushButtonTest_clicked();
    void on_pushButtonTraction_clicked();
    void on_pushButtonShowDiag_clicked();
    void on_pushButtonTable_clicked();

    void onBuildGraphSignalReceived(const QJsonObject &dataJson);
    void onEnableShowDiagGraphButtonReceived();
    void onEnableShowTractionGraphButtonReceived();
    void onEnableShowTableButtonReceived();

signals:
    void calc();
    void buildDiagGraph(const QVector<QVector<double>> &data);
    void buildTractionGraph(const QVector<QVector<double>> &data);
};

#endif // MAINWINDOW_H

