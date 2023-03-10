#include "mainwindow.h"
#include <QApplication>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLockFile lockFile(QDir::temp().absoluteFilePath("diploma.lock"));

    /* Пытаемся закрыть Lock File, если попытка безуспешна в течение 100 миллисекунд,
       значит уже существует Lock File созданный другим процессом.
       Следовательно, выбрасываем предупреждение и закрываем программу */
    if(!lockFile.tryLock(100)){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Приложение уже запущено.\n"
                       "Разрешено запускать только один экземпляр приложения.");
        msgBox.setWindowTitle("Ошибка");
        msgBox.exec();
        return 1;
    }

    /* Перевод */
    QTranslator *qt_translator = new QTranslator;
    if(qt_translator->load( ":translations/translations/qtbase_ru.qm" )) {
        a.installTranslator( qt_translator );
    }

    MainWindow w;
    w.show();

    return a.exec();
}
