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
    void hideDeleteButton();
    void showDeleteButton();
    void createBlankForm();

    QModelIndex *getWIndex() const;
    void setWIndex(QModelIndex *value);

private:
    Ui::RailcarEditForm *ui;

    QDataWidgetMapper* mapper;
    QModelIndex *wIndex;

signals:
    void deleteRailcarSignal();
    void submitTableModel();
    void revertTableModel();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_delete_pushButton_clicked();
};

#endif // RAILCAREDITFORM_H
