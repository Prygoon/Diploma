#include "database.h"

DataBase::DataBase() {

}

DataBase::~DataBase() {

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

// Методы восстановления базы данных локомотивов
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!(this->createLocoTable() && this->createRailcarTable() && this->createRailcarMapTable() && this->createProjectTable())) {
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
}

// Метод для открытия базы данных
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует */
    if(!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName(DATABASE_HOSTNAME);
        db.setDatabaseName(DATABASE_PATH DATABASE_NAME);
    }

    if(db.open()){
        QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON;");
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

// Метод для создания таблицы локомотивов в базе данных
bool DataBase::createLocoTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением. */
    QSqlQuery query;

    if(!query.exec( "CREATE TABLE " TABLE_LOCO_NAME " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    TABLE_LOCO_TYPE                    " TEXT       NOT NULL,"
                    TABLE_LOCO_CALC_THRUST_FORCE       " INTEGER    NOT NULL,"
                    TABLE_LOCO_MASS                    " INTEGER    NOT NULL,"
                    TABLE_LOCO_CONSTRUCTION_VELOCITY   " INTEGER    NOT NULL,"
                    TABLE_LOCO_CALC_VELOCITY           " REAL       NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE_LOCO_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

// Метод для создания таблицы вагонов в базе данных
bool DataBase::createRailcarTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением. */
    QSqlQuery query;

    if(!query.exec( "CREATE TABLE " TABLE_RAILCAR_NAME " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    TABLE_RAILCAR_TYPE        " TEXT       NOT NULL,"
                    TABLE_RAILCAR_AXLE_COUNT  " INTEGER    NOT NULL,"
                    TABLE_RAILCAR_K_COEF      " REAL       NOT NULL,"
                    TABLE_RAILCAR_A_COEF      " REAL       NOT NULL,"
                    TABLE_RAILCAR_B_COEF      " REAL       NOT NULL,"
                    TABLE_RAILCAR_C_COEF      " REAL       NOT NULL"
                    " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE_RAILCAR_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

// Метод для создания таблицы карты(с указанием массы брутто и долей в составе) вагонов в базе данных
bool DataBase::createRailcarMapTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением. */
    QSqlQuery query;

    if(!query.exec( "CREATE TABLE " TABLE_RAILCAR_MAP_NAME " ("
                    "id                          INTEGER    PRIMARY  KEY    AUTOINCREMENT ,"
                    "railcar_id1                 INTEGER    NOT NULL                      ,"
                    "railcar_id2                 INTEGER    NOT NULL                      ,"
                    TABLE_RAILCAR_MAP_MASS     " INTEGER    NOT NULL                      ,"
                    TABLE_RAILCAR_MAP_PERCENT  " REAL       NOT NULL                      ,"
                    "FOREIGN KEY (railcar_id1) REFERENCES " TABLE_RAILCAR_NAME"(id)       ,"
                    "FOREIGN KEY (railcar_id2) REFERENCES " TABLE_RAILCAR_NAME"(id)"
                    " )"
                    )) {
        qDebug() << "DataBase: error of create " << TABLE_RAILCAR_MAP_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

// Метод для создания таблицы проектов в базе данных
bool DataBase::createProjectTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением. */
    QSqlQuery query;

    if(!query.exec( "CREATE TABLE " TABLE_PROJECT_NAME " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "railcar_map_id              INTEGER    NOT NULL UNIQUE,"
                    TABLE_PROJECT_TITLE        " TEXT       NOT NULL       ,"
                    "FOREIGN KEY (railcar_map_id) REFERENCES " TABLE_RAILCAR_MAP_NAME"(id)"
                    " )"
                    )) {
        qDebug() << "DataBase: error of create " << TABLE_RAILCAR_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}
