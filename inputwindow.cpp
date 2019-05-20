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

    QModelIndex proxyIndex = proxyModel->mapToSource(index); // Тут возможно творится какая-то магия. Не трогать.
    wInputEditForm->setWIndex(new QModelIndex(proxyIndex));
    wInputEditForm->getMapper()->setCurrentModelIndex(proxyIndex);
    wInputEditForm->show();
}

void InputWindow::on_excel_pushButton_clicked()
{
    QString excelFilePath = QFileDialog::getOpenFileName(this, "Open Excel file", QDir().homePath(), "MS Excel files (*.xlsx)");
    ui->excel_lineEdit->setText(excelFilePath);

    QXlsx::Document excelDoc(excelFilePath);

    if(excelFilePath != "") {
        for (int i = 0; i < 16; i++) {
            QVariant slopeRead = excelDoc.read(3, i + 3);
            QVariant lengthRead = excelDoc.read(4, i + 3);
            QVariant curveLengthRead = excelDoc.read(5, i + 3);
            QVariant curveRadiusRead = excelDoc.read(6, i + 3);

            trackSectionModel->insertRows(i, 1);
            trackSectionModel->setData(trackSectionModel->index(i, 1), i + 1);
            trackSectionModel->setData(trackSectionModel->index(i, 2), slopeRead.toDouble());
            trackSectionModel->setData(trackSectionModel->index(i, 3), lengthRead.toInt());
            trackSectionModel->setData(trackSectionModel->index(i, 4), curveLengthRead.toInt());
            trackSectionModel->setData(trackSectionModel->index(i, 5), curveRadiusRead.toInt());
            trackSectionModel->setData(trackSectionModel->index(i, 6), projectId);
            trackSectionModel->submitAll();

            qDebug() << slopeRead << lengthRead << curveLengthRead << curveRadiusRead << projectId;
        }
    }


    //    excelDoc.write("A1", "Hello");
    //    excelDoc.write("A2", "from");
    //    excelDoc.write("A3", "my");
    //    excelDoc.write("A4", "diploma");
    //    excelDoc.write("A5", "project!");
    //    excelDoc.save();

    qDebug() << QDir().homePath();
    qDebug() << excelFilePath;
}

void InputWindow::onDeleteSignalRecieved()
{
    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
        wInputEditForm->getMapper()->submit();
        railcarsMapModel->removeRow(wInputEditForm->getWIndex()->row());
        railcarsMapModel->submitAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {

        wInputEditForm->getMapper()->submit();
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
        wInputEditForm->getMapper()->submit();
        railcarsMapModel->submitAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        setProjectId();
        wInputEditForm->getMapper()->submit();
        trackSectionModel->submitAll();
    }
}

void InputWindow::onRevertSignalReceived()
{
    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
        wInputEditForm->getMapper()->revert();
        railcarsMapModel->revertAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        wInputEditForm->getMapper()->revert();
        trackSectionModel->revertAll();
    }
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

    proxyModel = new TrackSectionProxyModel();
    proxyModel->setSourceModel(trackSectionModel);

    /* Устанавливаем названия колонок в таблице с сортировкой данных */
    for(int i = 0; i < trackSectionModel->columnCount(); i++){
        trackSectionModel->setHeaderData(i, Qt::Horizontal, headers[i]);
    }

    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    trackSectionModel->setSort(0, Qt::AscendingOrder);
    trackSectionModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void InputWindow::showTrackSectionTableView()
{
    //proxyModel = new TrackSectionProxyModel(this);
    //proxyModel->setSourceModel(trackSectionModel);   // Кладем табличку на бок
    ui->trackSection_tableView->setModel(proxyModel);     // Устанавливаем модель на TableView
    ui->trackSection_tableView->setRowHidden(0, true);       // Скрываем ряд с id записей
    //ui->trackSection_tableView->setColumnHidden(6, true);

    // Разрешаем выделение рядов
    ui->trackSection_tableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    // Устанавливаем режим выделения лишь одного ряда в таблице
    ui->trackSection_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->trackSection_tableView->resizeColumnsToContents();

    ui->trackSection_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->trackSection_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->trackSection_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->trackSection_tableView->horizontalHeader()->setMinimumSectionSize(50);

    trackSectionModel->select(); // Делаем выборку данных из таблицы
}

void InputWindow::on_pushButton_buildGraph_clicked()
{
    dataJson = new QJsonObject();
    QVariant localTmp;
    QJsonArray *slopes = new QJsonArray();
    QJsonArray *lengths = new QJsonArray();
    QJsonArray *curveLengths = new QJsonArray();
    QJsonArray *curveRadiuses = new QJsonArray();
    for (int i = 0; i < trackSectionModel->rowCount(); i++) {
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_SLOPE);
        slopes->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_LENGTH);
        lengths->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_CURVE_LENGTH);
        curveLengths->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_CURVE_RADIUS);
        curveRadiuses->push_back(QJsonValue::fromVariant(localTmp));
    }

    dataJson->insert("slopes", *slopes);
    dataJson->insert("lengths", *lengths);
    dataJson->insert("curve_lengths", *curveLengths);
    dataJson->insert("curve_radiuses", *curveRadiuses);
    dataJson->insert("lenStation", 1500); // пока фиксированное, взять из lineEdit
    //dataJson->insert("slope", );
    qDebug() << *dataJson;
    delete slopes;
    delete lengths;
    delete curveLengths;
    delete curveRadiuses;
}
