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

    //setWindowFlag(Qt::WindowStaysOnTopHint); // Поверх родительского окна (не работает на некоторых Линуксах)
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
    mapper->addMapping(railcarMass_lineEdit, 2);
    mapper->addMapping(percent_lineEdit, 3);
}

void InputEditForm::setTrackSectionModel(QAbstractItemModel *model)
{

    mapper->setModel(model);
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
    railcarMass_lineEdit->setText("");
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

    connect(railcarMass_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
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

    connect(trackSectionIndex_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
    connect(trackSectionSlope_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
    connect(trackSectionLength_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
    connect(trackSectionCurveLength_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);
    connect(trackSectionCurveRadius_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTextEdited);

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
    railcarMassHorizontalLayout = new QHBoxLayout();
    railcarMassHorizontalLayout->setObjectName("mass_layout");

    railcarMass_label = new QLabel(this);
    railcarMass_label->setObjectName("mass_label");
    railcarMass_label->setText("Масса");
    railcarMassHorizontalLayout->addWidget(railcarMass_label);

    railcarMassHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    railcarMassHorizontalLayout->addItem(railcarMassHorizontalSpacer);

    railcarMass_lineEdit = new QLineEdit(this);
    railcarMass_lineEdit->setObjectName("mass_lineEdit");
    railcarMassHorizontalLayout->addWidget(railcarMass_lineEdit);

    ui->generalVerticalLayout->insertLayout(1, railcarMassHorizontalLayout);
}

void InputEditForm::setupRailcarPercentLayout()
{
    railcarPercentHorizontalLayout = new QHBoxLayout();
    railcarPercentHorizontalLayout->setObjectName("percent_horizontalLayout");

    railcarPercent_label = new QLabel(this);
    railcarPercent_label->setObjectName("percent_label");
    railcarPercent_label->setText("Доля вагонов в составе");
    railcarPercentHorizontalLayout->addWidget(railcarPercent_label);

    railcarPercentHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    railcarPercentHorizontalLayout->addItem(railcarPercentHorizontalSpacer);

    percent_lineEdit = new QLineEdit(this);
    percent_lineEdit->setObjectName("percent_lineEdit");
    railcarPercentHorizontalLayout->addWidget(percent_lineEdit);

    ui->generalVerticalLayout->insertLayout(2, railcarPercentHorizontalLayout);
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
    return railcarMass_lineEdit->text().isEmpty() || percent_lineEdit->text().isEmpty();
}

bool InputEditForm::isTrackSectionFormEmpty()
{
    return trackSectionIndex_lineEdit->text().isEmpty()
            || trackSectionSlope_lineEdit->text().isEmpty()
            || trackSectionLength_lineEdit->text().isEmpty();
            //|| trackSectionCurveLength_lineEdit->text().isEmpty()
            //|| trackSectionCurveRadius_lineEdit->text().isEmpty();
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
    emit deleteButtonSignal();
    //mapper->submit();
    close();
}

void InputEditForm::onTextEdited()
{
    //Пока все поля не заполнены, кнопка ОК неактивна
    if(sender()->objectName().contains("railcar")){
        if(isRailcarFormEmpty()) {
            ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
        } else {
            ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
        }
    } else if (sender()->objectName().contains("trackSection")) {
        if(isTrackSectionFormEmpty()) {
            ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
        } else {
            ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
        }
    }
}
