#ifndef LOCOEDITFORM_H
#define LOCOEDITFORM_H

#define BLANK_TEXT ""

#include <QDialog>
#include <QDataWidgetMapper>
#include <QCloseEvent>
#include <QValidator>


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
    void disableSaveButton();
    void enableSaveButton();
    void createBlankForm();

    void setWIndex(QModelIndex *value);
    QModelIndex *getWIndex() const;

private:
    Ui::LocoEditForm *ui;

    QDataWidgetMapper* mapper;
    QValidator* validator;
    QModelIndex* wIndex;

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    bool isAllLineEditsEmpty();

signals:
    void deleteLocoSignal();
    void submitTableModel();
    void revertTableModel();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_delete_pushButton_clicked();
    void onTextEdited(const QString &arg1);
};

#endif // LOCOEDITFORM_H
