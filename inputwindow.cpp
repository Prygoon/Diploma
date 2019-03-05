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

    this->setupRailcarTableModel(TABLE_RAILCAR_MAP_NAME,
                                 QStringList() << ("id")
                                 << ("Тип вагона")
                                 << ("Масса брутто")
                                 << ("Доля в составе")
                                 << ("project_id"));

    showRailcarTableView();

    this->setupTrackSectionModel(TABLE_TRACK_SECTION_NAME,
                                 QStringList() << ("id")
                                 << ("index")
                                 << ("Уклон")
                                 << ("Длина")
                                 << ("Длина кривой")
                                 << ("Радиус кривой")
                                 << ("project_id"));

    showTrackSectionTableView();


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
    setupRailcarEditForm(ui->pushButton_addRailcar);

    wInputEditForm->hideDeleteButton();
    wInputEditForm->createRailcarBlankForm();
    wInputEditForm->disableSaveButton();
    wInputEditForm->show();
    railcarsMapModel->insertColumn(railcarsMapModel->rowCount(QModelIndex()));
    wInputEditForm->getMapper()->toLast();
}

void InputWindow::on_railcars_tableView_doubleClicked(const QModelIndex &index)
{
    setupRailcarEditForm(ui->railcars_tableView);

    wInputEditForm->showDeleteButton();
    wInputEditForm->enableSaveButton();

    wInputEditForm->setWIndex(new QModelIndex(index));
    wInputEditForm->getMapper()->setCurrentModelIndex(index);
    wInputEditForm->show();
}

void InputWindow::on_addTrackSection_pushButton_clicked()
{
    setupTracSectionEditForm(ui->addTrackSection_pushButton);

    wInputEditForm->hideDeleteButton();
    wInputEditForm->createTrackSectionBlankForm();
    wInputEditForm->disableSaveButton();
    wInputEditForm->show();

    proxyModel->insertRow(proxyModel->columnCount(QModelIndex()));
    //trackSectionModel->insertRow(trackSectionModel->rowCount(QModelIndex()));
    wInputEditForm->getMapper()->toLast();
}

void InputWindow::on_trackSection_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex proxyIndex = proxyModel->mapToSource(index);

    setupTracSectionEditForm(ui->trackSection_tableView);

    wInputEditForm->showDeleteButton();
    wInputEditForm->enableSaveButton();

    wInputEditForm->setWIndex(new QModelIndex(proxyIndex));
    wInputEditForm->getMapper()->setCurrentModelIndex(proxyIndex); //Где-то тут скрыта магия. НЕ ТРОГАТЬ!
    wInputEditForm->show();
}

void InputWindow::deleteRailcar()
{
    railcarsMapModel->removeRow(wInputEditForm->getWIndex()->row());
    wInputEditForm->getMapper()->submit();
    railcarsMapModel->submitAll();
}

void InputWindow::deleteTrackSection()
{
    proxyModel->sourceModel()->removeRow(wInputEditForm->getWIndex()->row());
    wInputEditForm->getMapper()->submit();
    proxyModel->sourceModel()->submit();
}

void InputWindow::submitModel()
{ 
    if (railcarsMapModel != nullptr) {
        railcarsMapModel->submitAll();
    } else if (proxyModel != nullptr) {
        proxyModel->submit();
    }

    wInputEditForm->getMapper()->submit();
}

void InputWindow::revertModel()
{
    wInputEditForm->getMapper()->revert();
    railcarsMapModel->revertAll();
}

/* Метод для инициализации модеи представления данных вагонов*/
void InputWindow::setupRailcarTableModel(const QString &tableName, const QStringList &headers)
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

void InputWindow::showRailcarTableView()
{
    ui->railcars_tableView->setModel(railcarsMapModel);     // Устанавливаем модель на TableView
    ui->railcars_tableView->setColumnHidden(0, true);       // Скрываем колонку с id записей
    ui->railcars_tableView->setColumnHidden(4, true);

    // Разрешаем выделение строк
    ui->railcars_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->railcars_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->railcars_tableView->resizeColumnsToContents();

    ui->railcars_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->railcars_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->show();
    railcarsMapModel->select(); // Делаем выборку данных из таблицы
}

void InputWindow::setupRailcarEditForm(QWidget *sender)
{
    QString *widgetName = new QString(sender->objectName());
    wInputEditForm = new InputEditForm(widgetName);
    wInputEditForm->setParent(this, Qt::Window);
    wInputEditForm->setAttribute(Qt::WA_DeleteOnClose);
    wInputEditForm->setRailcarModel(railcarsMapModel);

    connect(wInputEditForm, &InputEditForm::deleteButtonSignal, this, &InputWindow::deleteRailcar);
    connect(wInputEditForm, &InputEditForm::submitTableModel, this, &InputWindow::submitModel);
    connect(wInputEditForm, &InputEditForm::revertTableModel, this, &InputWindow::revertModel);
}

void InputWindow::setupTracSectionEditForm(QWidget *sender)
{
    QString *widgetName = new QString(sender->objectName());
    wInputEditForm = new InputEditForm(widgetName);
    wInputEditForm->setParent(this, Qt::Window);
    wInputEditForm->setAttribute(Qt::WA_DeleteOnClose);
    wInputEditForm->setTrackSectionModel(trackSectionModel);

    connect(wInputEditForm, &InputEditForm::deleteButtonSignal, this, &InputWindow::deleteTrackSection);
    connect(wInputEditForm, &InputEditForm::submitTableModel, this, &InputWindow::submitModel);
    connect(wInputEditForm, &InputEditForm::revertTableModel, this, &InputWindow::revertModel);
}

/* Метод для инициализации модеи представления данных участков пути */
void InputWindow::setupTrackSectionModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице */
    trackSectionModel = new QSqlRelationalTableModel(this);
    trackSectionModel->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных */
    for(int i = 0; i < trackSectionModel->columnCount(); i++){
        trackSectionModel->setHeaderData(i, Qt::Horizontal, headers[i]);
    }

    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    railcarsMapModel->setSort(0, Qt::AscendingOrder);
    railcarsMapModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void InputWindow::showTrackSectionTableView()
{
    proxyModel = new TrackSectionProxyModel(this);
    proxyModel->setSourceModel(trackSectionModel);   // Кладем табличку на бок
    ui->trackSection_tableView->setModel(proxyModel);     // Устанавливаем модель на TableView
    ui->trackSection_tableView->setRowHidden(0, true);       // Скрываем ряд с id записей
    ui->trackSection_tableView->setRowHidden(1, true);
    ui->trackSection_tableView->setRowHidden(6, true);

    // Разрешаем выделение колонок
    ui->trackSection_tableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    // Устанавливаем режим выделения лишь одной колонки в таблице
    ui->trackSection_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->trackSection_tableView->resizeColumnsToContents();

    ui->trackSection_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->trackSection_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->trackSection_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->trackSection_tableView->horizontalHeader()->setMinimumSectionSize(50);

    trackSectionModel->select(); // Делаем выборку данных из таблицы
}
