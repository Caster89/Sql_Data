#pragma once
#ifndef QUERYGENERATOR_H
#define QUERYGENERATOR_H
#include <QString>
#include <QVector>
#include <QSqlRecord>
#include <QStringList>
#include "myfield.h"

namespace QueryGenerator
{

    enum Query_Type{SELECT,DELETE,INSERT,UPDATE,CREATETABLE,ALTERTABLE};
    QString SelectWhereStat(QString Table_name, QSqlRecord where_rec, QString comparison = "LIKE");
    QString SelectStat(QString Table_Name, QVector<QString> Columns=QVector<QString>());
    QString InsertStat(QString Table_name, QSqlRecord insert_rec);
    QString UpdateStat(QString Table_name, QSqlRecord update_rec);
    QString DeleteWhereStat(QString Table_name,QSqlRecord delete_rec, QString comparison = "=");
    QString UpdateFromTable(QString origTable, QString destTable, QVector<MyField> copyRec, QVector<MyField> refRec);

    QString WhereStat(QSqlRecord where_rec, QString comparison = "LIKE");
    QString Where2TableStat(QString origTable, QString destTable, QVector<MyField> fields);
    QString InsertStat(QString Table_name, QSqlRecord rec_Insert);
    QString UpdateWhereStat(QString Table_name, QSqlRecord rec_Update, QSqlRecord rec_Where, QString comparison = "=");
    QString createTable(QString Table_name,QVector<QVector<QString> > Fields_values, QVector<QString> Primary, QVector<QString> Unique=QVector<QString>(0));
    QString createTable(QString Table_name, QVector<MyField> fields);
    QString createExtTable(QString Table_name, QVector<MyField> fields);
    QStringList insertField(QString tableName, QVector<MyField> fields, MyField newField);
    QString insertField(MyField newField);
    QString editField(FieldEdit fieldChanges);
    QStringList removeField(QString tableName, QVector<MyField> fields, MyField field);
    QString copyTable(QString origTableName, QString destTableName, QVector<MyField> origFields, QVector<MyField> destFields);
    QString copyExtTable(QString origTableName, QString destTableName, QVector<MyField> origFields, QVector<MyField> destFields);
    QStringList multipleChanges(QString tableName, QList<FieldEdit *>);
}

#endif // QUERYGENERATOR_H
