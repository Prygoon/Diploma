#ifndef LOCOEDITFORM_H
#define LOCOEDITFORM_H

#include <QDialog>
#include <QDataWidgetMapper>

namespace Ui {
class LocoEditForm;
}

class LocoEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit LocoEditForm(QWidget *parent = nullptr);
    ~LocoEditForm();

    void setModel(QAbstractItemModel *model);

    QDataWidgetMapper *getMapper() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LocoEditForm *ui;
    QDataWidgetMapper* mapper;
};

#endif // LOCOEDITFORM_H
