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
                     << ("Расчетная скорость")
                     << ("Длина локомотива")
                     << ("Тяговая характеристика"));

    showTableView();

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
    setupLocoEditForm();

    wLocoEditForm->showDeleteButton();
    wLocoEditForm->enableSaveButton();
    wLocoEditForm->setWIndex(new QModelIndex(index));
    wLocoEditForm->getMapper()->setCurrentModelIndex(index);
    wLocoEditForm->setEditOrNewCheck(true);
    wLocoEditForm->show();
    //qDebug() << "Works!!" << index.row();
}

void LocomotiveDbWindow::on_pushButton_add_clicked()
{
    setupLocoEditForm();

    wLocoEditForm->hideDeleteButton();
    wLocoEditForm->createBlankForm();
    wLocoEditForm->disableSaveButton();
    wLocoEditForm->setEditOrNewCheck(false);
    wLocoEditForm->show();
    model->insertRow(model->rowCount(QModelIndex()));
    wLocoEditForm->getMapper()->toLast();
}

void LocomotiveDbWindow::onDeleteLocoSignalReceived()
{
    model->removeRow(wLocoEditForm->getWIndex()->row());
    model->submitAll();
}

void LocomotiveDbWindow::onSubmitModelSignalReceived(const QString &strTractionJson)
{
    QModelIndex *index = wLocoEditForm->getWIndex();
    //qDebug() << "Index" << model->index();
    if(index != nullptr) {
        model->setData(model->index(wLocoEditForm->getWIndex()->row(), 7), strTractionJson);
    } else {
        model->insertRows(model->rowCount() + 1, 1);
        model->setData(model->index(model->rowCount() - 1, 7), strTractionJson);
    }

    wLocoEditForm->getMapper()->submit();
    model->submitAll();
}

void LocomotiveDbWindow::onRevertModelSignalReceived()
{
    wLocoEditForm->getMapper()->revert();
    model->revertAll();
}

/* Метод для инициализации модеи представления данных */
void LocomotiveDbWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице */
    model = new QSqlRelationalTableModel(this);
    model->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных */
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0, Qt::AscendingOrder);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void LocomotiveDbWindow::showTableView()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    ui->tableView->setColumnHidden(7, true);    // Скрываем колонку с тяговой характеристикой
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

void LocomotiveDbWindow::setupLocoEditForm()
{
    wLocoEditForm = new LocoEditForm();
    wLocoEditForm->setParent(this, Qt::Window);
    wLocoEditForm->setAttribute(Qt::WA_DeleteOnClose);
    wLocoEditForm->setModal(true);
    wLocoEditForm->setModel(model);

    connect(wLocoEditForm, &LocoEditForm::deleteLocoSignal, this, &LocomotiveDbWindow::onDeleteLocoSignalReceived);
    connect(wLocoEditForm, static_cast<void (LocoEditForm::*)(QString const&)>(&LocoEditForm::submitTableModel), this, &LocomotiveDbWindow::onSubmitModelSignalReceived);
    connect(wLocoEditForm, &LocoEditForm::revertTableModel, this, &LocomotiveDbWindow::onRevertModelSignalReceived);
}

void LocomotiveDbWindow::closeEvent(QCloseEvent *event)
{
    emit showMainWindow();
    event->accept();
}

