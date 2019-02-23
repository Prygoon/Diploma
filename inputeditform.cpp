#include "inputeditform.h"
#include "ui_inputeditform.h"

InputEditForm::InputEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputEditForm)
{
    ui->setupUi(this);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName("verticalLayout");

    if(isRailcarTableButtonsClicked) {
        setupRaicarLayout();
        setupMassLayout();
        setupPercentLayout();
    }

    ui->gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

    this->setWindowFlag(Qt::WindowStaysOnTopHint); // Поверх родительского окна (не работает на некоторых Линуксах)

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    if(isAllLineEditsEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    connect(mass_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
    connect(percent_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
}

InputEditForm::~InputEditForm()
{
    delete ui;
}

void InputEditForm::setModel(QAbstractItemModel *model)
{
    int railcarIdIndex = static_cast<QSqlRelationalTableModel*>(model)->fieldIndex("nameplate");
    QSqlTableModel *relModel = static_cast<QSqlRelationalTableModel*>(model)->relationModel(railcarIdIndex);

    //railcar_comboBox->setModelColumn(1);
    railcar_comboBox->setModel(relModel);
    railcar_comboBox->setModelColumn(relModel->fieldIndex("nameplate"));

    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(railcar_comboBox, 1);
    mapper->addMapping(mass_lineEdit, 2);
    mapper->addMapping(percent_lineEdit, 3);
}

QDataWidgetMapper *InputEditForm::getMapper() const
{
    return mapper;
}

void InputEditForm::hideDeleteButton()
{
    ui->delete_pushButton->setHidden(true);
}

void InputEditForm::showDeleteButton()
{
    ui->delete_pushButton->setHidden(false);
}

void InputEditForm::disableSaveButton()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
}

void InputEditForm::enableSaveButton()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
}

void InputEditForm::createBlankForm()
{
    mass_lineEdit->setText("");
    percent_lineEdit->setText("");
}

void InputEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}

QModelIndex *InputEditForm::getWIndex() const
{
    return wIndex;
}

void InputEditForm::setIsRailcalTableButtonsClicked(bool value)
{
    isRailcarTableButtonsClicked = value;
}

void InputEditForm::setupRaicarLayout()
{
    railcarHorizontalLayout = new QHBoxLayout();
    railcarHorizontalLayout->setObjectName("railcarLayout");

    railcar_label = new QLabel(this);
    railcar_label->setObjectName("railcar_label");
    railcar_label->setText("Тип вагона");

    railcarHorizontalLayout->addWidget(railcar_label);

    horizontalSpacer = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

    railcarHorizontalLayout->addItem(horizontalSpacer);

    railcar_comboBox = new QComboBox(this);
    railcar_comboBox->setObjectName("railcar_comboBox");

    railcarHorizontalLayout->addWidget(railcar_comboBox);

    verticalLayout->addLayout(railcarHorizontalLayout);
}

void InputEditForm::setupMassLayout()
{
    massHorizontalLayout = new QHBoxLayout();
    massHorizontalLayout->setObjectName("mass_layout");

    mass_label = new QLabel(this);
    mass_label->setObjectName(QString::fromUtf8("mass_label"));
    mass_label->setText("Масса");

    massHorizontalLayout->addWidget(mass_label);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    massHorizontalLayout->addItem(horizontalSpacer_2);

    mass_lineEdit = new QLineEdit(this);
    mass_lineEdit->setObjectName(QString::fromUtf8("mass_lineEdit"));

    massHorizontalLayout->addWidget(mass_lineEdit);

    verticalLayout->addLayout(massHorizontalLayout);
}

void InputEditForm::setupPercentLayout()
{
    percentHorizontalLayout = new QHBoxLayout();
    percentHorizontalLayout->setObjectName("horizontalLayout_3");
    percent_label = new QLabel(this);
    percent_label->setObjectName("percent_label");
    percent_label->setText("Доля вагонов в составе");

    percentHorizontalLayout->addWidget(percent_label);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    percentHorizontalLayout->addItem(horizontalSpacer_3);

    percent_lineEdit = new QLineEdit(this);
    percent_lineEdit->setObjectName("percent_lineEdit");

    percentHorizontalLayout->addWidget(percent_lineEdit);

    verticalLayout->addLayout(percentHorizontalLayout);
}

void InputEditForm::closeEvent(QCloseEvent *event)
{
    emit revertTableModel();
    event->accept();
}

bool InputEditForm::isAllLineEditsEmpty()
{
    return mass_lineEdit->text().isEmpty() || percent_lineEdit->text().isEmpty();
}



void InputEditForm::on_buttonBox_accepted()
{
    emit submitTableModel();
    //mapper->submit();
    close();
}

void InputEditForm::on_buttonBox_rejected()
{
    emit revertTableModel();
    //mapper->revert();
    close();
}

void InputEditForm::on_delete_pushButton_clicked()
{
    emit deleteLocoSignal();
    //mapper->submit();
    close();
}

void InputEditForm::onTextEdited()
{
    //Пока все поля не заполнены, кнопка ОК неактивна
    if(isAllLineEditsEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
    }
}
