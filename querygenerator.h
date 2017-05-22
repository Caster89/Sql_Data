#pragma once
#ifndef QUERYGENERATOR_H
#define QUERYGENERATOR_H
#include <QString>
#include <QVector>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QStringList>
#include "myfield.h"

namespace QueryGenerator
{

    enum Query_Type{SELECT,DELETE,INSERT,UPDATE,CREATETABLE,ALTERTABLE};
    QString SelectWhereStat(const QString Table_name,const QSqlRecord *where_rec,const QString comparison = "LIKE");
    QString SelectWhereStat(const QString Table_name, const QSqlRecord *where_rec, const QSqlIndex *selFields = new QSqlIndex(), const QString comparison = "LIKE") ;
    QString SelectStat(const QString Table_Name,const QSqlRecord *columns);
    QString SelectStat(const QString Table_Name,const QVector<MyField> Columns=QVector<MyField>());
    QString InsertStat(const QString Table_name, const QSqlRecord *insert_rec);
    QString UpdateStat(const QString Table_name,const QSqlRecord *update_rec);
    QString DeleteWhereStat(const QString Table_name,const QSqlRecord *delete_rec,const QString comparison = "=");
    QString UpdateFromTable(const QString origTable,const QString destTable,const QVector<MyField> copyRec,const QVector<MyField> refRec);

    QString WhereStat(const QSqlRecord *where_rec,const QString comparison = "LIKE");
    QString Where2TableStat(const QString origTable,const QString destTable,const QVector<MyField> fields);
    QString InsertStat(const QString Table_name,const QSqlRecord rec_Insert);
    QString UpdateWhereStat(const QString Table_name,const QSqlRecord *rec_Update,const QSqlRecord *rec_Where,const QString comparison = "=");
    QString createTable(const QString Table_name,const QVector<QVector<QString> > Fields_values,const QVector<QString> Primary,const QVector<QString> Unique=QVector<QString>());
    QString createTable(const QString Table_name, const QVector<MyField> fields);

    QString createExtTable(const QString Table_name, const QVector<MyField> fields);
    QStringList insertField(const QString tableName,const QVector<MyField> fields,const MyField newField);
    QString insertField(const MyField newField);
    QString editField(const FieldEdit fieldChanges);
    QStringList removeField(const QString tableName,const QVector<MyField> fields,const MyField field);
    QString copyTable(const QString origTableName,const QString destTableName,const QVector<MyField> origFields,const QVector<MyField> destFields);
    QString copyExtTable(const QString origTableName,const QString destTableName,const QVector<MyField> origFields,const QVector<MyField> destFields);
    QStringList multipleChanges(const QString tableName,const QList<FieldEdit *>);
    QStringList editTable(const QString tableName,const QList<FieldEdit*> changes);
}

#endif // QUERYGENERATOR_H
