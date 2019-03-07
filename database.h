#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QFile>
#include <QDebug>

//Директивы имен таблицы, полей таблицы и базы данных
#define DATABASE_HOSTNAME   "DiplomaDataBase"
#define DATABASE_NAME       "Diploma.db"
#define DATABASE_PATH       "./data/"

//Таблица локомотивов
#define TABLE_LOCO_NAME                  "locomotives"
#define TABLE_LOCO_TYPE                  "type"
#define TABLE_LOCO_CALC_THRUST_FORCE     "calc_thrust_force"
#define TABLE_LOCO_MASS                  "mass"
#define TABLE_LOCO_CONSTRUCTION_VELOCITY "construction_velocity"
#define TABLE_LOCO_CALC_VELOCITY         "calc_velocity"

//Таблица вагонов
#define TABLE_RAILCAR_NAME       "railcars"
#define TABLE_RAILCAR_TYPE       "type"
#define TABLE_RAILCAR_AXLE_COUNT "axle_count"
#define TABLE_RAILCAR_NAMEPLATE  "nameplate"
#define TABLE_RAILCAR_K_COEF     "k_coefficient"
#define TABLE_RAILCAR_A_COEF     "a_coefficient"
#define TABLE_RAILCAR_B_COEF     "b_coefficient"
#define TABLE_RAILCAR_C_COEF     "c_coefficient"

//Таблица карты вагонов
#define TABLE_RAILCAR_MAP_NAME    "railcars_map"
#define TABLE_RAILCAR_MAP_MASS    "mass"
#define TABLE_RAILCAR_MAP_PERCENT "percent"

//Таблица участков пути
#define TABLE_TRACK_SECTION_NAME         "track_sections"
#define TABLE_TRACK_SECTION_INDEX        "serial number"
#define TABLE_TRACK_SECTION_LENGTH       "length"
#define TABLE_TRACK_SECTION_SLOPE        "slope"
#define TABLE_TRACK_SECTION_CURVE_LENGTH "curve_length"
#define TABLE_TRACK_SECTION_CURVE_RADIUS "curve_radius"

//Таблица проектов
#define TABLE_PROJECT_NAME   "projects"
#define TABLE_PROJECT_TITLE  "title"

class DataBase
{

public:
    static DataBase& Instance()
        {
            // согласно стандарту, этот код ленивый и потокобезопасный
            static DataBase db;
            return db;
        }

    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу */
    void connectToDataBase();
    bool inserIntoTable(const QVariantList &data);
    bool updateTable(const QVariantList &data);

private:
    explicit DataBase(); // конструктор недоступен
    ~DataBase(); // и деструктор тоже

    DataBase(DataBase const&) = delete;
    DataBase& operator= (DataBase const&) = delete;

    //Oбъект базы данных, с которым будет производиться работа
    QSqlDatabase db;

    //Внутренние методы для работы с базой данных
    bool openDataBase();
    void closeDataBase();
    bool restoreDataBase();

    bool createLocoTable();
    bool createRailcarTable();    
    bool createRailcarMapTable();
    bool createTrackSectionsTable();
    bool createProjectTable();

    bool createAllTables();
};

#endif // DATABASE_H
