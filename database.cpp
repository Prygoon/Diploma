#include "database.h"

DataBase::DataBase() {
    dataDirPath = QDir::currentPath().append(DATABASE_PATH);
    dataBasePath = dataDirPath + DATABASE_NAME;
}

DataBase::~DataBase() {

}

// Методы для подключения к базе данных
void DataBase::connectToDataBase()
{
    //qDebug() << dataBasePath;

    if(!QDir().exists(dataDirPath)) {
        QDir().mkpath(dataDirPath);
    }

    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление */
    if(!QFile(dataBasePath).exists()) {
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

// Методы восстановления базы данных локомотивов
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!createAllTables()) {
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
        db.setDatabaseName(dataBasePath);
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
                    TABLE_LOCO_CALC_THRUST_FORCE       " REAL       NOT NULL,"
                    TABLE_LOCO_MASS                    " REAL       NOT NULL,"
                    TABLE_LOCO_CONSTRUCTION_VELOCITY   " INTEGER    NOT NULL,"
                    TABLE_LOCO_CALC_VELOCITY           " REAL       NOT NULL,"
                    TABLE_LOCO_LENGTH                  " REAL       NOT NULL,"
                    TABLE_LOCO_TRACTION                " TEXT       NOT NULL"
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
                    TABLE_RAILCAR_NAMEPLATE	  " TEXT       NOT NULL    UNIQUE,"
                    TABLE_RAILCAR_K_COEF      " REAL       NOT NULL,"
                    TABLE_RAILCAR_A_COEF      " REAL       NOT NULL,"
                    TABLE_RAILCAR_B_COEF      " REAL       NOT NULL,"
                    TABLE_RAILCAR_C_COEF      " REAL       NOT NULL "
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
                    "railcar_id                  INTEGER    NOT NULL                      ,"
                    TABLE_RAILCAR_MAP_MASS     " INTEGER    NOT NULL                      ,"
                    TABLE_RAILCAR_MAP_PERCENT  " REAL       NOT NULL                      ,"
                    "FOREIGN KEY (railcar_id) REFERENCES " TABLE_RAILCAR_NAME"(id)"
                    " )"
                    )) {
        qDebug() << "DataBase: error of create " << TABLE_RAILCAR_MAP_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

//Метод для создания таблицы участков пути
bool DataBase::createTrackSectionsTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением. */
    QSqlQuery query;

    if(!query.exec( "CREATE TABLE " TABLE_TRACK_SECTION_NAME " ( "
                    "id	                               INTEGER PRIMARY KEY AUTOINCREMENT,"
                    TABLE_TRACK_SECTION_SLOPE        " REAL    NOT NULL,"
                    TABLE_TRACK_SECTION_LENGTH       " INTEGER NOT NULL,"
                    TABLE_TRACK_SECTION_CURVE_LENGTH " INTEGER NOT NULL DEFAULT 0,"
                    TABLE_TRACK_SECTION_CURVE_RADIUS " INTEGER NOT NULL DEFAULT 0"
                    " )"
                    )) {
        qDebug() << "DataBase: error of create " << TABLE_TRACK_SECTION_NAME;
        qDebug() << query.lastError().text();
        return true;
    } else {
        return false;
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
                    TABLE_PROJECT_TITLE        " TEXT       NOT NULL"
                    " )"
                    )) {
        qDebug() << "DataBase: error of create " << TABLE_RAILCAR_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

bool DataBase::createAllTables()
{
    return this->createProjectTable()
            && createRailcarTable()
            && createRailcarMapTable()
            && createLocoTable()
            && createTrackSectionsTable();
}
