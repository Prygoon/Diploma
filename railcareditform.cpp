#include "railcareditform.h"
#include "ui_railcareditform.h"

RailcarEditForm::RailcarEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RailcarEditForm)
{
    ui->setupUi(this);

    // Валидатор для поля ввода типа вагона
    validator = new QRegExpValidator(QRegExp("^[А-Я,а-я,0-9]+&"), this); //FIXME поправить маску
    ui->railcar_type_lineEdit->setValidator(validator);

    // Валидатор для поля ввода количества осей
    validator = new QRegExpValidator(QRegExp("^[1-9]{1}[0-9]{0,20}$"), this);
    ui->axle_count_lineEdit->setValidator(validator);

    // Валидтор для полей ввода коэффициентов
    validator = new QRegExpValidator(QRegExp("^(0|([1-9][0-9]*))([\\.\\,][0-9]+)?$"), this);
    ui->k_lineEdit->setValidator(validator);
    ui->a_lineEdit->setValidator(validator);
    ui->b_lineEdit->setValidator(validator);
    ui->c_lineEdit->setValidator(validator);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    this->setWindowFlag(Qt::WindowStaysOnTopHint); // Поверх родительского окна (не работает на некоторых Линуксах)

    if(isAllLineEditsEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    connect(ui->railcar_type_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &RailcarEditForm::onTextEdited);
    connect(ui->axle_count_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &RailcarEditForm::onTextEdited);
    connect(ui->k_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &RailcarEditForm::onTextEdited);
    connect(ui->a_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &RailcarEditForm::onTextEdited);
    connect(ui->b_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &RailcarEditForm::onTextEdited);
    connect(ui->c_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &RailcarEditForm::onTextEdited);
}

RailcarEditForm::~RailcarEditForm()
{
    delete ui;
}

void RailcarEditForm::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->railcar_type_lineEdit, 1);
    mapper->addMapping(ui->axle_count_lineEdit, 2);

    mapper->addMapping(ui->k_lineEdit, 4);
    mapper->addMapping(ui->a_lineEdit, 5);
    mapper->addMapping(ui->b_lineEdit, 6);
    mapper->addMapping(ui->c_lineEdit, 7);
}

void RailcarEditForm::on_buttonBox_accepted()
{
    QString nameplate = ui->railcar_type_lineEdit->text() + ", " + ui->axle_count_lineEdit->text() + "-осный";
    emit setNameplateData(nameplate);

    emit submitTableModel();
}

void RailcarEditForm::on_buttonBox_rejected()
{
    emit revertTableModel();
    close();
}

void RailcarEditForm::on_delete_pushButton_clicked()
{
    emit deleteRailcarSignal();
    close();
}

QDataWidgetMapper *RailcarEditForm::getMapper() const
{
    return mapper;
}

void RailcarEditForm::hideDeleteButton()
{
    ui->delete_pushButton->setHidden(true);
}

void RailcarEditForm::showDeleteButton()
{
    ui->delete_pushButton->setHidden(false);
}

void RailcarEditForm::disableSaveButton()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
}

void RailcarEditForm::enableSaveButton()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
}

void RailcarEditForm::createBlankForm()
{
    //TODO: Придумать нормальные плейсхолдеры
    ui->railcar_type_lineEdit->setText(BLANK_TEXT);
    ui->railcar_type_lineEdit->setPlaceholderText("Грузовой");

    ui->axle_count_lineEdit->setText(BLANK_TEXT);
    ui->axle_count_lineEdit->setPlaceholderText("4");

    ui->k_lineEdit->setText(BLANK_TEXT);
    ui->k_lineEdit->setPlaceholderText("0.7");

    ui->a_lineEdit->setText(BLANK_TEXT);
    ui->a_lineEdit->setPlaceholderText("8");

    ui->b_lineEdit->setText(BLANK_TEXT);
    ui->b_lineEdit->setPlaceholderText("0.1");

    ui->c_lineEdit->setText(BLANK_TEXT);
    ui->c_lineEdit->setPlaceholderText("0.0025");
}

QModelIndex *RailcarEditForm::getWIndex() const
{
    return wIndex;
}

void RailcarEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}

void RailcarEditForm::closeEvent(QCloseEvent *event)
{
    emit revertTableModel();
    event->accept();
}

bool RailcarEditForm::isAllLineEditsEmpty()
{
    return ui->railcar_type_lineEdit->text().isEmpty() \
            || ui->axle_count_lineEdit->text().isEmpty() \
            || ui->k_lineEdit->text().isEmpty() \
            || ui->a_lineEdit->text().isEmpty() \
            || ui->b_lineEdit->text().isEmpty() \
            || ui->c_lineEdit->text().isEmpty();
}

void RailcarEditForm::onTextEdited(const QString &arg1)
{
    if(static_cast<QLineEdit*>(sender())->objectName() == "railcar_type_lineEdit") {
        ui->railcar_type_lineEdit->setText(arg1.left(1).toUpper() + arg1.mid(1));
    } else {
        QString line = arg1;
        int commaIndex = arg1.indexOf(',');
        line.replace(commaIndex, 1, '.');
        static_cast<QLineEdit*>(sender())->setText(line);
    }

    //Пока все поля не заполнены, кнопка ОК неактивна
    if(isAllLineEditsEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
    }
}
