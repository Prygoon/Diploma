#include "inputeditform.h"
#include "ui_inputeditform.h"

InputEditForm::InputEditForm(QString *senderName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputEditForm)
{
    ui->setupUi(this);

    //generalVerticalLayout = new QVBoxLayout();
    //generalVerticalLayout->setObjectName("generalVerticalLayout");

    if(senderName == QString("pushButton_addRailcar") || senderName == QString("railcars_tableView")) {
        setupRailcarForm();
    } else {
        setupTrackSectionForm();
    }

    setWindowFlag(Qt::WindowStaysOnTopHint); // Поверх родительского окна (не работает на некоторых Линуксах)
    setModal(true);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

}

InputEditForm::~InputEditForm()
{
    delete ui;
}

void InputEditForm::setRailcarModel(QAbstractItemModel *railcarMapModel)
{
    int railcarIdIndex = static_cast<QSqlRelationalTableModel*>(railcarMapModel)->fieldIndex("nameplate");
    QSqlTableModel *relModel = static_cast<QSqlRelationalTableModel*>(railcarMapModel)->relationModel(railcarIdIndex);

    railcar_comboBox->setModel(relModel);
    railcar_comboBox->setModelColumn(relModel->fieldIndex("nameplate"));

    mapper->setModel(railcarMapModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(railcar_comboBox, 1);
    mapper->addMapping(mass_lineEdit, 2);
    mapper->addMapping(percent_lineEdit, 3);
}

void InputEditForm::setTrackSectionModel(QAbstractItemModel *trackSectionModel)
{
    mapper->setModel(trackSectionModel);
    mapper->addMapping(trackSectionIndex_lineEdit, 1);
    mapper->addMapping(trackSectionSlope_lineEdit, 2);
    mapper->addMapping(trackSectionLength_lineEdit, 3);
    mapper->addMapping(trackSectionCurveLength_lineEdit, 4);
    mapper->addMapping(trackSectionCurveRadius_lineEdit, 5);
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

void InputEditForm::createRailcarBlankForm()
{
    mass_lineEdit->setText("");
    percent_lineEdit->setText("");
}

void InputEditForm::createTrackSectionBlankForm()
{
    trackSectionIndex_lineEdit->setText("");
    trackSectionSlope_lineEdit->setText("");
    trackSectionLength_lineEdit->setText("");
    trackSectionCurveLength_lineEdit->setText("");
    trackSectionCurveRadius_lineEdit->setText("");
}

void InputEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}

QModelIndex *InputEditForm::getWIndex() const
{
    return wIndex;
}

void InputEditForm::setupRailcarForm()
{
    resize(350, 142);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(350, 142));
    setMaximumSize(QSize(350, 142));

    setupRaicarLayout();
    setupRailcarMassLayout();
    setupRailcarPercentLayout();

    if(isRailcarFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    connect(mass_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
    connect(percent_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);

    //ui->gridLayout->addLayout(ui->generalVerticalLayout, 0, 0, 1, 1);
}

void InputEditForm::setupTrackSectionForm()
{
    resize(350, 206);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(350, 206));
    setMaximumSize(QSize(350, 206));

    setupTrackSectionIndexLayout();
    setupTrackSectionSlopeLayout();
    setupTrackSectionLengthLayout();
    setupTrackSectionCurveLengthLayout();
    setupTrackSectionCurveRadiusLayout();


    if(isTrackSectionFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    //ui->gridLayout->addLayout(ui->generalVerticalLayout, 0, 0, 1, 1);
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

    ui->generalVerticalLayout->insertLayout(0, railcarHorizontalLayout);
}

void InputEditForm::setupRailcarMassLayout()
{
    massHorizontalLayout = new QHBoxLayout();
    massHorizontalLayout->setObjectName("mass_layout");

    mass_label = new QLabel(this);
    mass_label->setObjectName("mass_label");
    mass_label->setText("Масса");
    massHorizontalLayout->addWidget(mass_label);

    massHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    massHorizontalLayout->addItem(massHorizontalSpacer);

    mass_lineEdit = new QLineEdit(this);
    mass_lineEdit->setObjectName("mass_lineEdit");
    massHorizontalLayout->addWidget(mass_lineEdit);

    ui->generalVerticalLayout->insertLayout(1, massHorizontalLayout);
}

void InputEditForm::setupRailcarPercentLayout()
{
    percentHorizontalLayout = new QHBoxLayout();
    percentHorizontalLayout->setObjectName("percent_horizontalLayout");

    percent_label = new QLabel(this);
    percent_label->setObjectName("percent_label");
    percent_label->setText("Доля вагонов в составе");
    percentHorizontalLayout->addWidget(percent_label);

    percentHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    percentHorizontalLayout->addItem(percentHorizontalSpacer);

    percent_lineEdit = new QLineEdit(this);
    percent_lineEdit->setObjectName("percent_lineEdit");
    percentHorizontalLayout->addWidget(percent_lineEdit);

    ui->generalVerticalLayout->insertLayout(2, percentHorizontalLayout);
}

void InputEditForm::setupTrackSectionIndexLayout()
{
    trackSectionIndexHorizontalLayout = new QHBoxLayout();
    trackSectionIndexHorizontalLayout->setObjectName("trackSection_horizontalLayout");

    trackSectionIndex_label = new QLabel(this);
    trackSectionIndex_label->setObjectName("trackSectionIndex_label");
    trackSectionIndex_label->setText("Порядковый номер");
    trackSectionIndexHorizontalLayout->addWidget(trackSectionIndex_label);

    trackSectionIndexHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    trackSectionIndexHorizontalLayout->addItem(trackSectionIndexHorizontalSpacer);

    trackSectionIndex_lineEdit = new QLineEdit(this);
    trackSectionIndex_lineEdit->setObjectName("trackSectionIndex_lineEdit");
    trackSectionIndexHorizontalLayout->addWidget(trackSectionIndex_lineEdit);

    ui->generalVerticalLayout->insertLayout(0, trackSectionIndexHorizontalLayout);
}

void InputEditForm::setupTrackSectionSlopeLayout()
{
    trackSectionSlopeHorizontalLayout = new QHBoxLayout();
    trackSectionSlopeHorizontalLayout->setObjectName("trackSectionSlope_horizontalLayout");

    trackSectionSlope_label = new QLabel(this);
    trackSectionSlope_label->setObjectName("trackSectionSlope_label");
    trackSectionSlope_label->setText("Уклон");

    trackSectionSlopeHorizontalLayout->addWidget(trackSectionSlope_label);

    trackSectionSlopeHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    trackSectionSlopeHorizontalLayout->addItem(trackSectionSlopeHorizontalSpacer);

    trackSectionSlope_lineEdit = new QLineEdit(this);
    trackSectionSlope_lineEdit->setObjectName("trackSectionSlope_lineEdit");

    trackSectionSlopeHorizontalLayout->addWidget(trackSectionSlope_lineEdit);

    ui->generalVerticalLayout->insertLayout(1, trackSectionSlopeHorizontalLayout);
}

void InputEditForm::setupTrackSectionLengthLayout()
{
    trackSectionLengthHorizontalLayout = new QHBoxLayout();
    trackSectionLengthHorizontalLayout->setObjectName("trackSectionLength_horizontalLayout");

    trackSectionLength_label = new QLabel(this);
    trackSectionLength_label->setObjectName("trackSectionLength_label");
    trackSectionLength_label->setText("Длина");
    trackSectionLengthHorizontalLayout->addWidget(trackSectionLength_label);

    trackSectionLengthHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    trackSectionLengthHorizontalLayout->addItem(trackSectionLengthHorizontalSpacer);

    trackSectionLength_lineEdit = new QLineEdit(this);
    trackSectionLength_lineEdit->setObjectName("trackSectionLength_lineEdit");
    trackSectionLengthHorizontalLayout->addWidget(trackSectionLength_lineEdit);

    ui->generalVerticalLayout->insertLayout(2, trackSectionLengthHorizontalLayout);
}

void InputEditForm::setupTrackSectionCurveLengthLayout()
{
    trackSectionCurveLengthHorizontalLayout = new QHBoxLayout();
    trackSectionCurveLengthHorizontalLayout->setObjectName("trackSectionCurveLength_horizontalLayout");

    trackSectionCurveLength_label = new QLabel(this);
    trackSectionCurveLength_label->setObjectName("trackSectionCurveLength_label");
    trackSectionCurveLength_label->setText("Длина кривой");
    trackSectionCurveLengthHorizontalLayout->addWidget(trackSectionCurveLength_label);

    trackSectionCurveLengthHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    trackSectionCurveLengthHorizontalLayout->addItem(trackSectionCurveLengthHorizontalSpacer);

    trackSectionCurveLength_lineEdit = new QLineEdit(this);
    trackSectionCurveLength_lineEdit->setObjectName("trackSectionCurveLength_lineEdit");
    trackSectionCurveLengthHorizontalLayout->addWidget(trackSectionCurveLength_lineEdit);

    ui->generalVerticalLayout->insertLayout(3, trackSectionCurveLengthHorizontalLayout);
}

void InputEditForm::setupTrackSectionCurveRadiusLayout()
{
    trackSectionCurveRadiusHorizontalLayout = new QHBoxLayout();
    trackSectionCurveRadiusHorizontalLayout->setObjectName("trackSectionCurveRadius_horizontalLayout");

    trackSectionCurveRadius_label = new QLabel(this);
    trackSectionCurveRadius_label->setObjectName("trackSectionCurveRadius_label");
    trackSectionCurveRadius_label->setText("Радиус кривой");
    trackSectionCurveRadiusHorizontalLayout->addWidget(trackSectionCurveRadius_label);

    trackSectionCurveRadiusHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    trackSectionCurveRadiusHorizontalLayout->addItem(trackSectionCurveRadiusHorizontalSpacer);

    trackSectionCurveRadius_lineEdit = new QLineEdit(this);
    trackSectionCurveRadius_lineEdit->setObjectName("trackSectionCurveRadius_lineEdit");
    trackSectionCurveRadiusHorizontalLayout->addWidget(trackSectionCurveRadius_lineEdit);

    ui->generalVerticalLayout->insertLayout(4, trackSectionCurveRadiusHorizontalLayout);
}

void InputEditForm::closeEvent(QCloseEvent *event)
{
    emit revertTableModel();
    event->accept();
}

bool InputEditForm::isRailcarFormEmpty()
{
    return mass_lineEdit->text().isEmpty() || percent_lineEdit->text().isEmpty();
}

bool InputEditForm::isTrackSectionFormEmpty()
{
    return trackSectionIndex_lineEdit->text().isEmpty()
            || trackSectionSlope_lineEdit->text().isEmpty()
            || trackSectionLength_lineEdit->text().isEmpty()
            || trackSectionCurveLength_lineEdit->text().isEmpty()
            || trackSectionCurveRadius_lineEdit->text().isEmpty();
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
    if(isRailcarFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
    }
}
