#include "locoeditform.h"
#include "ui_locoeditform.h"

LocoEditForm::LocoEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocoEditForm)
{
    ui->setupUi(this);

    validator = new  QRegExpValidator(QRegExp("^\\d{,3}[А-Я,а-я]{1,4}\\d{1,4}[А-Я,а-я]{,5}$"), this);
    ui->loco_type_lineEdit->setValidator(validator);

    validator = new  QRegExpValidator(QRegExp("^[1-9]{1}[0-9]{0,20}$"), this);
    ui->thrust_force_lineEdit->setValidator(validator);
    ui->mass_lineEdit->setValidator(validator);
    ui->constr_velocity_lineEdit->setValidator(validator);

    validator = new  QRegExpValidator(QRegExp("^(0|([1-9][0-9]*))([\\.\\,][0-9]+)?$"), this);
    ui->calc_velocity_lineEdit->setValidator(validator);
    ui->length_lineEdit->setValidator(validator);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    //this->setWindowFlag(Qt::WindowStaysOnTopHint); // Поверх родительского окна (не работает на некоторых Линуксах)

    if(isAllLineEditsEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    connect(ui->loco_type_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &LocoEditForm::onTextEdited);
    connect(ui->thrust_force_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &LocoEditForm::onTextEdited);
    connect(ui->mass_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &LocoEditForm::onTextEdited);
    connect(ui->constr_velocity_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &LocoEditForm::onTextEdited);
    connect(ui->calc_velocity_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &LocoEditForm::onTextEdited);
    connect(ui->length_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &LocoEditForm::onTextEdited);
}

LocoEditForm::~LocoEditForm()
{
    delete ui;
}

void LocoEditForm::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->loco_type_lineEdit, 1);
    mapper->addMapping(ui->thrust_force_lineEdit, 2);
    mapper->addMapping(ui->mass_lineEdit, 3);
    mapper->addMapping(ui->constr_velocity_lineEdit, 4);
    mapper->addMapping(ui->calc_velocity_lineEdit, 5);
    mapper->addMapping(ui->length_lineEdit, 6);
}

void LocoEditForm::on_buttonBox_accepted()
{    
    emit submitTableModel();
}

void LocoEditForm::on_buttonBox_rejected()
{
    emit revertTableModel();
    close();
}

void LocoEditForm::on_delete_pushButton_clicked()
{
    emit deleteLocoSignal();
    close();
}

void LocoEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}

QModelIndex *LocoEditForm::getWIndex() const
{
    return wIndex;
}

void LocoEditForm::closeEvent(QCloseEvent *event)
{
    emit revertTableModel();
    event->accept();
}

bool LocoEditForm::isAllLineEditsEmpty()
{
    return ui->loco_type_lineEdit->text().isEmpty() \
            || ui->thrust_force_lineEdit->text().isEmpty() \
            || ui->mass_lineEdit->text().isEmpty() \
            || ui->constr_velocity_lineEdit->text().isEmpty() \
            || ui->calc_velocity_lineEdit->text().isEmpty() \
            || ui->length_lineEdit->text().isEmpty();
}


QDataWidgetMapper *LocoEditForm::getMapper() const
{
    return mapper;
}

void LocoEditForm::hideDeleteButton()
{
    ui->delete_pushButton->setHidden(true);
}

void LocoEditForm::showDeleteButton()
{
    ui->delete_pushButton->setHidden(false);
}

void LocoEditForm::disableSaveButton()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
}

void LocoEditForm::enableSaveButton()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
}

void LocoEditForm::createBlankForm()
{
    ui->loco_type_lineEdit->setText(BLANK_TEXT);
    ui->loco_type_lineEdit->setPlaceholderText("4ТЭ10С");
    ui->thrust_force_lineEdit->setText(BLANK_TEXT);
    ui->thrust_force_lineEdit->setPlaceholderText("в кН");
    ui->mass_lineEdit->setText(BLANK_TEXT);
    ui->mass_lineEdit->setPlaceholderText("в тоннах");
    ui->constr_velocity_lineEdit->setText(BLANK_TEXT);
    ui->constr_velocity_lineEdit->setPlaceholderText("в км/ч");
    ui->calc_velocity_lineEdit->setText(BLANK_TEXT);
    ui->calc_velocity_lineEdit->setPlaceholderText("в км/ч");
    ui->length_lineEdit->setText(BLANK_TEXT);
    ui->length_lineEdit->setPlaceholderText("в метрах");

}

void LocoEditForm::onTextEdited(const QString &arg1)
{
    if(static_cast<QLineEdit*>(sender())->objectName() == "loco_type_lineEdit") {
        ui->loco_type_lineEdit->setText(arg1.toUpper());
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
