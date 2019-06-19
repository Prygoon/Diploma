#ifndef LOCOEDITFORM_H
#define LOCOEDITFORM_H

#define BLANK_TEXT ""

#include <QDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QCloseEvent>
#include <QValidator>
#include <QtXlsx>
#include <QFileDialog>


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

    void setEditOrNewCheck(bool value);
    void setWIndex(QModelIndex *value);
    QModelIndex *getWIndex() const;
    QString getStrTractionJson() const;

private:
    Ui::LocoEditForm *ui;

    QSqlTableModel *locoModel;
    QDataWidgetMapper* mapper;
    QValidator* validator;
    QModelIndex* wIndex;

    QString dataDir;
    QString strTractionJson;

    bool editOrNewCheck;

    void closeEvent(QCloseEvent *event); // Переопределённый виртуальный метод обработки крестика закрытия окна
    bool isAllLineEditsEmpty();

signals:
    void deleteLocoSignal();
    void submitTableModel(const QString &strTractionJson);
    void revertTableModel();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_delete_pushButton_clicked();
    void onTextEdited(const QString &arg1);
    void on_traction_pushButton_clicked();
};

#endif // LOCOEDITFORM_H
