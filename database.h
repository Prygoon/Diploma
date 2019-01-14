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

#define TABLE                       "Locomotives"
#define TABLE_TYPE                  "Type"
#define TABLE_MASS                  "Mass"
#define TABLE_CONSTRUCTION_VELOCITY "ConstructionVelocity"
#define TABLE_CALC_VELOCITY         "CalcVelocity"

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    //~DataBase();

    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу */
    void connectToDataBase();
    bool inserIntoTable(const QVariantList &data);

private:
    //Oбъект базы данных, с которым будет производиться работа
    QSqlDatabase db;

    //Внутренние методы для работы с базой данных
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();
};

#endif // DATABASE_H
