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
    //projectId = 1;
    //projectTitle = new QString("Test");

    setupLocomotiveTableModel(TABLE_LOCO_NAME);
    showLocomotiveComboBox();

    setupRailcarTableModel(TABLE_RAILCAR_MAP_NAME,
                           QStringList() << ("id")
                           << ("Тип вагона")
                           << ("Масса брутто")
                           << ("Доля в составе"));

    showRailcarTableView();

    setupTrackSectionModel(TABLE_TRACK_SECTION_NAME,
                           QStringList() << ("id")
                           << ("Уклон")
                           << ("Длина")
                           << ("Длина кривой")
                           << ("Радиус кривой"));

    showTrackSectionTableView();

    setupParamsComboboxes();

    dataDir = QDir().homePath();
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
        //setProjectId();
        wInputEditForm->getMapper()->submit();
        railcarsMapModel->submitAll();
    }

    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
        //setProjectId();
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

//void InputWindow::setProjectId()
//{
//    if (wInputEditForm->getSenderName()->contains("railcar", Qt::CaseInsensitive)) {
//        int aRow = railcarsMapModel->rowCount() - 1;
//        QModelIndex localIndex = railcarsMapModel->index(aRow, 4);
//        railcarsMapModel->setData(localIndex, projectId);
//    }

//    if (wInputEditForm->getSenderName()->contains("trackSection", Qt::CaseInsensitive)) {
//        int aRow = trackSectionModel->rowCount() - 1;
//        QModelIndex localIndex = trackSectionModel->index(aRow, 6);
//        trackSectionModel->setData(localIndex, projectId);
//    }
//}

void InputWindow::addLocomotiveToJson()
{
    //int localCurrentComboBoxIndex = locomotiveModel->index(ui->comboBox->currentIndex(), 0).data(0).toInt();
    QVariant localTmp;
    QJsonObject *localLocomotiveJson = new QJsonObject();

    localTmp = locomotiveModel->record(ui->locoComboBox->currentIndex()).value(TABLE_LOCO_CALC_THRUST_FORCE);
    localLocomotiveJson->insert(TABLE_LOCO_CALC_THRUST_FORCE, QJsonValue::fromVariant(localTmp));
    localTmp = locomotiveModel->record(ui->locoComboBox->currentIndex()).value(TABLE_LOCO_MASS);
    localLocomotiveJson->insert(TABLE_LOCO_MASS, QJsonValue::fromVariant(localTmp));
    localTmp = locomotiveModel->record(ui->locoComboBox->currentIndex()).value(TABLE_LOCO_CONSTRUCTION_VELOCITY);
    localLocomotiveJson->insert(TABLE_LOCO_CONSTRUCTION_VELOCITY, QJsonValue::fromVariant(localTmp));
    localTmp = locomotiveModel->record(ui->locoComboBox->currentIndex()).value(TABLE_LOCO_CALC_VELOCITY);
    localLocomotiveJson->insert(TABLE_LOCO_CALC_VELOCITY, QJsonValue::fromVariant(localTmp));
    localTmp = locomotiveModel->record(ui->locoComboBox->currentIndex()).value(TABLE_LOCO_LENGTH);
    localLocomotiveJson->insert(TABLE_LOCO_LENGTH, QJsonValue::fromVariant(localTmp));
    localTmp = locomotiveModel->record(ui->locoComboBox->currentIndex()).value(TABLE_LOCO_TRACTION);
    localLocomotiveJson->insert(TABLE_LOCO_TRACTION, objectFromString(localTmp.toString()));

    dataJson->insert("locomotive", *localLocomotiveJson);
}

void InputWindow::addTrackSectionsToJson()
{
    QVariant localTmp;
    QJsonObject *localTrackSectionsJson = new QJsonObject();
    QJsonArray *localSlopes = new QJsonArray();
    QJsonArray *localLengths = new QJsonArray();
    QJsonArray *localCurveLengths = new QJsonArray();
    QJsonArray *localCurveRadiuses = new QJsonArray();

    for (int i = 0; i < trackSectionModel->rowCount(); i++) {
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_SLOPE);
        localSlopes->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_LENGTH);
        localLengths->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_CURVE_LENGTH);
        localCurveLengths->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = trackSectionModel->record(i).value(TABLE_TRACK_SECTION_CURVE_RADIUS);
        localCurveRadiuses->push_back(QJsonValue::fromVariant(localTmp));
    }

    localTrackSectionsJson->insert("slopes", *localSlopes);
    localTrackSectionsJson->insert("lengths", *localLengths);
    localTrackSectionsJson->insert("curve_lengths", *localCurveLengths);
    localTrackSectionsJson->insert("curve_radiuses", *localCurveRadiuses);
    dataJson->insert("trackSection", *localTrackSectionsJson);
}

void InputWindow::addParamsToJson()
{
    QJsonObject *localParamsJson = new QJsonObject();

    localParamsJson->insert("po_length", ui->poLengthLineEdit->text().toInt());
    localParamsJson->insert("speed_limit", ui->speedLimitLineEdit->text().toDouble());
    localParamsJson->insert("brake_pads", ui->brakePadsComboBox->currentText());
    localParamsJson->insert("path", ui->pathComboBox->currentText());

    if(ui->fuelThrustLineEdit->isEnabled() && ui->fuelNothrustLineEdit) {
        localParamsJson->insert("thrust_fuel", ui->fuelThrustLineEdit->text().toDouble());
        localParamsJson->insert("nothrust_fuel", ui->fuelNothrustLineEdit->text().toDouble());
    }

    dataJson->insert("params", *localParamsJson);
}

QJsonObject InputWindow::objectFromString(const QString &strJson)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(strJson.toUtf8());

    // Проверка валидности документа
    if(!doc.isNull()) {
        if(doc.isObject()) {
            obj = doc.object();
        } else {
            qDebug() << "Document is not an object" << endl;
        }
    } else {
        qDebug() << "Invalid JSON...\n" << strJson << endl;
    }
    return obj;
}

void InputWindow::addRailcarMapToJson()
{
    QSqlTableModel *localRailcarModel = new QSqlTableModel();
    localRailcarModel->setTable(TABLE_RAILCAR_NAME);
    localRailcarModel->select();

    QSqlTableModel *localRailcarMapModel = new QSqlTableModel();
    localRailcarMapModel->setTable(TABLE_RAILCAR_MAP_NAME);
    localRailcarMapModel->select();

    QVariant localTmp;
    QJsonObject *localRailcarsJson = new QJsonObject(); // Надо как-то достать отсюда коэфициенты
    QJsonArray *localTypes = new QJsonArray();

    QJsonObject *localJsonObject = new QJsonObject();
    QJsonArray *localJsonArray = new QJsonArray();

    QString localRailcarId;
    QString localFilter;

    for (int i = 0; i < localRailcarMapModel->rowCount(); i++) {
        localTmp = localRailcarMapModel->record(i).value(TABLE_RAILCAR_MAP_MASS);
        localJsonObject->insert(TABLE_RAILCAR_MAP_MASS, QJsonValue::fromVariant(localTmp));

        localTmp = localRailcarMapModel->record(i).value(TABLE_RAILCAR_MAP_PERCENT);
        localJsonObject->insert(TABLE_RAILCAR_MAP_PERCENT, QJsonValue::fromVariant(localTmp));

        localRailcarId = localRailcarMapModel->record(i).value("railcar_id").toString();
        localFilter = QString("id = ").append(localRailcarId);
        localRailcarModel->setFilter(localFilter);
        localTmp = localRailcarModel->record(0).value(TABLE_RAILCAR_AXLE_COUNT);
        localJsonObject->insert(TABLE_RAILCAR_AXLE_COUNT, QJsonValue::fromVariant(localTmp));

        localTmp = localRailcarModel->record(0).value(TABLE_RAILCAR_K_COEF);
        localJsonArray->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = localRailcarModel->record(0).value(TABLE_RAILCAR_A_COEF);
        localJsonArray->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = localRailcarModel->record(0).value(TABLE_RAILCAR_B_COEF);
        localJsonArray->push_back(QJsonValue::fromVariant(localTmp));
        localTmp = localRailcarModel->record(0).value(TABLE_RAILCAR_C_COEF);
        localJsonArray->push_back(QJsonValue::fromVariant(localTmp));
        localJsonObject->insert("coefs", *localJsonArray);

        localTypes->push_back(*localJsonObject);

        *localJsonArray = QJsonArray{};
    }

    localRailcarsJson->insert("types", *localTypes);
    dataJson->insert("railcars", *localRailcarsJson);
}

/* Метод для инициализации модеи представления данных локомотивов*/
void InputWindow::setupLocomotiveTableModel(const QString &tableName)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в комбобоксе */
    locomotiveModel = new QSqlTableModel();
    locomotiveModel->setTable(tableName);
    locomotiveModel->setSort(1, Qt::AscendingOrder);
}

void InputWindow::showLocomotiveComboBox()
{
    ui->locoComboBox->setModel(locomotiveModel);
    ui->locoComboBox->setModelColumn(locomotiveModel->fieldIndex("type"));
    locomotiveModel->select();
    ui->locoComboBox->setCurrentIndex(0);
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
    //railcarsMapModel->setRelation(4, (QSqlRelation(TABLE_PROJECT_NAME, "id", TABLE_PROJECT_TITLE)));

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
    ui->railcarsTableView->setModel(railcarsMapModel);     // Устанавливаем модель на TableView
    ui->railcarsTableView->setColumnHidden(0, true);       // Скрываем колонку с id записей
    //ui->railcars_tableView->setColumnHidden(4, true);

    // Разрешаем выделение строк
    ui->railcarsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->railcarsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->railcarsTableView->resizeColumnsToContents();

    ui->railcarsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->railcarsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->railcars_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    trackSectionModel = new QSqlTableModel(this);
    trackSectionModel->setTable(tableName);
    //trackSectionModel->setRelation(6, (QSqlRelation(TABLE_PROJECT_NAME, "id", TABLE_PROJECT_TITLE)));

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
    ui->trackSectionTableView->setModel(proxyModel);     // Устанавливаем модель на TableView
    ui->trackSectionTableView->setRowHidden(0, true);       // Скрываем ряд с id записей
    //ui->trackSection_tableView->setColumnHidden(6, true);

    // Разрешаем выделение рядов
    ui->trackSectionTableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    // Устанавливаем режим выделения лишь одного ряда в таблице
    ui->trackSectionTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->trackSectionTableView->resizeColumnsToContents();

    ui->trackSectionTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->trackSectionTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->trackSectionTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->trackSection_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->trackSection_tableView->horizontalHeader()->setMinimumSectionSize(50);

    trackSectionModel->select(); // Делаем выборку данных из таблицы
}

void InputWindow::setupParamsComboboxes()
{
    QStringList items;
    items << "Композиционные" << "Чугунные";
    ui->brakePadsComboBox->addItems(items);
    ui->brakePadsComboBox->setCurrentIndex(0);

    items.clear();
    items << "Звеньевой" << "Бесстыковой";
    ui->pathComboBox->addItems(items);
    ui->pathComboBox->setCurrentIndex(0);
}

void InputWindow::on_fuelCheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::CheckState::Unchecked) {
        ui->fuelThrustLineEdit->setDisabled(true);
        ui->fuelNothrustLineEdit->setDisabled(true);
    } else if(arg1 == Qt::CheckState::Checked) {
        ui->fuelThrustLineEdit->setEnabled(true);
        ui->fuelNothrustLineEdit->setEnabled(true);
    }
}

void InputWindow::on_addRailcarPushButton_clicked()
{
    setupRailcarEditForm(ui->addRailcarPushButton);

    wInputEditForm->hideDeleteButton();
    wInputEditForm->createRailcarBlankForm();
    wInputEditForm->disableSaveButton();
    wInputEditForm->show();

    railcarsMapModel->insertRow(railcarsMapModel->rowCount(QModelIndex()));
    wInputEditForm->getMapper()->toLast();
}

void InputWindow::on_addTrackSectionPushButton_clicked()
{
    setupTracSectionEditForm(ui->addTrackSectionPushButton);

    wInputEditForm->hideDeleteButton();
    wInputEditForm->createTrackSectionBlankForm();
    wInputEditForm->disableSaveButton();
    wInputEditForm->show();

    trackSectionModel->insertRow(trackSectionModel->rowCount(QModelIndex()));
    wInputEditForm->getMapper()->toLast();
}

void InputWindow::on_excelPushButton_clicked()
{
    QString excelFilePath = QFileDialog::getOpenFileName(this, "Open Excel file", dataDir, "MS Excel files (*.xlsx)");
    ui->excelLineEdit->setText(excelFilePath);
    ui->excelLineEdit->setToolTip(excelFilePath);
    dataDir = excelFilePath;

    QXlsx::Document excelDoc(excelFilePath);

    if(excelFilePath != "") {
        //trackSectionModel->clear();
        trackSectionModel->removeRows(0, trackSectionModel->rowCount());
        trackSectionModel->submitAll();

        for (int i = 0; i < 16; i++) {
            QVariant slopeRead = excelDoc.read(3, i + 3);
            QVariant lengthRead = excelDoc.read(4, i + 3);
            QVariant curveLengthRead = excelDoc.read(5, i + 3);
            QVariant curveRadiusRead = excelDoc.read(6, i + 3);

            trackSectionModel->insertRows(i, 1);
            //trackSectionModel->setData(trackSectionModel->index(i, 1), i + 1);
            trackSectionModel->setData(trackSectionModel->index(i, 1), slopeRead.toDouble());
            trackSectionModel->setData(trackSectionModel->index(i, 2), lengthRead.toInt());
            trackSectionModel->setData(trackSectionModel->index(i, 3), curveLengthRead.toInt());
            trackSectionModel->setData(trackSectionModel->index(i, 4), curveRadiusRead.toInt());
            trackSectionModel->submitAll();

            qDebug() << slopeRead << lengthRead << curveLengthRead << curveRadiusRead;
        }
    }


    //    excelDoc.write("A1", "Hello");
    //    excelDoc.write("A2", "from");
    //    excelDoc.write("A3", "my");
    //    excelDoc.write("A4", "diploma");
    //    excelDoc.write("A5", "project!");
    //    excelDoc.save();

    qDebug() << dataDir << endl << excelFilePath;
}

void InputWindow::on_buildGraphPushButton_clicked()
{
    dataJson = new QJsonObject();

    addLocomotiveToJson();
    addTrackSectionsToJson();
    addRailcarMapToJson();
    addParamsToJson();

    //qDebug() << *dataJson;
    emit buildGraph(*dataJson);
    this->close();
    emit showMainWindow();
    //dataJson->insert("lenStation", 1500); // пока фиксированное, взять из lineEdit
    //dataJson->insert("slope", );
    //delete slopes;
    //delete lengths;
    //delete curveLengths;
    //delete curveRadiuses;
}

void InputWindow::on_cancelPushButton_clicked()
{
    this->close();
    emit showMainWindow();
}

void InputWindow::on_railcarsTableView_doubleClicked(const QModelIndex &index)
{
    setupRailcarEditForm(ui->railcarsTableView);

    wInputEditForm->showDeleteButton();
    wInputEditForm->enableSaveButton();

    wInputEditForm->setWIndex(new QModelIndex(index));
    wInputEditForm->getMapper()->setCurrentModelIndex(index);
    wInputEditForm->show();
}

void InputWindow::on_trackSectionTableView_doubleClicked(const QModelIndex &index)
{
    //QModelIndex proxyIndex = proxyModel->mapToSource(index);
    setupTracSectionEditForm(ui->trackSectionTableView);

    wInputEditForm->showDeleteButton();
    wInputEditForm->enableSaveButton();

    QModelIndex proxyIndex = proxyModel->mapToSource(index); // Тут возможно творится какая-то магия. Не трогать.
    wInputEditForm->setWIndex(new QModelIndex(proxyIndex));
    wInputEditForm->getMapper()->setCurrentModelIndex(proxyIndex);
    wInputEditForm->show();
}
