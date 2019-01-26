#ifndef RAILCAREDITFORM_H
#define RAILCAREDITFORM_H

#include <QDialog>
#include <QDataWidgetMapper>

namespace Ui {
class RailcarEditForm;
}

class RailcarEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit RailcarEditForm(QWidget *parent = nullptr);
    ~RailcarEditForm();

    void setModel(QAbstractItemModel *model);

    QDataWidgetMapper *getMapper() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::RailcarEditForm *ui;

    QDataWidgetMapper* mapper;
};

#endif // RAILCAREDITFORM_H
