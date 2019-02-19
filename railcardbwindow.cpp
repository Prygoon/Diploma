#include "railcardbwindow.h"
#include "ui_railcardbwindow.h"

RailcarDbWindow::RailcarDbWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RailcarDbWindow)
{
    ui->setupUi(this);

    /* Создаем объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных */
    db = &DataBase::Instance();
    db->connectToDataBase();

    this->setupModel(TABLE_RAILCAR_NAME,
                     QStringList() << ("id")
                     << ("Тип вагона")
                     << ("Количество осей")
                     << ("nameplate")
                     << ("k")
                     << ("a")
                     << ("b")
                     << ("c"));

    showTableView();

    wRailcarEditForm = new RailcarEditForm();
    wRailcarEditForm->setParent(this, Qt::Window);
    wRailcarEditForm->setModel(model);

    connect(wRailcarEditForm, &RailcarEditForm::deleteRailcarSignal, this, &RailcarDbWindow::deleteRailcar);
    connect(wRailcarEditForm, &RailcarEditForm::submitTableModel, this, &RailcarDbWindow::submitModel);
    connect(wRailcarEditForm, &RailcarEditForm::revertTableModel, this, &RailcarDbWindow::revertModel);
    connect(wRailcarEditForm, static_cast<void (RailcarEditForm::*)(QVariant const&)>(&RailcarEditForm::setNameplateData), this, &RailcarDbWindow::setNameplateData);
}

RailcarDbWindow::~RailcarDbWindow()
{
    delete ui;
}

void RailcarDbWindow::on_pushButton_quit_clicked()
{
    this->close();
    emit showMainWindow();
}

void RailcarDbWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    wRailcarEditForm->showDeleteButton();
    wRailcarEditForm->enableSaveButton();
    wRailcarEditForm->setWIndex(new QModelIndex(index));
    wRailcarEditForm->getMapper()->setCurrentModelIndex(index);
    wRailcarEditForm->show();
    //qDebug() << "Works!!" << index.row();
}

void RailcarDbWindow::on_pushButton_add_clicked()
{
    wRailcarEditForm->hideDeleteButton();
    wRailcarEditForm->createBlankForm();
    wRailcarEditForm->disableSaveButton();
    wRailcarEditForm->show();
    model->insertRow(model->rowCount(QModelIndex()));

    wRailcarEditForm->getMapper()->toLast();
}

void RailcarDbWindow::deleteRailcar()
{
    model->removeRow(wRailcarEditForm->getWIndex()->row());
    model->submitAll();
}

void RailcarDbWindow::submitModel()
{
    wRailcarEditForm->getMapper()->submit();
    model->submitAll();
}

void RailcarDbWindow::revertModel()
{
    wRailcarEditForm->getMapper()->revert();
    model->revertAll();
}

void RailcarDbWindow::setNameplateData(const QVariant &data)
{
    int aRow = model->rowCount() - 1;
    QModelIndex localIndex = model->index(aRow, 3);
    //model.
    model->setData(localIndex, data);
}

/* Метод для инициализации модеи представления данных */
void RailcarDbWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице */
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных */
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0, Qt::AscendingOrder);

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void RailcarDbWindow::showTableView()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    ui->tableView->setColumnHidden(3, true);
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    model->select(); // Делаем выборку данных из таблицы
}

void RailcarDbWindow::closeEvent(QCloseEvent *event)
{
    emit showMainWindow();
    event->accept();
}
