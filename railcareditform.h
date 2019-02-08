#ifndef RAILCAREDITFORM_H
#define RAILCAREDITFORM_H

#define BLANK_TEXT ""

#include <QDialog>
#include <QDataWidgetMapper>
#include <QCloseEvent>
#include <QValidator>

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
    void disableSaveButton();
    void enableSaveButton();
    void createBlankForm();

    QModelIndex *getWIndex() const;
    void setWIndex(QModelIndex *value); 

private:
    Ui::RailcarEditForm *ui;

    QDataWidgetMapper* mapper;
    QValidator *validator;
    QModelIndex *wIndex;

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    bool isAllLineEditsEmpty();
signals:
    void deleteRailcarSignal();
    void submitTableModel();
    void revertTableModel();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_delete_pushButton_clicked();
    void onTextEdited(const QString &arg1);
};

#endif // RAILCAREDITFORM_H
