#pragma once
#ifndef QUERYGENERATOR_H
#define QUERYGENERATOR_H
#include <QString>
#include <QVector>
#include <QSqlRecord>

class QueryGenerator
{
public:
    QueryGenerator();

    enum Query_Type{SELECT,DELETE,INSERT,UPDATE,CREATETABLE,ALTERTABLE};
    static QString createSelect(QString Table_name,QVector<QVector<QString> > Query_Where);
    static QString SelectStat(QString Table_Name, QVector<QString> Columns=QVector<QString>());
    static QString InsertStat(QString Table_name, QSqlRecord insert_rec);
    static QString UpdateStat(QString Table_name, QSqlRecord update_rec);
    static QString DeleteStat(QString Table_name,QSqlRecord delete_rec);
    static QString WhereStat(QSqlRecord where_rec);
    QString createDelete(QString Table_name,QVector<QVector<QString> > Query_Where);
    static QString createInsert(QString Table_name,QVector<QVector<QString> > Query_Insert);
    QString createUpdate(QString Table_name,QVector<QVector<QString> > Query_Upadate, QVector<QVector<QString> > Query_Where);
    QString createTable(QString Table_name,QVector<QVector<QString> > Fields_values, QVector<QString> Primary, QVector<QString> Unique=QVector<QString>(0));


private:

};

#endif // QUERYGENERATOR_H
