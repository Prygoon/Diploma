#include "inputwindow.h"
#include "ui_inputwindow.h"

InputWindow::InputWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);

    /* Создаем объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных */
    db = &DataBase::Instance();
    db->connectToDataBase();

    this->setupModel(SQL_QUERY_FOR_RAILCAR_MODEL,
                     QStringList() << ("Id")
                     << ("Тип вагона")
                     << ("Количество осей")
                     << ("Масса брутто")
                     << ("Доля в составе"));

    showTableView();

    wInputEditForm = new InputEditForm();
    wInputEditForm->setParent(this, Qt::Window);
}

InputWindow::~InputWindow()
{
    delete ui;
}

void InputWindow::closeEvent(QCloseEvent *event)
{
    emit showMainWindow();
    event->accept();
}

void InputWindow::on_pushButton_cancel_clicked()
{
    this->close();
    emit showMainWindow();
}

void InputWindow::on_pushButton_addRailcar_clicked()
{
    wInputEditForm->show();
}

/* Метод для инициализации модеи представления данных */
void InputWindow::setupModel(const QString &query, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице */
    railcarsMapModel = new QSqlQueryModel(this);
    railcarsMapModel->setQuery(query);

    /* Устанавливаем названия колонок в таблице с сортировкой данных */
    for(int i = 0; i < railcarsMapModel->columnCount(); i++){
        railcarsMapModel->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
}

void InputWindow::showTableView()
{
    ui->tableView->setModel(railcarsMapModel);     // Устанавливаем модель на TableView
    //ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->show();
    //railcarsMapModel->select(); // Делаем выборку данных из таблицы
}
