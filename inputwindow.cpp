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

    this->setupModel(TABLE_RAILCAR_MAP_NAME,
                     QStringList() << ("id")
                     << ("Тип вагона")
                     << ("Масса брутто")
                     << ("Доля в составе"));

    showTableView();

    wInputEditForm = new InputEditForm();
    wInputEditForm->setParent(this, Qt::Window);
    wInputEditForm->setModel(railcarsMapModel);

    connect(wInputEditForm, &InputEditForm::deleteLocoSignal, this, &InputWindow::deleteLoco);
    connect(wInputEditForm, &InputEditForm::submitTableModel, this, &InputWindow::submitModel);
    connect(wInputEditForm, &InputEditForm::revertTableModel, this, &InputWindow::revertModel);
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
    wInputEditForm->hideDeleteButton();
    wInputEditForm->createBlankForm();
    wInputEditForm->disableSaveButton();
    wInputEditForm->show();
    railcarsMapModel->insertRow(railcarsMapModel->rowCount(QModelIndex()));
    wInputEditForm->getMapper()->toLast();
}

void InputWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    wInputEditForm->showDeleteButton();
    wInputEditForm->enableSaveButton();
    wInputEditForm->setWIndex(new QModelIndex(index));
    wInputEditForm->getMapper()->setCurrentModelIndex(index);
    wInputEditForm->show();
}

void InputWindow::deleteLoco()
{
    railcarsMapModel->removeRow(wInputEditForm->getWIndex()->row());
    wInputEditForm->getMapper()->submit();
    railcarsMapModel->submitAll();
}

void InputWindow::submitModel()
{
    wInputEditForm->getMapper()->submit();
    railcarsMapModel->submitAll();
}

void InputWindow::revertModel()
{
    wInputEditForm->getMapper()->revert();
    railcarsMapModel->revertAll();
}

/* Метод для инициализации модеи представления данных */
void InputWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице */
    railcarsMapModel = new QSqlRelationalTableModel(this);
    railcarsMapModel->setTable(tableName);
    railcarsMapModel->setRelation(1, (QSqlRelation(TABLE_RAILCAR_NAME, "id", TABLE_RAILCAR_NAMEPLATE)));

    /* Устанавливаем названия колонок в таблице с сортировкой данных */
    for(int i = 0; i < railcarsMapModel->columnCount(); i++){
        railcarsMapModel->setHeaderData(i, Qt::Horizontal, headers[i]);
    }

    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    railcarsMapModel->setSort(0, Qt::AscendingOrder);
    railcarsMapModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void InputWindow::showTableView()
{
    ui->tableView->setModel(railcarsMapModel);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);       // Скрываем колонку с id записей

    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->show();
    railcarsMapModel->select(); // Делаем выборку данных из таблицы
}
