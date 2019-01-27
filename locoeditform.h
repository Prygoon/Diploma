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
    void hideDeleteButton();
    void showDeleteButton();

    void setWIndex(QModelIndex *value);

    QModelIndex *getWIndex() const;

private:
    Ui::LocoEditForm *ui;
    QDataWidgetMapper* mapper;

    QModelIndex *wIndex;

signals:
    void deleteLocoSignal();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_delete_pushButton_clicked();

};

#endif // LOCOEDITFORM_H
