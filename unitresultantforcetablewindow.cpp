#include "unitresultantforcetablewindow.h"
#include "ui_unitresultantforcetablewindow.h"

UnitResultantForceTableWindow::UnitResultantForceTableWindow(QWidget *parent, QVector<QVector<double> > *data) :
    QDialog(parent),
    ui(new Ui::UnitResultantForceTableWindow),
    data(data)
{
    ui->setupUi(this);

    model = new QStandardItemModel();
    for (int i = 0; i < data->at(0).size(); i++) {
        for (int j = 0; j < data->size(); j++) {
            item = new QStandardItem(QString::number(data->at(j).at(i), 'f', 2));
            model->setItem(j, i, item);
        }
    }

    ui->tableView->setModel(model);
    //ui->tableView->resizeRowsToContents();
    //ui->tableView->resizeColumnsToContents();

    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->verticalHeader()->setMinimumSectionSize(50);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(50);

}

UnitResultantForceTableWindow::~UnitResultantForceTableWindow()
{
    delete ui;
}

void UnitResultantForceTableWindow::closeEvent(QCloseEvent *event)
{
    emit enableShowTableButton();
    event->accept();
}

void UnitResultantForceTableWindow::on_pushButtonClose_clicked()
{
    emit enableShowTableButton();
    close();
}
