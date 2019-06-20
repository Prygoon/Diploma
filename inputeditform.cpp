#include "inputeditform.h"
#include "ui_inputeditform.h"

InputEditForm::InputEditForm(QString *senderName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputEditForm),
    senderName(senderName)
{
    ui->setupUi(this);

    //generalVerticalLayout = new QVBoxLayout();
    //generalVerticalLayout->setObjectName("generalVerticalLayout");

    if(this->senderName->contains("addRailcarPushButton", Qt::CaseInsensitive)
            || this->senderName->contains("railcarsTableView", Qt::CaseInsensitive)) {
        setupRailcarForm();
        setWindowTitle("Редактирование типов вагонов");
        connect(railcarPercent_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &InputEditForm::onTextEdited);

    } else {
        setupTrackSectionForm();
        setWindowTitle("Редактирование участков пути");
        connect(trackSectionSlope_lineEdit, static_cast<void (QLineEdit::*)(QString const&)>(&QLineEdit::textEdited), this, &InputEditForm::onTextEdited);
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
    mapper->addMapping(railcarPercent_lineEdit, 3);
}

void InputEditForm::setTrackSectionModel(QAbstractItemModel *TrackSectionModel)
{
    mapper->setModel(TrackSectionModel);
    //mapper->addMapping(trackSectionIndex_lineEdit, 1);
    mapper->addMapping(trackSectionSlope_lineEdit, 1);
    mapper->addMapping(trackSectionLength_lineEdit, 2);
    mapper->addMapping(trackSectionCurveLength_lineEdit, 3);
    mapper->addMapping(trackSectionCurveRadius_lineEdit, 4);
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
    railcarPercent_lineEdit->setText("");
}

void InputEditForm::createTrackSectionBlankForm()
{
    //trackSectionIndex_lineEdit->setText("");
    trackSectionSlope_lineEdit->setText("");
    trackSectionLength_lineEdit->setText("");
    trackSectionCurveLength_lineEdit->setText("0");
    trackSectionCurveRadius_lineEdit->setText("0");
}

void InputEditForm::setWIndex(QModelIndex *value)
{
    wIndex = value;
}

QModelIndex *InputEditForm::getWIndex() const
{
    return wIndex;
}

QString *InputEditForm::getSenderName() const
{
    return senderName;
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

    validator = new  QRegExpValidator(QRegExp("^[1-9]{1}[0-9]{0,20}$"), this);
    railcarMass_lineEdit->setValidator(validator);

    validator = new  QRegExpValidator(QRegExp("^(0|([1-9][0-9]*))([\\.\\,][0-9]+)?$"), this);
    railcarPercent_lineEdit->setValidator(validator);

    if(isRailcarFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    connect(railcarMass_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onRailcarFormTextEdited);
    connect(railcarPercent_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onRailcarFormTextEdited);

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

    //setupTrackSectionIndexLayout();
    setupTrackSectionSlopeLayout();
    setupTrackSectionLengthLayout();
    setupTrackSectionCurveLengthLayout();
    setupTrackSectionCurveRadiusLayout();

    validator = new  QRegExpValidator(QRegExp("^[1-9]{1}[0-9]{0,20}$"), this);
    trackSectionLength_lineEdit->setValidator(validator);
    trackSectionCurveLength_lineEdit->setValidator(validator);
    trackSectionCurveRadius_lineEdit->setValidator(validator);

    validator = new  QRegExpValidator(QRegExp("^(0|([1-9][0-9]*))([\\.\\,][0-9]+)?$"), this);
    trackSectionSlope_lineEdit->setValidator(validator);

    if(isTrackSectionFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    }

    //connect(trackSectionIndex_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTrackSectionFormTextEdited);
    connect(trackSectionSlope_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTrackSectionFormTextEdited);
    connect(trackSectionLength_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTrackSectionFormTextEdited);
    connect(trackSectionCurveLength_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTrackSectionFormTextEdited);
    connect(trackSectionCurveRadius_lineEdit, &QLineEdit::textEdited, this, &InputEditForm::onTrackSectionFormTextEdited);

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
    railcarMass_lineEdit->setPlaceholderText("в тоннах");
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

    railcarPercent_lineEdit = new QLineEdit(this);
    railcarPercent_lineEdit->setObjectName("percent_lineEdit");
    railcarPercent_lineEdit->setPlaceholderText("в долях");
    railcarPercentHorizontalLayout->addWidget(railcarPercent_lineEdit);

    ui->generalVerticalLayout->insertLayout(2, railcarPercentHorizontalLayout);
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
    trackSectionSlope_lineEdit->setPlaceholderText("в тысячных");
    trackSectionSlopeHorizontalLayout->addWidget(trackSectionSlope_lineEdit);

    ui->generalVerticalLayout->insertLayout(0, trackSectionSlopeHorizontalLayout);
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
    trackSectionLength_lineEdit->setPlaceholderText("в метрах");
    trackSectionLengthHorizontalLayout->addWidget(trackSectionLength_lineEdit);

    ui->generalVerticalLayout->insertLayout(1, trackSectionLengthHorizontalLayout);
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
    trackSectionCurveLength_lineEdit->setPlaceholderText("в метрах");
    trackSectionCurveLengthHorizontalLayout->addWidget(trackSectionCurveLength_lineEdit);

    ui->generalVerticalLayout->insertLayout(2, trackSectionCurveLengthHorizontalLayout);
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
    trackSectionCurveRadius_lineEdit->setPlaceholderText("в метрах");
    trackSectionCurveRadiusHorizontalLayout->addWidget(trackSectionCurveRadius_lineEdit);

    ui->generalVerticalLayout->insertLayout(3, trackSectionCurveRadiusHorizontalLayout);
}

void InputEditForm::closeEvent(QCloseEvent *event)
{
    emit revertTableModel();
    event->accept();
}

bool InputEditForm::isRailcarFormEmpty()
{
    return railcarMass_lineEdit->text().isEmpty() || railcarPercent_lineEdit->text().isEmpty();
}

bool InputEditForm::isTrackSectionFormEmpty()
{
    return /*trackSectionIndex_lineEdit->text().isEmpty()
                            || */trackSectionSlope_lineEdit->text().isEmpty()
            || trackSectionLength_lineEdit->text().isEmpty();
    //|| trackSectionCurveLength_lineEdit->text().isEmpty()
    //|| trackSectionCurveRadius_lineEdit->text().isEmpty();
}

void InputEditForm::onTextEdited(const QString &arg1)
{
    QString line = arg1;
    int commaIndex = arg1.indexOf(',');
    line.replace(commaIndex, 1, '.');
    static_cast<QLineEdit*>(sender())->setText(line);
}

void InputEditForm::on_buttonBox_accepted()
{
    //emit writeProjectId();
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

void InputEditForm::onRailcarFormTextEdited()
{
    //Пока все поля не заполнены, кнопка ОК неактивна
    if(isRailcarFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
    }
}

void InputEditForm::onTrackSectionFormTextEdited()
{
    if(isTrackSectionFormEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Save)->setDisabled(false);
    }
}
