#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H
#include <QtWidgets>
#include <QSqlDatabase>
#include <QVector>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <querygenerator.h>
#include "myfield.h"

class MySqlTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MySqlTableModel(QWidget *parent, QSqlDatabase newDB);

    int rowCount (const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role= Qt::DisplayRole) const;

    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role=Qt::EditRole);
    //QVector Values;
    QVector<QSqlRecord > Values;

    bool insertRow(int row, const QModelIndex &parent);
    bool setData(const QModelIndex &index, const QVariant &value, int role);



    void select();
    QSqlRecord record(int Rec_index);
    QVector<MyField> getFields();
    MyField getField(int section);
    inline int indexOf(QString FieldName){
        return baseRecord.indexOf(FieldName);
    }

    bool submitAll();
    bool submit();

private:
    //Varaible containing the database to which the model is connected
    QSqlDatabase Database;

    //Two int variables with the number of rows and the number of columns
    int num_rows;
    int num_cols;

    //This baseRecord is created each time the columns are changed in order to
    //have a default structure, which can be used to create new records
    QSqlRecord baseRecord;

    //QVector of MyFields which is created each time the columns have to be re-read
    QVector<MyField> Fields;
    //QVector of My fields which contains just the Primary Fields
    QVector<MyField>  Primary_keys;

    //This variable is created as a "Driver" to create the sqlqueries
    QueryGenerator QueryGen=QueryGenerator();
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
    void clearEditBuffer();
    void revert();


    bool canUpdate(QSqlRecord *avlbRecord);
    QSqlRecord GetPrimary(QSqlRecord* curr_Record);
    bool CreateDirectory(QSqlRecord newRec);
    QString GetDirectory(QSqlRecord newRec);
    bool insertImages(QString TableName,QSqlRecord Primary_Values,QVector<QSqlRecord> Images);

};

#endif // MYSQLTABLEMODEL_H
