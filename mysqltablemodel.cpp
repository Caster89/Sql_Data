#include "mysqltablemodel.h"
#include <QtWidgets>
#include <QFileInfoList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlTableModel>
#include <QSqlIndex>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QSqlRecord>
#include <QtDebug>
#include <QSqlField>
#include <QSqlDriver>
#include <QSqlError>
#include <QDir>
#include <QMap>
#include "myfield.h"
#include "querygenerator.h"
#include "datatype.h"

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::MySqlTableModel
/// \param parent
/// \param newDB
/// Initiates the SQLTable, the parent is used to construct the QAbstractTableModel
/// the newDB is the QSqlDatabase linkd to this table model. It is saved in the
/// Database variable
MySqlTableModel::MySqlTableModel(QWidget *parent, QSqlDatabase newDB)
    :QSqlTableModel(parent,newDB)
{
    baseDirectory=newDB.databaseName();
    baseDirectory.cdUp();
    qDebug()<<"Text: "<<DataType::Text<<"\n Long Text: "<<DataType::LongText<<"\n Image: "<<DataType::Image<<"\n Images: "<<DataType::Images<<"\n Other: "<<DataType::Other;
}

void MySqlTableModel::setTable(const QString &tableName){
    qDebug()<<"///////////Calling set Table///////////";
    editIndex=-1;
    insertIndex=-1;
    QSqlTableModel::setTable(tableName);
    primaryRecord=database().primaryIndex(tableName);
    baseRecord=database().record(tableName);
}


/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::select
///This method is called when initiating a Database. It determines the number of
///columns, rows, Primary keys, etc
bool MySqlTableModel::select()
{
    qDebug()<<"///////////Calling select///////////";
    /*The first step is to set the table to Main_table, which is the table containing
     * the data in sql database
     */
    QStringList avlbTables = database().tables();
    if(avlbTables.contains("Main_table")){
        setTable("Main_table");
    }
    bool isOK = QSqlTableModel::select();


    QSqlQuery query;
    /*Next a query is created in the 'Fields_table', selecting all entries,
    and ordering them by the 'Position' column. num_cols is set to 0
    */
    query.exec("SELECT * FROM Fields_table ORDER BY Position");
    num_cols=0;
    /*The results of query are read one by one and stored in the vector of Fields
     The fields are added one by one to the baseRecord which is used as a mould for
     future records
     */
    while (query.next()){
        num_cols++;
        MyField temp_vec = MyField(query.value("Name").toString(),
                                   query.value("Table").toString(),
                                   DataType::dataType(query.value("Type").toInt()),
                                   query.value("Vis_Preview").toBool(),
                                   query.value("Vis_Table").toBool(),
                                   query.value("Position").toInt(),
                                   query.value("Primary").toBool(),
                                   query.value("Comments").toString());
        Fields.append(temp_vec);

    }

    return isOK;
}


//*****************************************************************//
//data is used to retrieve the data from the database. For most    //
//fields this is done using the standard method, for fields consis-//
//ting of files are retrieved using the standard method, but are   //
//then packaged in a sqlrecord                                     //
//*****************************************************************//
QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()||(role!=Qt::DisplayRole && role!= Qt::EditRole))
             return QVariant();
    //QVariant originalValue=QSqlTableModel::data(index,role);
    //If the field is not the main table it has to be pulled from
    //an external table
    if(Fields[index.column()].getTable()!="Main_table"){

        //The primary values for the element are obtained and stored in a
        //QSqlRecord using the getPrimary function
        QSqlRecord primValues;
        primValues=getPrimary(index);
        //A search string is created to find the record in the correct table
        QString stmt=QueryGenerator::SelectStat(Fields[index.column()].getTable());
        QString where=QueryGenerator::WhereStat(primValues);
        stmt.append(QLatin1String(" "));
        stmt.append(where);
        //the query is executed
        QSqlQuery foreignTableQuery;
        foreignTableQuery.exec(stmt);
        //and the results stored in a vector of QSqlRecords
        QVector<QSqlRecord> returnVector;
        while(foreignTableQuery.next()){
            returnVector.append(foreignTableQuery.record());
        }

        return QVariant::fromValue(returnVector);
    }
    //If the field is in the main table but is an image if is treated
    //separately, returning a QSqlRecord rather than a string.
    if(Fields[index.column()].getType()==DataType::Image){
        QSqlRecord imageRecord;
        QString fileName=QSqlTableModel::data(index,role).toString();
        imageRecord.append(QSqlField("File",QVariant::String));
        //imageRecord.append(QSqlField("Directory",QVariant::String));
        imageRecord.append(QSqlField("Status",QVariant::String));
        imageRecord.setValue("Status","O");
        imageRecord.setValue("File",fileName);
        return QVariant::fromValue(imageRecord);

    }


    return QSqlTableModel::data(index,role);


}


//*****************************************************************//
//insertRowIntoTable is used to populate the values of a new row   //
//in the table. The record is taken and the values for external   //
//tables are saved in a separate record, along with the name of   //
//the table into which they have to be inserted. The original     //
//method is then used to insert the cleaned records, and the      //
//external values are inserted separately. The directory for the  //
//record is created, and the files copied in the appropriate      //
//directory. The records are inserted using an overloaded function//
//insertRowIntoTable which takes as input the record to insert,the//
//primary values and the name of the table to insert.             //
//The external values should be passed to the function as a       //
//QVector<QSqlRecord>, while the single files should be passed as //
// a QSqlRecord with two field, a filename and a directory        //
//*****************************************************************//
bool MySqlTableModel::insertRowIntoTable(const QSqlRecord &values){
    qDebug()<<"///////////Calling insertRowIntoTable///////////";
    //beginInsertRows(QModelIndex(), rowCount()-1, rowCount()-1);
    QSqlRecord rec=values;

    if(!canUpdate(&rec)){
        revert();
        return false;
    }

    //A new Record is used to store the parts of the record not saved in the Main Table. Each field
    //will be a separate record
    //In the mean time the value is set to null in original record.
    //Also the fields which refer to files are modified, the record is saved in a separate record and the field is
    //modified to be a string, which is set to the name of the field plus the file extension
    QSqlRecord sepRec=QSqlRecord();
    QMap <QString, QString> fileTransfer;
    qDebug()<<"File transfer has "<<fileTransfer.count()<<" elements";
    for(int i=0;i<Fields.size();i++){
        //Storing and setting Null external fields
        if(Fields[i].getTable()!="Main_table" && rec.value(rec.indexOf(Fields[i].getName())).isValid()){
            sepRec.append(rec.field(Fields[i].getName()));
            sepRec.setValue(Fields[i].getName(),rec.value(Fields[i].getName()));
            rec.setNull(Fields[i].getName());
            //rec.remove(i);
        }
        //Modifying the file fields
        if(Fields[i].getType()==DataType::Image && rec.value(rec.indexOf(Fields[i].getName())).isValid()){

            QSqlRecord currRec=qvariant_cast<QSqlRecord>(rec.value(Fields[i].getName()));
            if(!currRec.value("File").isNull()){
                QString newFile = currRec.value("File").toString();
                QFileInfo *newFileInfo = new QFileInfo(newFile);
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                QString destFile = GetDirectory(rec,Fields[i] ) + QDir::separator() + newFileInfo->fileName();

                rec.setValue(Fields[i].getName(),destFile);
                fileTransfer[newFile] = destFile;
            }
        }

    }

    if(!QSqlTableModel::insertRowIntoTable(rec)){
        qDebug()<<"Failed to insert row";
        qDebug()<<this->lastError();
        return false;
     }

    createDirectory(rec);

    QMap<QString, QString>::const_iterator i = fileTransfer.constBegin();
    while (i != fileTransfer.constEnd()) {
        QFile origFile(i.key());
        origFile.copy(i.value());
        ++i;
    }

    fileTransfer.clear();
    //Once the record is inserted in the database the values stored in separate tables
    //are added using the overloaded function
    for(int i=0;i<Fields.size();++i){
            if(Fields[i].getTable()!="Main_table" && sepRec.value(Fields[i].getName()).isValid()){
                QVector<QSqlRecord> tempRecords=qvariant_cast<QVector<QSqlRecord> >(sepRec.value(Fields[i].getName()));
                for(int n=0;n<tempRecords.size();++n)
                    insertRowIntoTable(tempRecords[n],getPrimary(&rec),Fields[i].getTable());
            }
        }

    return true;

}

//*****************************************************************//
//insertRowIntoTable overloaded function is used to insert records //
//in alterante tables                                              //
//*****************************************************************//
bool MySqlTableModel::insertRowIntoTable(const QSqlRecord &values,QSqlRecord primaryValues, QString table){
    qDebug()<<"///////////Calling insertRowIntoTable (overloaded)///////////";
    qDebug()<<values;
    QSqlRecord rec=values;
    //The primary values are added to the current record
    for(int i=0;i<primaryValues.count();++i){
        rec.insert(i,primaryValues.field(i));
        rec.setValue(i,primaryValues.value(i));
    }
    //The status of the record is set to old
    rec.setValue("Status","O");

    QString fileDir=copyFile(rec,primaryValues,table);
    rec.setValue("File",fileDir);
    QString stmt=QueryGenerator::InsertStat(table,rec);
    QSqlQuery query;
    qDebug()<<"The Sql Query is: "<<stmt;
    qDebug()<<"Query result: "<<query.exec(stmt);
    return true;
}

//*****************************************************************//
//updateRowInTable is used to change values of existing records. As//
//with insertRowIntoTable the record is first divided in values    //
//which should be inserted in the main table and values which have //
//to be inserted in alternate tables. A check is done to make sure //
//that the primary values have not changed, in which case additional//
//changes have to be made to the directory,as well as checks to make//
//sure the primary values are valid. Once the basic values are     //
//updated the values for the alternate tables are scanned 1-by-1   //
//based on the values of Status: O-M-R the records are either      //
//ignored, updated or removed                                      //
//*****************************************************************//
bool MySqlTableModel::updateRowInTable(int row, const QSqlRecord &values){
    qDebug()<<"///////////Calling update row///////////";
    //The new values are copied in a new record in order to manipulate it without
    //changing the original values
    QSqlRecord rec=values;
    //primChanges stores whether the primary values of the field have changed
    bool primChanged=false;
    //Two new records are created to store the primary fields of both the old and the
    //new record. The new one should be empty if the new records have not been copied
    QSqlRecord origRecord=primaryValues(row);
    QSqlRecord newRec=getPrimary(&rec);
    if(newRec!=origRecord){
        primChanged=true;
        if(!canUpdate(&rec)){
            qDebug()<<"The record cannot be updated with these values as they already exist";
            return false;
        }
    }
    QSqlRecord sepRec=QSqlRecord();
    emit beforeUpdate(row, rec);

    //The fields which have to be stored in a different table are stored in sepRec and
    //removed from the original value
    //The fields which store a file are also store in a separate record fileRec
    QMap <QString, QString> fileTransfer;
    QFileInfoList fileDelete;
    for(int i=0;i<Fields.size();i++){
        if(Fields[i].getTable()!="Main_table" && rec.value(rec.indexOf(Fields[i].getName())).isValid() && !rec.value(rec.indexOf(Fields[i].getName())).isNull()){
            sepRec.append(rec.field(Fields[i].getName()));
            sepRec.setValue(Fields[i].getName(),rec.value(Fields[i].getName()));
            rec.setNull(Fields[i].getName());
        }else if(Fields[i].getType()==DataType::Image &&
                rec.value(rec.indexOf(Fields[i].getName())).isValid() &&
                !rec.value(rec.indexOf(Fields[i].getName())).isNull()){
            QSqlRecord currRec=qvariant_cast<QSqlRecord>(rec.value(Fields[i].getName()));

            if(currRec.value("Status").toString()=="N"){
                    QString newFile = currRec.value("File").toString();
                    QFileInfo *newFileInfo = new QFileInfo(newFile);
                    rec.replace(
                            rec.indexOf(Fields[i].getName()),
                            QSqlField(Fields[i].getName(),QVariant::String)
                            );
                    QString destFile = GetDirectory(rec,Fields[i] ) + QDir::separator() + newFileInfo->fileName();
                    rec.setValue(Fields[i].getName(),destFile);
                    fileTransfer[newFile] = destFile;
            }else if(currRec.value("Status").toString()=="M"){
                QString newFile = currRec.value("File").toString();
                QFileInfo *newFileInfo = new QFileInfo(newFile);
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                QDir destDir = QDir::current();
                destDir.cd(GetDirectory(rec,Fields[i]));
                QString destFile = GetDirectory(rec,Fields[i] ) + QDir::separator() + newFileInfo->fileName();


                rec.setValue(Fields[i].getName(),destFile);
                fileTransfer[newFile] = destFile;
                fileDelete = destDir.entryInfoList();

            }else if(currRec.value("Status").toString()=="R"){
                QDir destDir = QDir::current();
                destDir.cd(GetDirectory(rec,Fields[i]));
                fileDelete = destDir.entryInfoList();
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                rec.setNull(Fields[i].getName());
            }
        }

    }

    //The record is updated using the original method
    if(!QSqlTableModel::updateRowInTable(row,rec))
        return false;
    qDebug()<<sepRec;

    {
    QFileInfoList::const_iterator i = fileDelete.constBegin();
    while (i != fileDelete.constEnd()) {
        QFile delFile(i->absoluteFilePath());
        delFile.remove();
        ++i;
    }
    }

    {
    QMap<QString, QString>::const_iterator i = fileTransfer.constBegin();
    while (i != fileTransfer.constEnd()) {
        QFile origFile(i.key());
        origFile.copy(i.value());
        ++i;
    }
    }



    for(int i=0;i<sepRec.count();i++){
        QString tableName=sepRec.fieldName(i);
        QVector<QSqlRecord>  fileList=qvariant_cast<QVector<QSqlRecord> >(sepRec.value(i));
        for(int n=0;n<fileList.size();n++){
            QSqlRecord currRecord=fileList[n];
            //QSqlRecord currRecord=separateDir(fileList[n]);
            //qDebug()<<"Current list: "<<fileList[n];
            //qDebug()<<"Corrected record: "<<currRecord;
            if(currRecord.value("Status").toString()=="N"){
                currRecord=separateDir(currRecord);
                insertRowIntoTable(fileList[n],origRecord,tableName);
            }else if(currRecord.value("Status").toString()=="R"){
                deleteFromTable(currRecord,tableName);
            }else if (currRecord.value("Status").toString()=="M"){
                //qDebug()<<"***********************\n You chose to update the a value, in particulare:\n"<<newRec<<"\n Using these new Values:\n"<<currRecord<<"\n in the following table: "<<tableName;
                updateRowInTable(currRecord,newRec,tableName);
            }
        }
    }
    //Once the value is changed in the main table if primary values have been changed 
    //the folder has to be renames as well as all the values in the related tables
    if (primChanged){
        qDebug()<<"Primary Changed";
        primaryChange(origRecord,newRec);
    }
    return true;
}
//*****************************************************************//
//updateRowInTable: overloaded function used to update a row in an //
//external table. It writes the necessary SQL command and executes //
//it
bool MySqlTableModel::updateRowInTable(const QSqlRecord &values,QSqlRecord primaryValues,QString table){
    qDebug()<<"///////////Calling updateRowInTable (overloaded)///////////";
    QSqlRecord rec=values;
    QSqlRecord whereValues=QSqlRecord();
    whereValues.append(values.field("File"));
    whereValues.append(values.field("Directory"));
    whereValues.setValue("File",values.value("File"));
    whereValues.setValue("Directory",values.value("Directory"));
    for(int i=0;i<primaryValues.count();++i){
        rec.insert(i,primaryValues.field(i));
        rec.setValue(i,primaryValues.value(i));
    }
    rec.setValue("Status","O");

    QString stmt=QueryGenerator::UpdateStat(table,rec);
    QString whereStmt=QueryGenerator::WhereStat(whereValues);
    stmt.append(" ");
    stmt.append(whereStmt);
    QSqlQuery query;
    query.exec(stmt);
    return true;
}
//*****************************************************************//
//primaryChange makes sure that if a primary value has changed all //
//external tables are updated as well as the directories
bool MySqlTableModel::primaryChange(QSqlRecord oldPrim, QSqlRecord newPrim){
    //The old directory and new directory are created
    QString oldDir=GetDirectory(oldPrim);
    QString newDir=GetDirectory(newPrim);

    //The directory is renamed from the old to the new
    QDir renameDir=QDir(oldDir);
    qDebug()<<"The old directory is: "<<oldDir<<"\n The new directory is: "<<newDir;
    qDebug()<<"Renaming was successful: "<<renameDir.rename(oldDir,newDir);

    //The new directory has a separator added to the end
    //The new record has a Diretory field added to it
    newDir.append(QDir::separator());
    newPrim.append(QSqlField("Directory"));
    for(int i=0;i<Fields.size();++i){
        //For every Filed not contained in the main table the records are updated
        if(Fields[i].getTable()!="Main_table"){
            //The current table is recorded and added to the end of the directory
            QString tableDir=newDir;
            tableDir.append(Fields[i].getTable());
            tableDir.append(QDir::separator());
            //The value for the current directory is set in the new record
            newPrim.setValue("Directory",tableDir);
            //The update statement is created with the new primary values and the new
            //directory
            QString upStmt= QueryGenerator::UpdateStat(Fields[i].getTable(),newPrim);
            upStmt.append(' ');
            upStmt.append(QueryGenerator::WhereStat(oldPrim));

            QSqlQuery selQuery;
            selQuery.exec(upStmt);

            
        }
    }
    return true;
}

bool MySqlTableModel::deleteRowFromTable(int row){
    qDebug()<<"///////////Calling deleteRowFromTable///////////";
    QSqlRecord recToDel = primaryValues(row);
    if(QSqlTableModel::deleteRowFromTable(row)){
        qDebug()<<"The original method worked";
        for(int i=0;i<Fields.count();++i){
            if(Fields[i].getTable()!="Main_table"){
                QString stmt=QueryGenerator::DeleteStat(Fields[i].getTable(),recToDel);
                QSqlQuery dltQuery;
                if(!dltQuery.exec(stmt)){
                    qDebug()<<"Returning false";
                    return false;
                }
            }
        }
        deleteRecordDirectory(recToDel);
        qDebug()<<"Returning true";
        return true;
    }
    qDebug()<<"Returning false";
    qDebug()<<"The Error: "<<this->lastError();
    return false;
}

bool MySqlTableModel::deleteFromTable(const QSqlRecord &values, QString tableName){
    qDebug()<<"///////////Calling deleteRowFromTable (overloaded)///////////";
    QString fileDir=values.value("Directory").toString();
    fileDir.append(values.value("File").toString());
    QFile fileToDel(fileDir);
    qDebug()<<"deleting file"<<fileDir;

    QSqlRecord primValues=database().primaryIndex(tableName);
    for (int i=0;i<primValues.count();i++){
        primValues.setValue(i,values.value(primValues.fieldName(i)));
    }
    QString dltStmt =QueryGenerator::DeleteStat(tableName,primValues);
    qDebug()<<"Delete string is: "<<dltStmt;
    QSqlQuery delQuery;
    if (delQuery.exec(dltStmt)){
        if(fileToDel.remove())
            return true;
    }
    return false;
}

QString MySqlTableModel::copyFile(const QSqlRecord &record,QSqlRecord primaryValues, QString table){
    QString path= QDir::currentPath();
            //baseDirectory.absolutePath();
    //QString path
    path.append(QDir::separator());

    for(int i=0;i<primaryValues.count();++i){
        path.append(primaryValues.value(i).toString());
        path.append('_');
    }
    path.chop(1);
    path.append(QDir::separator());
    path.append(table);
    path.append(QDir::separator());
    //at this point the path is /PrimField1_PrimField2_..._PrimFieldN/Table/
    //QString origFile = values.value("Directory").toString()+QDir::separator();
    QFileInfo origFileInfo(record.value("File").toString());
    path.append(origFileInfo.fileName());
    QFile origFile(record.value("File").toString());
    if(origFile.copy(path)){
        //QString returnPath= "";
        //for(int i=0;i<primaryValues.count();++i){
            //returnPath.append(primaryValues.value(i).toString());
            //returnPath.append('_');
        //}
        //returnPath += QDir::separator() + origFileInfo.fileName();

        return QDir::current().relativeFilePath(path);
    }
    return "";
}


bool MySqlTableModel::removeFile(QSqlRecord record, QSqlField field){
    QString path=GetDirectory(record);
    QDir dir(path);
    dir.setNameFilters(QStringList() << field.name().append(".*"));
    dir.setFilter(QDir::Files);
    bool success=true;
    foreach(QString dirFile, dir.entryList())
    {
        if(!dir.remove(dirFile))
            success=false;
    }
    return success;
}


QVector<MyField> MySqlTableModel::getFields(){
    return Fields;
}

MyField MySqlTableModel::getField(int section){
    return Fields[section];
}

QSqlRecord MySqlTableModel::record(int Rec_index) const{
    QSqlRecord returnRecord(baseRecord);
    qDebug()<<"Trying to fill the following record: "<<baseRecord;
    for(int i=0;i<returnRecord.count();i++){
        int col=fieldIndex(returnRecord.fieldName(i));
        if (Fields[col].getTable()!="Main_table"){
            returnRecord.replace(col,QSqlField(Fields[col].getName()));
        }

        returnRecord.setValue(returnRecord.fieldName(i),data(index(Rec_index,col),Qt::DisplayRole));
    }
    return returnRecord;
}

QVariant MySqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal){
        return Fields[section].getName();
    }
    return QVariant();
}

bool MySqlTableModel::canUpdate(QSqlRecord *avlbRecord){
    QSqlRecord primValues = getPrimary(avlbRecord);
    QString stmt= QueryGenerator::SelectStat("Main_table");
    QString wStmt=QueryGenerator::WhereStat(primValues);
    stmt.append(' ');
    stmt.append(wStmt);
    qDebug()<<"Can update"<<stmt;
    QSqlQuery qCanUpdate =QSqlQuery();
    qCanUpdate.exec(stmt);

    while(qCanUpdate.next()){
        qDebug()<<qCanUpdate.record();
        return false;
    }
    return true;
}

QSqlRecord MySqlTableModel::getPrimary(const QModelIndex &index) const{
    QSqlRecord record=primaryRecord;
    for(int i=0;i<record.count();++i){
        QModelIndex nIndex = QSqlTableModel::index(index.row(),baseRecord.indexOf(record.fieldName(i)));
        record.setValue(i,data(nIndex,Qt::EditRole));
    }
    return record;
}

QSqlRecord MySqlTableModel::getPrimary(int row) const{
    QModelIndex indx = index(row,0);
    return getPrimary(indx);
}

QSqlRecord MySqlTableModel::getPrimary(QSqlRecord *curr_Record) const{
    QSqlRecord record=primaryRecord;
    for(int i=0;i<record.count();++i)
        record.setValue(i,curr_Record->value(curr_Record->indexOf(record.fieldName(i))));
    return record;
}

bool MySqlTableModel::createDirectory(QSqlRecord newRec){
   QDir baseDir(database().databaseName());
   baseDir.cdUp();
   QString createDir="";

   QSqlRecord primValues=getPrimary(&newRec);
   for(int i=0;i<primValues.count();++i){
       createDir.append(primValues.value(i).toString());
       createDir.append('_');
   }
   createDir.chop(1);
   if(!baseDir.mkdir(createDir))
       return false;
   QString addDir;
   for(int i=0;i<Fields.size();++i){
       if(Fields[i].getTable()!="Main_table" || Fields[i].getType()==DataType::Image){
           addDir=createDir;
           addDir.append(QDir::separator());
           addDir.append(Fields[i].getName());
           if(!baseDir.mkdir(addDir)){
               deleteRecordDirectory(newRec);
               return false;
           }
       }
   }
   return true;
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
   for (int i=0;i<Fields.size();i++){
       if(Fields[i].getTable()!="Main_table"){
           QString subDir=newDir;
           subDir.append(QDir::separator());
           subDir.append(Fields[i].getName());
           currDir.mkdir(subDir);
       }
   }
}

bool MySqlTableModel::deleteRecordDirectory(QSqlRecord newRec){
    QDir baseDir = QDir::current();

    QString createDir="";

    QSqlRecord primValues=getPrimary(&newRec);
    for(int i=0;i<primValues.count();++i){
        createDir.append(primValues.value(i).toString());
        createDir.append('_');
    }
    createDir.chop(1);
    baseDir.cd(createDir);
    qDebug()<<baseDir;
    if(baseDir.removeRecursively())
        return true;
    return false;
}

QSqlRecord MySqlTableModel::separateDir(QSqlRecord altRecord){
    QFileInfo compDirectory = QFileInfo(altRecord.value("File").toString());
    altRecord.append(QSqlField("Directory",QVariant::String));
    altRecord.setValue("Directory",compDirectory.absolutePath().append(QDir::separator()));
    altRecord.setValue("File",compDirectory.fileName());
    return altRecord;

}

QString MySqlTableModel::GetDirectory(QSqlRecord newRec, bool relative) const{
    QString newDir;
    if(relative){
        newDir= "";
    }else{
        newDir = QDir::currentPath();
        newDir.append(QDir::separator());
    }


    newRec = getPrimary(&newRec);
    for(int i=0;i<newRec.count();i++){
        newDir.append(newRec.value(i).toString());
        newDir.append('_');
    }
    newDir.chop(1);

    return newDir;

}

QString MySqlTableModel::GetDirectory(QSqlRecord Rec,MyField field, bool relative){
    QString baseDir= GetDirectory(Rec, relative);
    baseDir.append(QDir::separator());
    baseDir.append(field.getName());
    return baseDir;
}

bool MySqlTableModel::insertImages(QString TableName, QSqlRecord Primary_Values, QVector<QSqlRecord> Images){
    QString workDir=Database.databaseName();
    QStringList workDirList=workDir.split(QDir::separator());
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

    QSqlRecord currImage;
    for (int i=0;i<Images.size();i++){
        currImage=Images[i];
        if (currImage.value("Status").toString()=="N"){
            for(int n=0;n<Primary_Values.count();n++){
                currImage.append(Primary_Values.field(n));
                currImage.setValue(Primary_Values.fieldName(n),Primary_Values.value(n));
            }
            QFileInfo orgImage = QFileInfo(currImage.value("File").toString());
            QFile::copy(orgImage.absoluteFilePath(),QString(workDir+QDir::separator()+orgImage.fileName()));
            currImage.setValue("File",orgImage.fileName());
            QString addQuery=QueryGen.InsertStat(TableName,currImage);
        } else if (currImage.value("Status").toString()=="R"){            
            QString Filepath=GetDirectory(Primary_Values);
            Filepath.append(currImage.value("File").toString());
            QDir remDir = QDir(Filepath);
            remDir.remove(Filepath);

        }
    }
    return true;
}




