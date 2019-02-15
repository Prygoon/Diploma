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
    railcar_comboBox->setObjectName(QString::fromUtf8("railcar_comboBox"));
    railcar_comboBox->setModelColumn(1);

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
}

InputEditForm::~InputEditForm()
{
    delete ui;
}

void InputEditForm::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(railcarType_comboBox, 1);
    mapper->addMapping(railcarMass_lineEdit, 2);
    mapper->addMapping(railcarPercent_lineEdit, 3);
}


