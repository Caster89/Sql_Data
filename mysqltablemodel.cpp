#include "mysqltablemodel.h"
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlTableModel>
#include <QAbstractItemModel>
#include <QSqlRecord>
#include <QtDebug>
#include <QSqlField>
#include <QDir>
#include "myfield.h"
#include "querygenerator.h"

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::MySqlTableModel
/// \param parent
/// \param newDB
/// Initiates the SQLTable, the parent is used to construct the QAbstractTableModel
/// the newDB is the QSqlDatabase linkd to this table model. It is saved in the
/// Database variable
MySqlTableModel::MySqlTableModel(QWidget *parent, QSqlDatabase newDB)
    :QAbstractTableModel(parent)
{
    Database = newDB;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::select
///This method is called when initiating a Database. It determines the number of
///columns, rows, Primary keys, etc
void MySqlTableModel::select()
{
    QSqlQuery query;
    /*First step is creating a query in the 'Fields_table', selecting all entries,
    and ordering them by the 'Poition' column. num_cols is set to 0
    */
    query.exec("SELECT * FROM Fields_table ORDER BY Position");
    num_cols=0;
    /*The results of query are read one by one in order to create a vector of Fields
     The fields are added one by one to the baseRecord which is used as a mould for
     future records
     */
    while (query.next()){
        num_cols++;
        MyField temp_vec = MyField(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toBool(),query.value(4).toBool(),query.value(5).toInt(),query.value(6).toBool());
        baseRecord.append(QSqlField(query.value(0).toString()));
        /*The field is checked to see if it is a primary one, in which case it is added
         *to the Primary Fields vector
         */
        if(query.value(6).toInt()==1){
            Primary_keys.append(temp_vec);
        }
        this->setHeaderData(num_cols-1,Qt::Horizontal,temp_vec.getName());
        Fields.append(temp_vec);

    }
    /*Once the columns are determined the rows are read. Each record is stored in
     *a temporary QSqlRecord. Next, if the row is not stored in the Main_table, and
     *is thus stored in another table, the other table is queried, the results stored
     *in a QVector<QSqlRecord> and placed i the temporary QSqlRecord. Finally once
     *all fields are completed the record is appended to the main QVector
     */
    num_rows =0;
    query.exec("SELECT * FROM Main_table");
    while (query.next()){
        QSqlRecord curr_Record=QSqlRecord(query.record());

        for(int i=0;i<Fields.size();i++){
            //At each cycle the select and the where statement are reset
            QString selectStatement="";
            QString whereStatement="";

            if(Fields[i].getTable()!="Main_table"){
                selectStatement=QueryGen.SelectStat(Fields[i].getTable());
                whereStatement=QueryGen.WhereStat(GetPrimary(&curr_Record));
                /*for(int n=0;n<Primary_keys.size();n++){
                    QVector<QString> temp_where;
                    temp_where<<Primary_keys[n].getName()<<curr_Record.value(baseRecord.indexOf(Primary_keys[n].getName())).toString();
                    //temp_where<<Primary_keys[i].getName()<<(query.record().value(baseRecord.indexOf(Fields[index.column()].getName()))).toString();
                    Where_Cond.append(temp_where);
                }*/
                whereStatement=selectStatement + " " + whereStatement;
                QSqlQuery where_query;
                where_query.exec(whereStatement);
                //where_query.exec(Where_String.createSelect(Fields[i].getTable(),Where_Cond));
                QVector<QSqlRecord> Images_list;

                while(where_query.next()){
                    //QSqlRecord curr_Image=where_query.record();
                    //curr_Image.append(QSqlField("Status",QVariant::String));
                    //curr_Image.setValue("Status","O");
                    Images_list.append(where_query.record());
                }
                curr_Record.setValue(Fields[i].getName(),QVariant::fromValue(Images_list));
            }

        }
        //Values.append(query.record());
        Values.append(curr_Record);

        num_rows++;
    }
}

int MySqlTableModel::rowCount(const QModelIndex &parent) const
{
    return num_rows;
}

int MySqlTableModel::columnCount(const QModelIndex &parent) const
{
    return num_cols;
}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
             return QVariant();

    if (index.row() >= num_rows || index.row() < 0)
             return QVariant();

    if (role == Qt::DisplayRole||role == Qt::EditRole){

        if(Fields[index.column()].getTable()=="Main_table"){
            //QSqlRecord sel_record= record(index.row())
            //QSqlQuery *newVals = new QSqlQuery(Values);
            //newVals->seek(index.row());
            //Values.seek(index.row());
            QSqlRecord selRec=Values[index.row()];

            //return Values[index.row()].value(baseRecord.indexOf(Fields[index.column()]));
            return selRec.value(baseRecord.indexOf(Fields[index.column()].getName()));
        }else{
            QVector< QVector<QString> > Where_Cond;
            //QSqlQuery *newVals = new QSqlQuery(Values);
            //newVals->seek(index.row());
            //Values.seek(index.row());
            QSqlRecord selRec=Values[index.row()];
            for(int i=0;i<Primary_keys.size();i++){
                QVector<QString> temp_where;
                temp_where<<Primary_keys[i].getName()<<selRec.value(baseRecord.indexOf(Fields[index.column()].getName())).toString();
                Where_Cond.append(temp_where);
            }
            QueryGenerator Where_String = QueryGenerator();
            QSqlQuery query;
            query.exec(Where_String.createSelect(Fields[index.column()].getTable(),Where_Cond));
            QVector<QSqlRecord> ret_Vector;
            while(query.next()){
                QSqlRecord temp_record=QSqlRecord(query.record());
                temp_record.append(QSqlField("Status",QVariant::String));
                temp_record.setValue("Status","O");
                ret_Vector<<temp_record;
            }
            return QVariant::fromValue(ret_Vector);
        }
//        QVector<QVariant> record = Values[index.row()];
//             return record[index.column()];
    }

    return QVariant();

}

bool MySqlTableModel::insertRow(int row, const QModelIndex &parent){

    qDebug()<<"Current number of rows:"<<num_rows;
    if(row>Values.size()){
        return false;
    }else if(row<0){
        qDebug()<<"Adding to bottom";
        beginInsertRows(parent,Values.size(),Values.size());
        Values.append(QSqlRecord(baseRecord));
        endInsertRows();
        clearEditBuffer();
        bufferPosition=num_rows;
        bufferType="INSERT";
        num_rows++;
        qDebug()<<"Actual number of new rows: "<<Values.size()<<" Number memorized: "<<num_rows;
        return true;
    }else{
        qDebug()<<"Insert row";
        beginInsertRows(parent,row,row);
        Values.insert(row,QSqlRecord(baseRecord));
        endInsertRows();
        clearEditBuffer();
        bufferPosition=row;
        bufferType="INSERT";
        num_rows++;
        qDebug()<<"Actual number of new rows: "<<Values.size()<<" Number memorized: "<<num_rows;
        return true;
    }

}

void MySqlTableModel::clearEditBuffer(){
    bufferRecord.clear();
    bufferPosition=-1;
    bufferType="";
}

void MySqlTableModel::revert(){
    if (bufferType=="INSERT"){

        Values.remove(bufferPosition);
        num_rows--;
    }
    clearEditBuffer();
}

bool MySqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(bufferType=="INSERT" && index.row()==bufferPosition){
        qDebug()<<"Setting Data: "<<index.column();
        qDebug()<<"to: "<<value.toString();
        bufferRecord.append(baseRecord.field(index.column()));
        bufferRecord.setValue(index.column(),value);
        return true;
    }else if(bufferPosition!=index.row()){
        clearEditBuffer();
        bufferPosition=index.row();
    }
    if(value!=Values[bufferPosition].value(index.column())){
        bufferRecord.append(baseRecord.field(index.column()));
        bufferRecord.setValue(index.column(),value);
    }
}

QVector<MyField> MySqlTableModel::getFields(){
    return Fields;
}

MyField MySqlTableModel::getField(int section){
    return Fields[section];
}

QSqlRecord MySqlTableModel::record(int Rec_index){
    return Values[Rec_index];
}

QVariant MySqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal){
        return Fields[section].getName();
    }
    return QVariant();
}

bool MySqlTableModel::canUpdate(QSqlRecord *avlbRecord){
    QSqlRecord CkcRecord= GetPrimary(avlbRecord);
    QString selQuery=QueryGen.SelectStat("Main_table");
    QString whereQuery=QueryGen.WhereStat(CkcRecord);
    QString compQuery= selQuery + " " + whereQuery;
    QSqlQuery ckcAvlb(compQuery);
    ckcAvlb.exec();
    while(ckcAvlb.next()){
        return false;
    }
    return true;

}

bool MySqlTableModel::submit(){

}

bool MySqlTableModel::submitAll(){
    /*First step is to check if the record can be inserted, thus if the record is
     *new or if the primary fields have changedthe primary fields are compared,
     *if they are already present in the database the method returns false and ends
     **/
    QSqlQuery query;
    QSqlRecord bufferPrimary=GetPrimary(&bufferRecord);
    qDebug()<<bufferType;
    qDebug()<<bufferPrimary;
    if(bufferType=="INSERT"||bufferPrimary!=GetPrimary(&Values[bufferPosition])){
        if(!canUpdate(&bufferRecord)){
            qDebug()<<"Cannot Update";
            return false;
        }
    }
    if (bufferType=="INSERT"){
        CreateDirectory(bufferPrimary);
        QString insert_stat=QueryGen.InsertStat("Main_table",bufferPrimary);
        qDebug()<<insert_stat;
        //query.exec(insert_stat);
    }
    for (int i=0;i<Fields.size();i++){
        QString fieldName=Fields[i].getName();
        if(Fields[i].getTable()!="Main_table" && bufferRecord.contains(fieldName)){

            insertImages(Fields[i].getTable(),bufferPrimary, bufferRecord.value(fieldName).value<QVector<QSqlRecord> >());
            bufferRecord.remove(bufferRecord.indexOf(fieldName));
        }else if(Fields[i].getType()=="IMAGE"){
            if(bufferRecord.value(fieldName).toString()!=Values[bufferPosition].value(fieldName).toString()){
                if (bufferRecord.value(fieldName).toString().isEmpty()){
                    QString imgDir=GetDirectory()
                }


            }
        }
    }
    /*QString update_stat=QueryGen.UpdateStat("Main_table",bufferRecord);
    QString where_stat=QueryGen.WhereStat(bufferPrimary);
    query.exec(update_stat + " " + where_stat);
    return true;
    */
}

QSqlRecord MySqlTableModel::GetPrimary(QSqlRecord *curr_Record){
    QSqlRecord ret_Record = QSqlRecord();
    for(int i=0;i<Primary_keys.size();i++){
        QString FieldName=Primary_keys[i].getName();
        ret_Record.append(curr_Record->field(FieldName));
        ret_Record.setValue(FieldName,curr_Record->value(FieldName));
    }
    return ret_Record;
}

bool MySqlTableModel::CreateDirectory(QSqlRecord newRec){
   QString newDir=Database.databaseName();
   QStringList newDirList = newDir.split(QDir::separator());
   newDirList.removeLast();
   newDir=newDirList.join(QDir::separator());
   newDir.append(QDir::separator());
   for(int i =0;i<newRec.count();i++){
       newDir.append(newRec.value(i).toString());
       newDir.append('_');
   }
   newDir.chop(1);

   QDir currDir=QDir::currentPath();
   currDir.mkdir(newDir);
   qDebug()<<"Base Directory: "<<newDir;
   for (int i=0;i<Fields.size();i++){
       if(Fields[i].getTable()!="Main_table"){
           QString subDir=newDir;
           subDir.append(QDir::separator());
           subDir.append(Fields[i].getName());
           qDebug()<<"Sub Directory: "<<subDir;
           currDir.mkdir(subDir);
       }
   }
}

QString MySqlTableModel::GetDirectory(QSqlRecord newRec){
    QString newDir=Database.databaseName();
    QStringList newDirList=newDir.split(QDir::separator());
    newDirList.removeLast();
    newDir=newDirList.join(QDir::separator());
    newDir.append(QDir::separator());
    for(int i=0;i<newRec.count();i++){
        newDir.append(newRec.value(i).toString());
        newDir.append('_');
    }
    newDir.chop(1);
    //QDir currDir=QDir::currentPath();
    //newDir=currDir.absolutePath() + QDir::separator() + newDir + QDir::separator();
    return newDir;

}

bool MySqlTableModel::insertImages(QString TableName, QSqlRecord Primary_Values, QVector<QSqlRecord> Images){
    QString workDir=Database.databaseName();
    QStringList workDirList=workDir.split(QDir::separator());
    qDebug()<<"List: "<<workDirList;
    workDirList.removeLast();
    workDir=workDirList.join(QDir::separator());
    workDir.append(QDir::separator());
    QSqlQuery query=QSqlQuery();
    for(int i =0;i<Primary_Values.count();i++){
        workDir.append(Primary_Values.value(i).toString());
        workDir.append('_');
    }
    workDir.chop(1);
    workDir.append(QDir::separator());
    workDir.append(TableName);
    qDebug()<<"Image directory: "<<workDir;

    QSqlRecord currImage;
    for (int i=0;i<Images.size();i++){
        currImage=Images[i];
        if (currImage.value("Status").toString()=="N"){
            for(int n=0;n<Primary_Values.count();n++){
                currImage.append(Primary_Values.field(n));
                currImage.setValue(Primary_Values.fieldName(n),Primary_Values.value(n));
            }
            qDebug()<<currImage.value("File").toString();
            QFileInfo orgImage = QFileInfo(currImage.value("File").toString());
            QFile::copy(orgImage.absoluteFilePath(),QString(workDir+QDir::separator()+orgImage.fileName()));
            currImage.setValue("File",orgImage.fileName());
            qDebug()<<currImage.value("File").toString();
            QString addQuery=QueryGen.InsertStat(TableName,currImage);
            qDebug()<<addQuery;
            //query.exec(addQuery);
        } else if (currImage.value("Status").toString()=="R"){
            QString delStat = QueryGen.DeleteStat(TableName);
            QString whereStat= QueryGen.WhereStat(currImage);
            delStat.append(" ");
            delStat.append(whereStat);
            query.exec(delStat);
            QString Filepath=GetDirectory(Primary_Values);
            Filepath.append(currImage.value("File").toString());
            QDir remDir = QDir(Filepath);
            remDir.remove(Filepath);

        }
    }
}


Q_DECLARE_METATYPE(QVector<QSqlRecord>);
