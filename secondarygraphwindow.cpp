#include "secondarygraphwindow.h"
#include "ui_secondarygraphwindow.h"

SecondaryGraphWindow::SecondaryGraphWindow(QString *senderName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryGraphWindow),
    senderName(senderName)
{
    ui->setupUi(this);
}

SecondaryGraphWindow::~SecondaryGraphWindow()
{
    delete ui;
}

void SecondaryGraphWindow::buildFW0VGraph(const QVector<double> &FW0, const QVector<double> &velocity)
{
    ui->graphWidget->addGraph();
    ui->graphWidget->graph(0)->setData(FW0, velocity);
}

void SecondaryGraphWindow::buildW0xVGraph(const QVector<double> &W0x, const QVector<double> &velocity)
{
    ui->graphWidget->addGraph();
    ui->graphWidget->graph(1)->setData(W0x, velocity);
    ui->graphWidget->graph(1)->setPen(QPen(Qt::green));
}

void SecondaryGraphWindow::buildW0xbtVGraph(const QVector<double> &W0xbt, const QVector<double> &velocity)
{
    ui->graphWidget->addGraph();
    ui->graphWidget->graph(2)->setData(W0xbt, velocity);
    ui->graphWidget->graph(2)->setPen(QPen(Qt::red));
}

void SecondaryGraphWindow::buildFVGraph(const QVector<double> &force, const QVector<double> &velocity)
{
    ui->graphWidget->addGraph();
    ui->graphWidget->graph(0)->setData(force, velocity);
}

void SecondaryGraphWindow::enableButtons()
{
    if(this->senderName == QString("pushButtonShowDiag")) {
        emit enableShowDiagGraphButton();
    } else {
        emit enableShowTractionGraphButton();
    }
}

void SecondaryGraphWindow::closeEvent(QCloseEvent *event)
{
    enableButtons();
    event->accept();
}

void SecondaryGraphWindow::on_ClosePushButton_clicked()
{
    enableButtons();
    close();
}

void SecondaryGraphWindow::onBuildDiagGraphSignalReceived(const QVector<QVector<double> > &data)
{
    ui->graphWidget->clearGraphs();

    //ui->graphWidget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    //ui->graphWidget->axisRect()->setRangeZoom(Qt::Horizontal);
    //ui->graphWidget->axisRect()->setRangeDrag(Qt::Horizontal);
    //ui->graphWidget->setInteraction(QCP::iSelectPlottables);
    ui->graphWidget->xAxis->grid()->setZeroLinePen(QPen(Qt::SolidLine));
    ui->graphWidget->xAxis->grid()->setZeroLinePen(QPen(Qt::black));
    //ui->graphWidget->yAxis->grid()->setSubGridVisible(true);

    buildFW0VGraph(data.at(1), data.at(0));
    buildW0xVGraph(data.at(2), data.at(0));
    buildW0xbtVGraph(data.at(3), data.at(0));

    ui->graphWidget->xAxis->setRange(data.at(3).at(0) - 5, data.at(1).at(0) + 5);
    ui->graphWidget->xAxis->setRangeReversed(true);
    ui->graphWidget->xAxis->setLowerEnding(QCPLineEnding::esSpikeArrow);

    ui->graphWidget->yAxis->setRange(0, data.at(0).last() + 10);
    ui->graphWidget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->graphWidget->replot();
}

void SecondaryGraphWindow::onBuildDiagTractionSignalReceived(const QVector<QVector<double> > &data)
{
    ui->graphWidget->clearGraphs();

    buildFVGraph(data.at(0), data.at(1));

    ui->graphWidget->xAxis->setRange(0, data.at(0).last() + 10);
    ui->graphWidget->yAxis->setRange(0, data.at(1).at(0) + 50);
    ui->graphWidget->replot();
}
