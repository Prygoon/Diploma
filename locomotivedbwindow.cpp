#include "locomotivedbwindow.h"
#include "ui_locomotivedbwindow.h"

LocomotiveDbWindow::LocomotiveDbWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocomotiveDbWindow)
{
    ui->setupUi(this);

    /* Создаем объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных */
    db = &DataBase::Instance();
    db->connectToDataBase();

    this->setupModel(TABLE_LOCO_NAME,
                     QStringList() << ("id")
                     << ("Тип локомотива")
                     << ("Расчетная сила тяги")
                     << ("Расчетная масса")
                     << ("Констр. скорость")
                     << ("Расчетная скорость"));

    showTableView();

    wLocoEditForm = new LocoEditForm();
    wLocoEditForm->setParent(this, Qt::Window);
    wLocoEditForm->setModel(model);

    connect(wLocoEditForm, SIGNAL(deleteLocoSignal()), this, SLOT(deleteLoco()));
}

LocomotiveDbWindow::~LocomotiveDbWindow()
{
    delete ui;
}

void LocomotiveDbWindow::on_pushButton_qiut_clicked()
{
    close();
    emit showMainWindow();
}

void LocomotiveDbWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    wLocoEditForm->showDeleteButton();
    QModelIndex *qIndex = new QModelIndex();
    *qIndex = index;
    wLocoEditForm->setWIndex(qIndex);
    wLocoEditForm->getMapper()->setCurrentModelIndex(index);
    wLocoEditForm->show();
    //qDebug() << "Works!!" << index.row();
}

void LocomotiveDbWindow::on_pushButton_add_clicked()
{
    model->insertRow(model->rowCount());
    wLocoEditForm->getMapper()->toLast();
    wLocoEditForm->hideDeleteButton();
    wLocoEditForm->show();
}

void LocomotiveDbWindow::deleteLoco()
{
    //wLocoEditForm->getMapper()->setCurrentModelIndex(index);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->removeRows(wLocoEditForm->getWIndex()->row(), 1);
    model->submitAll();
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

/* Метод для инициализации модеи представления данных */
void LocomotiveDbWindow::setupModel(const QString &tableName, const QStringList &headers)
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
}

void LocomotiveDbWindow::showTableView()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->setColumnWidth(1, 50);

    model->select(); // Делаем выборку данных из таблицы
}

