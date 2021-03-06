#pragma once
#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H
#include <QtWidgets>
#include <QSqlDatabase>
#include <QVector>
#include <QSqlTableModel>
#include <QSqlIndex>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDir>
#include <QList>
#include <querygenerator.h>
#include "myfield.h"
#include "metatypedeclaration.h"

class QSqlTableModelPrivate;

class MySqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    /*enum dataType{
        Text,
        LongText,
        Image,
        Images,
        Other,
    };*/
    const QVector<QString> Disp = { "DisplayRole", "DecorationRole", "EditRole", "ToolTipRole", "StatusTipRole", "WhatsThisRole", "FontRole", "TextAlignmentRole", "BackgroundRole", "ForegroundRole", "CheckStateRole", "AccessibleTextRole", "AccessibleDescriptionRole", "SizeHintRole", "InitialSortOrderRole" };

    MySqlTableModel(QWidget *parent = Q_NULLPTR, QSqlDatabase newDB = QSqlDatabase());
    // Loads the necessary data in the model, such as the fields and their table
    bool select();
    //int rowCount (const QModelIndex &parent) const;
    //int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role= Qt::DisplayRole) const;
    //void revertRow(int row);
    //bool removeRows(int row,int count, const QModelIndex &parent=QModelIndex());
    void setTable(const QString &tableName);
    bool insertRowIntoTable(const QSqlRecord &values);
    bool insertRowIntoTable(const QSqlRecord &values,QSqlRecord primaryValues,QString table);
    bool updateRowInTable(int row, const QSqlRecord &values);
    bool updateRowInTable(const QSqlRecord &values,QSqlRecord primaryValues,QString table);
    bool deleteFromTable(const QSqlRecord &values, QString tableName);
    bool deleteRowFromTable(int row);
    QString copyFile(const QSqlRecord &record, QSqlRecord primaryValues, QString table);
    bool deleteRecordDirectory(QSqlRecord newRec);
    int insertIndex;
    int editIndex;
    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role=Qt::EditRole);
    //QVector Values;
    QVector<QSqlRecord > Values;



    QSqlRecord record(int Rec_index) const;
    QVector<MyField> getFields();
    MyField getField(int section);
    inline int indexOf(QString FieldName){
        return baseRecord.indexOf(FieldName);
    }

    bool editTableStructure(QList<FieldEdit *> changes);
    //bool submitAll();
    //bool submit();

private:
    //Varaible containing the database to which the model is connected
    QSqlDatabase Database;
    QDir baseDirectory;

    //Two int variables with the number of rows and the number of columns
    int num_rows;
    int num_cols;

    //This baseRecord is created each time the columns are changed in order to
    //have a default structure, which can be used to create new records
    QSqlRecord baseRecord;
    QSqlIndex primaryRecord;

    //QVector of MyFields which is created each time the columns have to be re-read
    QVector<MyField> Fields;
    //QVector of My fields which contains just the Primary Fields
    QVector<MyField>  Primary_keys;

    //This variable is created as a "Driver" to create the sqlqueries
    //QueryGenerator QueryGen=QueryGenerator();
    /*
     *The following three variables are used in order to keep a buffer for a future
     *record which has to be added. An int whihc sets the position of the current
     *record whihc is being edited, a string which holds whenter the record is being
     *edited or inserted and a QSqlRecord to keep the information
     **/
    int bufferPosition=-1;
    QString bufferType;
    QSqlRecord bufferRecord;

    //Method to clear the buffer information
    //void clearEditBuffer();
    //void revert();


    bool canUpdate(QSqlRecord *avlbRecord);
    QSqlRecord getPrimary(QSqlRecord* curr_Record) const;
    QSqlRecord getPrimary(const QModelIndex &index) const;
    QSqlRecord getPrimary(int row) const;
    bool createDirectory(QSqlRecord newRec);
    bool createFieldDirectory(MyField newField);
    bool deleteFieldDirectory(MyField oldField);
    bool renameFieldDirectory(MyField oldField, MyField newField);
    QString GetDirectory(QSqlRecord newRec, bool relative = true) const;
    QString GetDirectory(QSqlRecord newRec, MyField field, bool relative = true) const;
    bool removeFile(QSqlRecord record, QSqlField field);
    bool insertImages(QString TableName,QSqlRecord Primary_Values,QVector<QSqlRecord> Images);
    bool primaryChange(QSqlRecord oldPrim, QSqlRecord newPrim);
    QSqlRecord separateDir(QSqlRecord altRecord);

};

//Q_DECLARE_METATYPE(QSqlRecord);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
#endif // MYSQLTABLEMODEL_H
