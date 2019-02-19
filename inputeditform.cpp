#include "inputeditform.h"
#include "ui_inputeditform.h"

InputEditForm::InputEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputEditForm)
{
    ui->setupUi(this);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName("verticalLayout");
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("horizontalLayout");
    railcar_label = new QLabel(this);
    railcar_label->setObjectName("railcar_label");

    horizontalLayout->addWidget(railcar_label);

    horizontalSpacer = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    railcar_comboBox = new QComboBox(this);
    railcar_comboBox->setObjectName("railcar_comboBox");




    horizontalLayout->addWidget(railcar_comboBox);


    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    mass_label = new QLabel(this);
    mass_label->setObjectName(QString::fromUtf8("mass_label"));

    horizontalLayout_2->addWidget(mass_label);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_2);

    mass_lineEdit = new QLineEdit(this);
    mass_lineEdit->setObjectName(QString::fromUtf8("mass_lineEdit"));

    horizontalLayout_2->addWidget(mass_lineEdit);


    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    percent_label = new QLabel(this);
    percent_label->setObjectName(QString::fromUtf8("percent_label"));

    horizontalLayout_3->addWidget(percent_label);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer_3);

    percent_lineEdit = new QLineEdit(this);
    percent_lineEdit->setObjectName(QString::fromUtf8("percent_lineEdit"));

    horizontalLayout_3->addWidget(percent_lineEdit);


    verticalLayout->addLayout(horizontalLayout_3);

    ui->gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


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
    int railcarId1Index = static_cast<QSqlRelationalTableModel*>(model)->fieldIndex("nameplate");
    QSqlTableModel *relModel = static_cast<QSqlRelationalTableModel*>(model)->relationModel(railcarId1Index);

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

void InputEditForm::closeEvent(QCloseEvent *event)
{
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
