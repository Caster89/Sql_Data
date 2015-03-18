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
    :QSqlTableModel(parent,newDB)
{
    baseDirectory=newDB.databaseName();
    baseDirectory.cdUp();
    qDebug()<<"Text: "<<DataType::Text<<"\n Long Text: "<<DataType::LongText<<"\n Image: "<<DataType::Image<<"\n Images: "<<DataType::Images<<"\n Other: "<<MySqlTableModel::Other;
}

void MySqlTableModel::setTable(const QString &tableName){
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
    /*The results of query are read one by one in order to create a vector of Fields
     The fields are added one by one to the baseRecord which is used as a mould for
     future records
     */
    while (query.next()){
        num_cols++;
        MyField temp_vec = MyField(query.value(0).toString(),query.value(1).toString(),DataType::dataType(query.value(2).toInt()),query.value(3).toBool(),query.value(4).toBool(),query.value(5).toInt(),query.value(6).toBool());
        Fields.append(temp_vec);
        //baseRecord.append(QSqlField(query.value(0).toString()));
        /*The field is checked to see if it is a primary one, in which case it is added
         *to the Primary Fields vector
         */
        //if(query.value(6).toInt()==1){
            //Primary_keys.append(temp_vec);
        //}
        //this->setHeaderData(num_cols-1,Qt::Horizontal,temp_vec.getName());


    }
    /*Once the columns are determined the rows are read. Each record is stored in
     *a temporary QSqlRecord. Next, if the row is not stored in the Main_table, and
     *is thus stored in another table, the other table is queried, the results stored
     *in a QVector<QSqlRecord> and placed i the temporary QSqlRecord. Finally once
     *all fields are completed the record is appended to the main QVector
     */
    /*
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
                for(int n=0;n<Primary_keys.size();n++){
                    QVector<QString> temp_where;
                    temp_where<<Primary_keys[n].getName()<<curr_Record.value(baseRecord.indexOf(Primary_keys[n].getName())).toString();
                    //temp_where<<Primary_keys[i].getName()<<(query.record().value(baseRecord.indexOf(Fields[index.column()].getName()))).toString();
                    Where_Cond.append(temp_where);
                }
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
    }*/
    return isOK;
}



QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{


    QVariant originalValue=QSqlTableModel::data(index,role);

    if(Fields[index.column()].getTable()!="Main_table"){

        QSqlRecord primValues;
        primValues=getPrimary(index);
        QString stmt=QueryGenerator::SelectStat(Fields[index.column()].getTable());
        QString where=QueryGenerator::WhereStat(primValues);
        stmt.append(QLatin1String(" "));
        stmt.append(where);
        QSqlQuery foreignTableQuery;
        foreignTableQuery.exec(stmt);
        QVector<QSqlRecord> returnVector;
        while(foreignTableQuery.next()){
            returnVector.append(foreignTableQuery.record());
        }
        return QVariant::fromValue(returnVector);
    }
    if (!index.isValid()||(role!=Qt::DisplayRole && role!= Qt::EditRole))
             return QVariant();

    return QSqlTableModel::data(index,role);
    /*if (index.row() >= num_rows || index.row() < 0)
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

    return QVariant();*/

}

//*****************************************************************//
//insertRow is called when a new row has to be inserted. Once the  //
//row is created a method will be called ot populate it            //
//*****************************************************************//
bool MySqlTableModel::insertRow(int row, const QModelIndex &parent){
    if( QSqlTableModel::insertRow(row,parent)){
        if(editStrategy()==OnRowChange)
            insertIndex=row;
        return true;
    }
    return false;

    if(row>Values.size()){
        return false;
    }else if(row<0){
        beginInsertRows(parent,Values.size(),Values.size());
        Values.append(QSqlRecord(baseRecord));
        endInsertRows();
        clearEditBuffer();
        bufferPosition=num_rows;
        bufferType="INSERT";
        num_rows++;
        return true;
    }else{
        beginInsertRows(parent,row,row);
        Values.insert(row,QSqlRecord(baseRecord));
        endInsertRows();
        clearEditBuffer();
        bufferPosition=row;
        bufferType="INSERT";
        num_rows++;
        return true;
    }

}

//*****************************************************************//
//insertRowIntoTable is used to populate the values of a new row   //
//in the table. The record is taken and the values for external   //
//tables are saved in a separate record, along with the name of   //
//the table into which they have to be inserted. The original     //
//method is then used to insert the cleaned records, and the      //
//external values are inserted separatelly. The directory for the //
//record is created, and the files copied in the appropriate      //
//directory. The records are inserted using an overloaded function//
//insertRowIntoTable which takes as input the record to insert the//
//primary values and the name of the table to insert              //
//*****************************************************************//
bool MySqlTableModel::insertRowIntoTable(const QSqlRecord &values){
    QSqlRecord rec=values;
    if(!canUpdate(&rec)){
        revert();
        return false;
    }
    QSqlRecord sepRec=QSqlRecord();
    for(int i=0;i<Fields.size();i++){
        if(Fields[i].getTable()!="Main_table" && rec.value(rec.indexOf(Fields[i].getName())).isValid()){
            sepRec.append(rec.field(Fields[i].getName()));
            sepRec.setValue(Fields[i].getName(),rec.value(Fields[i].getName()));
            rec.setNull(Fields[i].getName());
        }
    }
    if(!QSqlTableModel::insertRowIntoTable(rec))
        return false;

    //Should probably insert a test ot check if the record can be created.
    //Possibly a return type for createDirectory of type bool
    createDirectory(rec);

    for(int i=0;i<Fields.size();++i){
        if(Fields[i].getTable()!="Main_table" && sepRec.value(Fields[i].getName()).isValid()){
            QVector<QSqlRecord> tempRecords=qvariant_cast<QVector<QSqlRecord> >(sepRec.value(Fields[i].getName()));
            for(int n=0;n<tempRecords.size();++n)
                insertRowIntoTable(tempRecords[n],getPrimary(&rec),Fields[i].getTable());
        }
    }

    for(int i=0;i<Fields.size();++i){
        if(Fields[i].getTable()!="Main_table" && rec.value(rec.indexOf(Fields[i].getName())).isValid()){
            QVector<QSqlRecord> tempRecords=qvariant_cast<QVector<QSqlRecord> >(rec.value(rec.indexOf(Fields[i].getName())));
            rec.setNull(Fields[i].getName());
            for(int n=0;n<tempRecords.size();++n)
                insertRowIntoTable(tempRecords[n],getPrimary(&rec),Fields[i].getTable());
            rec.setNull(rec.indexOf(Fields[i].getName()));
        }
    }
    QSqlTableModel::insertRowIntoTable(rec);
}


//*****************************************************************//
//insertRowIntoTable overloaded function is used to insert records //
//in alterante tables                                              //
//*****************************************************************//
bool MySqlTableModel::insertRowIntoTable(const QSqlRecord &values,QSqlRecord primaryValues, QString table){
    QSqlRecord rec=values;
    for(int i=0;i<primaryValues.count();++i){
        rec.insert(i,primaryValues.field(i));
        rec.setValue(i,primaryValues.value(i));
    }
    rec.setValue("Status","O");
    QString fileDir=copyFile(values,primaryValues,table);
    QFileInfo file=QFileInfo(fileDir);
    rec.append(QSqlField("Directory",QVariant::String));
    rec.setValue("Directory",file.absolutePath().append(QDir::separator()));
    rec.setValue("File",file.fileName());
    QString stmt=QueryGenerator::InsertStat(table,rec);
    QSqlQuery query;
    query.exec(stmt);
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
    //The new values are copied in a new record in order to manipulate it without
    //changing the original values
    QSqlRecord rec=values;
    //A separate record is created to store the data which has to be inserted in
    //different tables
    QSqlRecord sepRec=QSqlRecord();
    //primChanges stores whether the primary values of the field have changed
    bool primChanged=false;
    //Two new records are created to store the primary fields of both the old and the
    //new record. The new one should be empty if the new records have not been copied
    QSqlRecord origRecord=primaryValues(row);
    QSqlRecord newRec=getPrimary(&rec);
    //The fields which have to be stored in a different table are stored in sepRec and
    //removed from the original value
    for(int i=0;i<Fields.size();i++){
        if(Fields[i].getTable()!="Main_table" && rec.value(rec.indexOf(Fields[i].getName())).isValid()&& !rec.value(rec.indexOf(Fields[i].getName())).isNull()){
            sepRec.append(rec.field(Fields[i].getName()));
            sepRec.setValue(Fields[i].getName(),rec.value(Fields[i].getName()));
            rec.setNull(Fields[i].getName());
        }

    }
        if(newRec!=origRecord){
            primChanged=true;
        }
    //A last check is done to make sure that the values have not been changed to
    //the same as the old
    if(newRec==origRecord)
        primChanged=false;
    //The record is updated using the original method
    if(!QSqlTableModel::updateRowInTable(row,rec))
        return false;
    qDebug()<<sepRec;
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
}

bool MySqlTableModel::updateRowInTable(const QSqlRecord &values,QSqlRecord primaryValues,QString table){
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
    if(QSqlTableModel::deleteRowFromTable(row)){
        QSqlRecord recToDel = primaryValues(row);
        for(int i=0;i<Fields.count();++i){
            if(Fields[i].getTable()!="Main_table"){
                QString stmt=QueryGenerator::DeleteStat(Fields[i].getTable(),recToDel);
                QSqlQuery dltQuery;
                if(!dltQuery.exec(stmt))
                    return false;
            }
        }
        deleteRecordDirectory(recToDel);
        return true;
    }
    return false;
}

bool MySqlTableModel::deleteFromTable(const QSqlRecord &values, QString tableName){
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

QString MySqlTableModel::copyFile(const QSqlRecord &values,QSqlRecord primaryValues, QString table){



    QString path=baseDirectory.absolutePath();
    path.append(QDir::separator());
    for(int i=0;i<primaryValues.count();++i){
        path.append(primaryValues.value(i).toString());
        path.append('_');
    }
    path.chop(1);
    path.append(QDir::separator());
    path.append(table);
    path.append(QDir::separator());
    QFileInfo file(values.value("File").toString());
    path.append(file.fileName());
    QFile origFile(file.absoluteFilePath());
    if(origFile.copy(path))
        return path;
    return "";
}

void MySqlTableModel::clearEditBuffer(){
    bufferRecord.clear();
    bufferPosition=-1;
    bufferType="";
}

void MySqlTableModel::revert(){
    QSqlTableModel::revert();
    if (bufferType=="INSERT"){

        Values.remove(bufferPosition);
        num_rows--;
    }
    clearEditBuffer();
}

//*****************************************************************//
//setData is called by the mapper when commit() is called, which in//
//turn is called by the submit() command.                          //
//QWidgetMapper->submit()->commit()                                //
//*****************************************************************//
bool MySqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role){

    if(Fields[index.column()].getTable()=="Main_table"){
    }else{
        QVector<QSqlRecord> data = qvariant_cast<QVector<QSqlRecord> >(value);
    }
    return QSqlTableModel::setData(index,value,role);
    if(QSqlTableModel::setData(index,value,role)){
        switch (editStrategy()) {
        case OnRowChange:
            editIndex=index.row();
            break;
        }
        return true;
    }
    return false;

    if(bufferType=="INSERT" && index.row()==bufferPosition){
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

/*QSqlRecord MySqlTableModel::record(int Rec_index){
    return Values[Rec_index];
}*/

QVariant MySqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal){
        return Fields[section].getName();
    }
    return QVariant();
}


bool MySqlTableModel::submit(){
    return QSqlTableModel::submit();
}

bool MySqlTableModel::insertRows(int row, int count, const QModelIndex &parent){
    if (row < 0 || count <= 0 || row > rowCount() || parent.isValid())
            return false;
    switch(editStrategy()){
    case OnRowChange:
        if(count!=1)
            break;
        insertIndex=row;
        break;
    }
    return QSqlTableModel::insertRows(row,count,parent);
}

void MySqlTableModel::revertRow(int row){
    if (row<0)
        return;
    if (editStrategy()==OnRowChange){
        if(editIndex==row){
            editIndex=-1;
        }else if(insertIndex==row){
            insertIndex=-1;
        }
    }
    QSqlTableModel::revertRow(row);
}

bool MySqlTableModel::removeRows(int row,int count, const QModelIndex &parent){
    if(QSqlTableModel::removeRows(row,count)){

        for(int i=0;i<count;++i){
            if(row+1==insertIndex)
                insertIndex=-1;
        }
        select();
        return true;
    }
    return false;
    /*bool mkIt=QSqlTableModel::removeRows(row,count);
    return mkIt;
    if (parent.isValid() || row < 0 || count <= 0)
           return false;

       int i;
       switch (editStrategy()) {
       case OnFieldChange:
       case OnRowChange:
           for (i = 0; i < count; ++i) {
               if (row + i == insertIndex)
                   qDebug()<<"Shoufl revert row";
               else if (!QSqlTableModel::deleteRowFromTable(row + i))
                   return false;
           }
           select();
           break;
       case OnManualSubmit:
           for (i = 0; i < count; ++i) {
               int idx = row + i;
               if (idx >= rowCount())
                   return false;

           }
           break;
       }
       return true;
    /*qDebug()<<"got in here";
    if (parent.isValid() || row < 0 || count <= 0)
        qDebug()<<"Would return false";


    qDebug()<<QSqlTableModel::removeRows(3,1);
    return true;*/


}

//*****************************************************************//
//submitAll uses the original submit all, which in turn calls the  //
//different methods to update insert or remove, based on the       //
//required operation                                               //
//*****************************************************************//
bool MySqlTableModel::submitAll(){


    if(QSqlTableModel::submitAll()){
        switch(editStrategy()){
        case OnRowChange:
            editIndex=-1;
            insertIndex=-1;
        }
        return true;
    }
    return false;

    /*First step is to check if the record can be inserted, thus if the record is
     *new or if the primary fields have changedthe primary fields are compared,
     *if they are already present in the database the method returns false and ends
     **/
    QSqlQuery query;
    QSqlRecord bufferPrimary=getPrimary(&bufferRecord);

    if(bufferType=="INSERT"||bufferPrimary!=getPrimary(&Values[bufferPosition])){
        if(!canUpdate(&bufferRecord)){
            return false;
        }
    }
    if (bufferType=="INSERT"){
        createDirectory(bufferPrimary);
        QString insert_stat=QueryGen.InsertStat("Main_table",bufferPrimary);

        //query.exec(insert_stat);
    }
    for (int i=0;i<Fields.size();i++){
        QString fieldName=Fields[i].getName();
        if(Fields[i].getTable()!="Main_table" && bufferRecord.contains(fieldName)){

            insertImages(Fields[i].getTable(),bufferPrimary, bufferRecord.value(fieldName).value<QVector<QSqlRecord> >());
            bufferRecord.remove(bufferRecord.indexOf(fieldName));
        }else if(Fields[i].getType()==DataType::Image){
            if(bufferRecord.value(fieldName).toString()!=Values[bufferPosition].value(fieldName).toString()){
                if (bufferRecord.value(fieldName).toString().isEmpty()){
                    //QString imgDir=GetDirectory();
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
    /*QSqlRecord ret_Record = QSqlRecord();
    for(int i=0;i<Primary_keys.size();i++){
        QString FieldName=Primary_keys[i].getName();
        ret_Record.append(curr_Record->field(FieldName));
        ret_Record.setValue(FieldName,curr_Record->value(FieldName));
    }
    return ret_Record;*/
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
       if(Fields[i].getTable()!="Main_table"){
           addDir=createDir;
           addDir.append(QDir::separator());
           addDir.append(Fields[i].getTable());
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
    QDir baseDir(database().databaseName());
    baseDir.cdUp();
    QString createDir="";

    QSqlRecord primValues=getPrimary(&newRec);
    for(int i=0;i<primValues.count();++i){
        createDir.append(primValues.value(i).toString());
        createDir.append('_');
    }
    createDir.chop(1);
    baseDir.cd(createDir);
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

QString MySqlTableModel::GetDirectory(QSqlRecord newRec){
    QString newDir=baseDirectory.absolutePath();

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
            //query.exec(addQuery);
        } else if (currImage.value("Status").toString()=="R"){
            //QString delStat = QueryGen.DeleteStat(TableName);
            /*QString whereStat= QueryGen.WhereStat(currImage);
            delStat.append(" ");
            delStat.append(whereStat);
            query.exec(delStat);*/
            QString Filepath=GetDirectory(Primary_Values);
            Filepath.append(currImage.value("File").toString());
            QDir remDir = QDir(Filepath);
            remDir.remove(Filepath);

        }
    }
}


Q_DECLARE_METATYPE(QVector<QSqlRecord>);

