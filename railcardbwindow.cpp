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
                     << ("k")
                     << ("a")
                     << ("b")
                     << ("c"));

    showTableView();
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
    qDebug() << "Works!!" << index.row();
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
}

void RailcarDbWindow::showTableView()
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
