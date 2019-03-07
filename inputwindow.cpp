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

    /* Поля проекта определены статически для тестов
     * TODO: сделать подгрузку динамически из БД */
    projectId = 1;
    projectTitle = new QString("Test");

    this->setupRailcarTableModel(TABLE_RAILCAR_MAP_NAME,
                                 QStringList() << ("id")
                                 << ("Тип вагона")
                                 << ("Масса брутто")
                                 << ("Доля в составе")
                                 << ("project_id"));

    showRailcarTableView();

    this->setupTrackSectionModel(TABLE_TRACK_SECTION_NAME,
                                 QStringList() << ("id")
                                 << ("№ п/п")
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

    railcarsMapModel->insertRow(railcarsMapModel->rowCount(QModelIndex()));
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

    trackSectionModel->insertRow(trackSectionModel->rowCount(QModelIndex()));
    wInputEditForm->getMapper()->toLast();
}

void InputWindow::on_trackSection_tableView_doubleClicked(const QModelIndex &index)
{
    //QModelIndex proxyIndex = proxyModel->mapToSource(index);

    setupTracSectionEditForm(ui->trackSection_tableView);

    wInputEditForm->showDeleteButton();
    wInputEditForm->enableSaveButton();

    wInputEditForm->setWIndex(new QModelIndex(index));
    wInputEditForm->getMapper()->setCurrentModelIndex(index);
    wInputEditForm->show();
}

void InputWindow::onDeleteSignalRecieved()
{
    //wInputEditForm->getMapper()->submit();
    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
        railcarsMapModel->removeRow(wInputEditForm->getWIndex()->row());
        railcarsMapModel->submitAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        trackSectionModel->removeRow(wInputEditForm->getWIndex()->row());
        trackSectionModel->submitAll();
    }
}

//void InputWindow::deleteTrackSection()
//{
//    trackSectionModel->removeRow(wInputEditForm->getWIndex()->row());
//    wInputEditForm->getMapper()->submit();
//    trackSectionModel->submit();
//}

void InputWindow::onSubmitSignalReceived()
{ 
    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
        setProjectId();
        railcarsMapModel->submitAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        setProjectId();
        trackSectionModel->submitAll();
    }

    wInputEditForm->getMapper()->submit();
}

void InputWindow::onRevertSignalReceived()
{
    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
        railcarsMapModel->revertAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        trackSectionModel->revertAll();
    }
    wInputEditForm->getMapper()->revert();
}

void InputWindow::setProjectId()
{
    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
        int aRow = railcarsMapModel->rowCount() - 1;
        QModelIndex localIndex = railcarsMapModel->index(aRow, 4);
        railcarsMapModel->setData(localIndex, projectId);
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        int aRow = trackSectionModel->rowCount() - 1;
        QModelIndex localIndex = trackSectionModel->index(aRow, 6);
        trackSectionModel->setData(localIndex, projectId);
    }
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
    railcarsMapModel->setRelation(4, (QSqlRelation(TABLE_PROJECT_NAME, "id", TABLE_PROJECT_TITLE)));

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
    //ui->railcars_tableView->setColumnHidden(4, true);

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

    connect(wInputEditForm, &InputEditForm::deleteButtonSignal, this, &InputWindow::onDeleteSignalRecieved);
    connect(wInputEditForm, &InputEditForm::submitTableModel, this, &InputWindow::onSubmitSignalReceived);
    connect(wInputEditForm, &InputEditForm::revertTableModel, this, &InputWindow::onRevertSignalReceived);
    //connect(wInputEditForm, &InputEditForm::writeProjectId, this, &InputWindow::onWriteProjectIdSignalReceived);
}

void InputWindow::setupTracSectionEditForm(QWidget *sender)
{
    QString *widgetName = new QString(sender->objectName());
    wInputEditForm = new InputEditForm(widgetName);
    wInputEditForm->setParent(this, Qt::Window);
    wInputEditForm->setAttribute(Qt::WA_DeleteOnClose);
    wInputEditForm->setTrackSectionModel(trackSectionModel);

    connect(wInputEditForm, &InputEditForm::deleteButtonSignal, this, &InputWindow::onDeleteSignalRecieved);
    connect(wInputEditForm, &InputEditForm::submitTableModel, this, &InputWindow::onSubmitSignalReceived);
    connect(wInputEditForm, &InputEditForm::revertTableModel, this, &InputWindow::onRevertSignalReceived);
    //connect(wInputEditForm, &InputEditForm::writeProjectId, this, &InputWindow::onWriteProjectIdSignalReceived);
}

/* Метод для инициализации модеи представления данных участков пути */
void InputWindow::setupTrackSectionModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице */
    trackSectionModel = new QSqlRelationalTableModel(this);
    trackSectionModel->setTable(tableName);
    trackSectionModel->setRelation(6, (QSqlRelation(TABLE_PROJECT_NAME, "id", TABLE_PROJECT_TITLE)));

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
    //proxyModel = new TrackSectionProxyModel(this);
    //proxyModel->setSourceModel(trackSectionModel);   // Кладем табличку на бок
    ui->trackSection_tableView->setModel(trackSectionModel);     // Устанавливаем модель на TableView
    ui->trackSection_tableView->setColumnHidden(0, true);       // Скрываем ряд с id записей
    //ui->trackSection_tableView->setColumnHidden(6, true);

    // Разрешаем выделение рядов
    ui->trackSection_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одного ряда в таблице
    ui->trackSection_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->trackSection_tableView->resizeRowsToContents();

    ui->trackSection_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->trackSection_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->trackSection_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->trackSection_tableView->horizontalHeader()->setMinimumSectionSize(50);

    trackSectionModel->select(); // Делаем выборку данных из таблицы
}
