#include "database.h"

DataBase::DataBase(QObject *parent) :
    QObject (parent)
{

}

// Методы для подключения к базе данных
void DataBase::connectToDataBase()
{
    if(!QDir().exists(DATABASE_PATH)) {
        QDir().mkpath(DATABASE_PATH);
    }

    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление */
    if(!QFile(DATABASE_PATH DATABASE_NAME).exists()) {
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

// Методы восстановления базы данных
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    //return false;
}

// Метод для открытия базы данных
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_PATH DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}


// Методы закрытия базы данных
void DataBase::closeDataBase()
{
    db.close();
}

// Метод для создания таблицы в базе данных
bool DataBase::createTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением. */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    TABLE_TYPE                    " TEXT       NOT NULL,"
                    TABLE_MASS                    " INTEGER    NOT NULL,"
                    TABLE_CONSTRUCTION_VELOCITY   " INTEGER    NOT NULL,"
                    TABLE_CALC_VELOCITY           " REAL       NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    //return false;
}
