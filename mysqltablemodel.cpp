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
/// Constructor for a special QSqlTableModel
/// @param parent The widget to which the model is parented
/// @param newDB The database to which the model is linked
/// Initiates the SQLTable, the parent is used to construct the QAbstractTableModel
/// the newDB is the QSqlDatabase linked to this table model. It is saved in the
/// Database variable
MySqlTableModel::MySqlTableModel(QWidget *parent, QSqlDatabase newDB)
    :QSqlTableModel(parent,newDB)
{
    baseDirectory=newDB.databaseName();
    baseDirectory.cdUp();
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::setTable
/// Sets the main table on which to work
/// @param tableName the name of the table which will be used
/// Sets the main table used throughout
void MySqlTableModel::setTable(const QString &tableName){
    qDebug()<<"##########Calling setTable##########";
    //editIndex=-1;
    //insertIndex=-1;
    QSqlTableModel::setTable(tableName);
    primaryRecord=database().primaryIndex(tableName);
    baseRecord=database().record(tableName);
}


/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::select
/// \return Returns whether the select operation was succesful
///This method is called when initiating a Database. It determines the number of
///columns, rows, Primary keys, etc
bool MySqlTableModel::select()
{
    qDebug()<<"##########Calling select##########";
    /*The first step is to set the table to Main_table, which is the table containing
     * the data in sql database
     */
    QStringList avlbTables = database().tables();
    if(avlbTables.contains("Main_table")){
        setTable("Main_table");
    }
    bool isOK = QSqlTableModel::select();

    if (isOK){
        QSqlQuery query;
        /*Next a query is created in the 'Fields_table', selecting all entries,
        and ordering them by the 'Position' column. num_cols is set to 0
        */
        query.exec("SELECT * FROM Fields_table ORDER BY Position");
        num_cols=0;
        /*The results of query are read one by one and stored in the vector Fields.
         *The fields are added one by one to the baseRecord which is used as a mould for
         *future records
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
                                       query.value("Unique").toBool(),
                                       query.value("Comments").toString());
            Fields.append(temp_vec);

        }

        return isOK;
    }
    return false;
}


//*****************************************************************//
//data is used to retrieve the data from the database. For most    //
//fields this is done using the standard method, for fields consis-//
//ting of files are retrieved using the standard method, but are   //
//then packaged in a sqlrecord                                     //
//*****************************************************************//
/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::data
/// \param index The position of the data to retrieve
/// \param role The role to retrieve
/// \return The data in the given row and column with the  userrole
/// data is used to retrieve the data from the database. For most
/// fields this is done using the standard method, for fields consis-
/// ting of files are retrieved using the standard method, but are
/// then packaged in a sqlrecord
QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{
    QModelIndex nIndex = createIndex(index.row(),fieldIndex(Fields[index.column()].getName()));
            //(index.row(),fieldIndex(Fields[index.column()].getName()));

    /*
     * If the index is not valid or the role is not either EditRole or DisplayRole
     * then en empty value is returned.
     */
    if (!nIndex.isValid()||(role!=Qt::DisplayRole && role!= Qt::EditRole))
             return QVariant();
    /*
     * If the field is not the main table it has to be pulled from
     * an external table
     */
    if(Fields[index.column()].extTable()){
        /*
         * The primary values for the element are obtained and stored in a
         * QSqlRecord using the getPrimary function
         */
        QSqlRecord primValues;
        primValues=getPrimary(index);
        //A search string is created to find the record in the correct table
        QString stmt=QueryGenerator::SelectStat(Fields[index.column()].getTable());
        stmt.chop(1);
        QString where=QueryGenerator::WhereStat(primValues);
        stmt.append(QLatin1String(" "));
        stmt.append(where);
        qDebug()<<"External query: \n"<<stmt;
        //The query is executed
        QSqlQuery foreignTableQuery;
        foreignTableQuery.exec(stmt);
        qDebug()<<"Resulted in "<<foreignTableQuery.size()<<" results";
        //and the results stored in a vector of QSqlRecords
        QVector<QSqlRecord> returnVector;
        /*
         * The value of the image is stored as the file name therefore the
         * path to get there has to be reconstructed from the primary values
         * and the name of the field
         * Primary1_Primary2_...PrimaryN/Field_Name/
         */
        QString baseDir = GetDirectory(getPrimary(index),Fields[index.column()]);
        baseDir.append(QDir::separator());
        QSqlRecord currRecord;
        QString destFile;
        while(foreignTableQuery.next()){
            qDebug()<<"Getting value";
            destFile = baseDir+foreignTableQuery.record().value("File").toString();
            currRecord = foreignTableQuery.record();
            currRecord.setValue("File",destFile);
            returnVector.append(currRecord);
        }

        return QVariant::fromValue(returnVector);
    }
    /*
     * If the field is in the main table but is an image it is treated
     * separately, returning a QSqlRecord rather than a string.
     */
    if(Fields[index.column()].extDir()){
        /*
         * As for Images types the only stored value is the file name
         * so the whole path has to be reconstructed.
         */
        QString baseDir = GetDirectory(getPrimary(index),Fields[index.column()]);
        baseDir.append(QDir::separator());
        QSqlRecord imageRecord;
        QString fileName=QSqlTableModel::data(index,role).toString();
        imageRecord.append(QSqlField("File",QVariant::String));
        imageRecord.append(QSqlField("Status",QVariant::String));
        imageRecord.setValue("Status","O");
        imageRecord.setValue("File",baseDir+fileName);
        qDebug()<<"Lookin for file: "<<imageRecord.value("File").toString();
        return QVariant::fromValue(imageRecord);
    }

    return QSqlTableModel::data(nIndex,role);
}


/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::insertRowIntoTable
/// \param values QSqlRecord witht he values to insert
/// \return Boolean which tells whether the insertion succeded
/// insertRowIntoTable is used to populate the values of a new row   //
/// in the table. The record is taken and the values for external   //
/// tables are saved in a separate record, along with the name of   //
/// the table into which they have to be inserted. The original     //
/// method is then used to insert the cleaned records, and the      //
/// external values are inserted separately. The directory for the  //
/// record is created, and the files copied in the appropriate      //
/// directory. The records are inserted using an overloaded function//
/// insertRowIntoTable which takes as input the record to insert,the//
/// primary values and the name of the table to insert.             //
/// The external values should be passed to the function as a       //
/// QVector<QSqlRecord>, while the single files should be passed as //
/// a QSqlRecord with two field, a filename and a directory        //
bool MySqlTableModel::insertRowIntoTable(const QSqlRecord &values){
    qDebug()<<"##########Calling insertRowIntoTable##########";

    //The insertion value is copied to rec
    QSqlRecord rec=values;
    /*
     * A quick check is done to make sure the value can be inserted
     * in particular checking the primary and unique values
     */
    if(!canUpdate(&rec)){
        revert();
        return false;
    }

    /*
     * A new Record is used to store the parts of the record not saved in the Main Table. Each field
     * will be a separate record
     * In the mean time the value is set to null in original record.
     * Also the fields which refer to files are modified, the record is saved in a separate record and the field is
     * modified to be a string, which is set to the name of the field plus the file extension
     */
    QSqlRecord sepRec=QSqlRecord();
    /*
     * File transfer is a map used to store the information necessary to copy the
     * files to the new location
     */
    QMap <QString, QString> fileTransfer;
    /*
     * Iteration over all the fields to remove the ones destined for
     * an external table or containing a file
     */
    for(int i=0;i<Fields.size();i++){
        /*
         * Storing and setting Null external fields
         * each field in sepRec is a QVector<QSqlRecord> containing all the
         * files for that specific field
         */
        if(Fields[i].extTable() && rec.value(rec.indexOf(Fields[i].getName())).isValid()){
            sepRec.append(rec.field(Fields[i].getName()));
            sepRec.setValue(Fields[i].getName(),rec.value(Fields[i].getName()));
            rec.setNull(Fields[i].getName());
        }

        /*
         * The file fields are also edited, replacing the QSqlRecord with the filename
         * and storing the information necessary to copy the file
         */
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

    //The original method is used to insert the cleaned QSqlRecord
    if(!QSqlTableModel::insertRowIntoTable(rec)){
        qDebug()<<"Failed to insert row";
        qDebug()<<this->lastError();
        return false;
     }
    //If the insertion is successfull then the necessary directories are created
    createDirectory(rec);

    /*
     * Once the directory structure is created the files are copied
     * starting from the single file fields
     */

    QMap<QString, QString>::const_iterator i = fileTransfer.constBegin();
    while (i != fileTransfer.constEnd()) {
        QFile origFile(i.key());
        origFile.copy(i.value());
        ++i;
    }

    /*
     * Next the multiple files fields have the files copied
     * using the overloaded insertRowIntoTable function
     */
    for(int i=0;i<Fields.size();++i){
            if(Fields[i].extTable() && sepRec.value(Fields[i].getName()).isValid()){
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
/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::insertRowIntoTable
/// \param values The file data required (file name, status, description)
/// \param primaryValues The primary values used to identify the ownership
/// \param table The name of the table into which to insert the record
/// \return
/// insertRowIntoTable overloaded function is used to insert records
/// in alterante tables
bool MySqlTableModel::insertRowIntoTable(const QSqlRecord &values,QSqlRecord primaryValues, QString table){
    qDebug()<<"##########Calling insertRowIntoTable (overloaded)##########";
    //The values are copied to rec
    QSqlRecord rec=values;
    //The primary values are added to the current record
    for(int i=0;i<primaryValues.count();++i){
        rec.insert(i,primaryValues.field(i));
        rec.setValue(i,primaryValues.value(i));
    }
    //The status of the record is set to old
    rec.setValue("Status","O");
    QFileInfo fileDir(copyFile(rec,primaryValues,table));
    rec.setValue("File",fileDir.fileName());
    QString stmt=QueryGenerator::InsertStat(table,rec);
    QSqlQuery query;
    return true;
}


/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::updateRowInTable
/// \param row Row number to update
/// \param values The new values with which to update
/// \return
/// updateRowInTable is used to change values of existing records. As
/// with insertRowIntoTable the record is first divided in values
/// which should be inserted in the main table and values which have
/// to be inserted in alternate tables. A check is done to make sure
/// that the primary values have not changed, in which case additional
/// changes have to be made to the directory,as well as checks to make
/// sure the primary values are valid. Once the basic values are
/// updated the values for the alternate tables are scanned 1-by-1
/// based on the values of Status: O-M-R the records are either
/// ignored, updated or removed
bool MySqlTableModel::updateRowInTable(int row, const QSqlRecord &values){
    qDebug()<<"##########Calling updateRowInTable##########";
    //The new values are copied in a new record
    QSqlRecord rec=values;
    //primChanges stores whether the primary values of the field have changed
    bool primChanged=false;
    /*
     * Two new records are created to store the primary fields of both the old and the
     * new record.
     */
    QSqlRecord origRecord=primaryValues(row);
    QSqlRecord newRec=getPrimary(&rec);
    //If the primary fields have changed then a check on the possibility of updating
    if(newRec!=origRecord){
        primChanged=true;
        if(!canUpdate(&rec)){
            qDebug()<<"The record cannot be updated with these values as they already exist";
            return false;
        }
    }

    //A similar method is used to update the value as for inserting
    QSqlRecord sepRec=QSqlRecord();
    //emit beforeUpdate(row, rec);

    /*
     * The fields which have to be stored in a different table are stored in sepRec and
     * removed from the original value
     * The fields which store a file are also stored in a separate record fileRec
     */
    QMap <QString, QString> fileTransfer;
    QFileInfoList fileDelete;
    for(int i=0;i<Fields.size();i++){
        if(Fields[i].extTable() &&
                rec.value(rec.indexOf(Fields[i].getName())).isValid() &&
                !rec.value(rec.indexOf(Fields[i].getName())).isNull()){

            sepRec.append(rec.field(Fields[i].getName()));
            sepRec.setValue(Fields[i].getName(),rec.value(Fields[i].getName()));
            rec.setNull(Fields[i].getName());
        }else if(Fields[i].getType()==DataType::Image &&
                rec.value(rec.indexOf(Fields[i].getName())).isValid() &&
                !rec.value(rec.indexOf(Fields[i].getName())).isNull()){

            QSqlRecord currRec=qvariant_cast<QSqlRecord>(rec.value(Fields[i].getName()));

            if(currRec.value("Status").toString()=="N"){
                //If the file is a new file store the new information
                QString newFile = currRec.value("File").toString();
                QFileInfo *newFileInfo = new QFileInfo(newFile);
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                QString destFile = GetDirectory(rec,Fields[i] ) + QDir::separator() + newFileInfo->fileName();
                destFile.replace(" ","_");
                rec.setValue(Fields[i].getName(),destFile);
                fileTransfer[newFile] = destFile;
            }else if(currRec.value("Status").toString()=="M"){
                /*
                 * if the file is a modification of the old
                 * store the information to add and the one to remove
                 */
                QString newFile = currRec.value("File").toString();
                QFileInfo *newFileInfo = new QFileInfo(newFile);
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                QDir destDir = QDir::current();
                destDir.cd(GetDirectory(rec,Fields[i]));
                QString destFile = GetDirectory(rec,Fields[i] ) + QDir::separator() + newFileInfo->fileName();
                destFile.replace(" ","_");
                rec.setValue(Fields[i].getName(),destFile);
                fileTransfer[newFile] = destFile;
                fileDelete = destDir.entryInfoList();

            }else if(currRec.value("Status").toString()=="R"){
                //If the file is to be remove simply store the path to it in fileDelete
                QDir destDir = QDir::current();
                destDir.cd(GetDirectory(rec,Fields[i]));
                fileDelete = destDir.entryInfoList();
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                rec.setNull(Fields[i].getName());
            }else{
                /*
                 * If nothing is to be done the filed still needs to be
                 * converted from QSqlRecord to QString and the complete
                 * path + filename chopped to only the filname
                 */
                QString fileName = currRec.value("File").toString();
                rec.replace(
                        rec.indexOf(Fields[i].getName()),
                        QSqlField(Fields[i].getName(),QVariant::String)
                        );
                rec.setValue(rec.indexOf(Fields[i].getName()),fileName);
            }
        }

    }

    //The record is updated using the original method
    if(!QSqlTableModel::updateRowInTable(row,rec))
        return false;

    //The files to delete are deleted
    {
    QFileInfoList::const_iterator i = fileDelete.constBegin();
    while (i != fileDelete.constEnd()) {
        qDebug()<<"Deleting: "<<i->absoluteFilePath();
        QFile delFile(i->absoluteFilePath());
        delFile.remove();
        ++i;
    }
    }

    //The new files to be copied are copied
    {
    QMap<QString, QString>::const_iterator i = fileTransfer.constBegin();
    while (i != fileTransfer.constEnd()) {
        QFile origFile(i.key());
        bool succ = origFile.copy(i.value());
        ++i;
    }
    }


    //The records in external fields are dealt with
    for(int i=0;i<sepRec.count();i++){
        QString tableName=sepRec.fieldName(i);
        QVector<QSqlRecord>  fileList=qvariant_cast<QVector<QSqlRecord> >(sepRec.value(i));
        for(int n=0;n<fileList.size();n++){
            QSqlRecord currRecord=fileList[n];
            if(currRecord.value("Status").toString()=="N"){
                insertRowIntoTable(fileList[n],origRecord,tableName);
            }else if(currRecord.value("Status").toString()=="R"){
                deleteFromTable(currRecord,tableName);
            }else if (currRecord.value("Status").toString()=="M"){
                updateRowInTable(currRecord,newRec,tableName);
            }
        }
    }
    /*
     * Once the value is changed in the main table if primary values have been changed
     * the folder has to be renames as well as all the values in the related tables
     */
    if (primChanged){
        qDebug()<<"Primary Changed";
        primaryChange(origRecord,newRec);
    }
    return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::updateRowInTable
/// Overloaded function to update values in an external table
/// \param values The files to be added to an externa table
/// \param primaryValues Primary values to find the correct row
/// \param table Name of the external table
/// \return
/// updateRowInTable: overloaded function used to update a row in an
/// external table. It writes the necessary SQL command and executes
/// it
bool MySqlTableModel::updateRowInTable(const QSqlRecord &values,QSqlRecord primaryValues,QString table){
    qDebug()<<"##########Calling updateRowInTable (overloaded)##########";
    QSqlRecord rec=values;

    /*
     * The where values record is populated with the required fields
     * namely the file name, and the primary values
     */
    QSqlRecord whereValues=QSqlRecord();
    whereValues.append(values.field("File"));
    QFileInfo fileName(values.value("File").toString());
    whereValues.setValue("File",fileName.fileName());
    rec.setValue("File", fileName.fileName());
    for(int i=0;i<primaryValues.count();++i){
        whereValues.insert(i,primaryValues.field(i));
        whereValues.setValue(i,primaryValues.value(i));
    }
    rec.setValue("Status","O");

    //Sql statement for updare and where are created
    QString stmt=QueryGenerator::UpdateStat(table,rec);
    QString whereStmt=QueryGenerator::WhereStat(whereValues);
    stmt.append(" ");
    stmt.append(whereStmt);
    QSqlQuery query;
    return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::primaryChange
/// Used when primary values of a record have been altered
/// \param oldPrim
/// \param newPrim
/// \return
/// primaryChange makes sure that if a primary value has changed all //
/// external tables are updated as well as the directories
bool MySqlTableModel::primaryChange(QSqlRecord oldPrim, QSqlRecord newPrim){
    //The old directory and new directory are created
    QString oldDir=GetDirectory(oldPrim);
    QString newDir=GetDirectory(newPrim);

    //The directory is renamed from the old to the new
    QDir renameDir;
    renameDir.rename(oldDir,newDir);


    /*
     * The new directory has a separator added to the end
     * The new record has a Diretory field added to it
     */

    //NOT NEEDED as Directory is not stored in the table anmore
    //newDir.append(QDir::separator());
    //newPrim.append(QSqlField("Directory"));
    for(int i=0;i<Fields.size();++i){
        //For every Filed not contained in the main table the records are updated
        if(Fields[i].extTable()){
            //The current table is recorded and added to the end of the directory
            //QString tableDir=newDir;
            //tableDir.append(Fields[i].getTable());
            //tableDir.append(QDir::separator());
            //The value for the current directory is set in the new record
            //newPrim.setValue("Directory",tableDir);
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

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::deleteRowFromTable
/// \param  The row to delete
/// \return
///
bool MySqlTableModel::deleteRowFromTable(int row){
    qDebug()<<"##########Calling deleteRowFromTable##########";
    QSqlRecord recToDel = primaryValues(row);
    /*
     * The row is removed using the original method. If it succeds then
     * the values in the external fields are removed as well
     */
    if(QSqlTableModel::deleteRowFromTable(row)){
        for(int i=0;i<Fields.count();++i){
            if(Fields[i].extTable()){
                QString stmt=QueryGenerator::DeleteWhereStat(Fields[i].getTable(),recToDel);
                        //DeleteStat(Fields[i].getTable(),recToDel);
                QSqlQuery dltQuery;
                if(!dltQuery.exec(stmt)){
                    qDebug()<<"Returning false";
                    return false;
                }
            }
        }
        //After deleting the database entries the folders are removed
        deleteRecordDirectory(recToDel);
        return true;
    }
    qDebug()<<"The Error: "<<this->lastError();
    return false;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::deleteFromTable
/// \param values
/// \param tableName
/// \return
/// Overloaded function used to remove individial values from external
/// tables
bool MySqlTableModel::deleteFromTable(const QSqlRecord &values, QString tableName){
    qDebug()<<"##########Calling deleteRowFromTable (overloaded)##########";
    QString fileDir=GetDirectory(values,MyField(tableName));
            //values.value("Directory").toString();
    fileDir.append(QDir::separator());
    fileDir.append(values.value("File").toString());
    QFile fileToDel(fileDir);

    QSqlRecord primValues=database().primaryIndex(tableName);
    for (int i=0;i<primValues.count();i++){
        primValues.setValue(i,values.value(primValues.fieldName(i)));
    }
    QString dltStmt =QueryGenerator::DeleteWhereStat(tableName,primValues);
    QSqlQuery delQuery;
    if (delQuery.exec(dltStmt)){
        if(fileToDel.remove())
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::copyFile
/// \param record
/// \param primaryValues
/// \param table
/// \return
/// Used to copy a file (contained in record) to the correct position within
/// the directory hiererchy of the database
QString MySqlTableModel::copyFile(const QSqlRecord &record,QSqlRecord primaryValues, QString table){
    //The destination is obtained as a combination of getDirectory and the original filename
    QString dest = GetDirectory(primaryValues,MyField(table));
    QFileInfo origFileInfo(record.value("File").toString());
    dest.append(origFileInfo.fileName());
    //The filename is purged of any white spaces
    dest.replace(" ","_");

    QFile origFile(record.value("File").toString());
    if(origFile.copy(dest)){
        return QDir::current().relativeFilePath(dest);
    }
    return "";
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::removeFile
/// \param record
/// \param field
/// \return
/// Removes the file specified in record stored in fied "field"
/// NOT USED ANYWHERE
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
    for(int i=0;i<returnRecord.count();i++){
        int col=fieldIndex(returnRecord.fieldName(i));
        if (Fields[col].extTable()){
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

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::canUpdate
/// Checks whether the avlbRecord can be inserted in the main table
/// \param avlbRecord
/// \return
bool MySqlTableModel::canUpdate(QSqlRecord *avlbRecord){
    QSqlRecord primValues = getPrimary(avlbRecord);
    QString stmt= QueryGenerator::SelectStat("Main_table");
    QString wStmt=QueryGenerator::WhereStat(primValues);
    stmt.append(' ');
    stmt.append(wStmt);
    QSqlQuery qCanUpdate =QSqlQuery();
    qCanUpdate.exec(stmt);

    while(qCanUpdate.next()){
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::getPrimary
/// Returns the primary values for the given row (index)
/// \param index
/// \return
///
QSqlRecord MySqlTableModel::getPrimary(const QModelIndex &index) const{
    QSqlRecord record=primaryRecord;
    for(int i=0;i<record.count();++i){
        QModelIndex nIndex = QSqlTableModel::index(index.row(),baseRecord.indexOf(record.fieldName(i)));
        record.setValue(i,data(nIndex,Qt::EditRole));
    }
    return record;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::getPrimary
/// \param row
/// \return
/// Returns the primary values for the given row
QSqlRecord MySqlTableModel::getPrimary(int row) const{
    QModelIndex indx = index(row,0);
    return getPrimary(indx);
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::getPrimary
/// Returns the primary values for the given record
/// \param curr_Record
/// \return
///
QSqlRecord MySqlTableModel::getPrimary(QSqlRecord *curr_Record) const{
    QSqlRecord record=primaryRecord;
    for(int i=0;i<record.count();++i)
        record.setValue(i,curr_Record->value(curr_Record->indexOf(record.fieldName(i))));
    return record;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::createDirectory
/// Creates the necessary directories for a newly added record
/// \param newRec
/// \return
///
bool MySqlTableModel::createDirectory(QSqlRecord newRec){
   QDir baseDir(database().databaseName());
   baseDir.cdUp();
   QString createDir="";

   QSqlRecord primValues=getPrimary(&newRec);
   createDir = GetDirectory(primValues);
   if(!baseDir.mkdir(createDir))
       return false;
   QString addDir;
   for(int i=0;i<Fields.size();++i){
       if(Fields[i].extTable() || Fields[i].getType()==DataType::Image){
           addDir = GetDirectory(primValues,Fields[i]);
           if(!baseDir.mkdir(addDir)){
               deleteRecordDirectory(newRec);
               return false;
           }
       }
   }
   return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::deleteRecordDirectory
/// Deletes the directory for the passed record
/// \param newRec
/// \return
///
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

//UNUSED METHOD-> OBSOLETE UNDER NEW WORKING OF IMAGES AND IMAGE
QSqlRecord MySqlTableModel::separateDir(QSqlRecord altRecord){
    QFileInfo compDirectory = QFileInfo(altRecord.value("File").toString());
    altRecord.append(QSqlField("Directory",QVariant::String));
    altRecord.setValue("Directory",compDirectory.absolutePath().append(QDir::separator()));
    altRecord.setValue("File",compDirectory.fileName());
    return altRecord;

}


/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::GetDirectory
/// returnes the directory for a given record
/// \param newRec
/// \param relative
/// \return
///
QString MySqlTableModel::GetDirectory(QSqlRecord newRec, bool relative) const{
    QString newDir;
    QString baseDir = "";
    if(!relative){
        baseDir = QDir::currentPath();
        baseDir.append(QDir::separator());
    }


    newRec = getPrimary(&newRec);
    for(int i=0;i<newRec.count();i++){
        newDir.append(newRec.value(i).toString());
        newDir.append('_');
    }
    newDir.chop(1);
    newDir.replace(" ","_");
    return baseDir+newDir;

}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::GetDirectory
/// Returnes the directory for a specific field of the given record
/// \param Rec
/// \param field
/// \param relative
/// \return
///
QString MySqlTableModel::GetDirectory(QSqlRecord Rec,MyField field, bool relative) const{
    QString baseDir= GetDirectory(Rec, relative);
    baseDir.append(QDir::separator());
    QString fieldDir = field.getName();
    fieldDir.replace(" ","_");
    return baseDir+fieldDir;
}

//PROBABLY USELESS FUNCTION
/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::insertImages
/// Applies the necessary modifications to the files when Images or Image
/// is edited
/// \param TableName
/// \param Primary_Values
/// \param Images
/// \return
///
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
            QString addQuery=QueryGenerator::InsertStat(TableName,currImage);
        } else if (currImage.value("Status").toString()=="R"){            
            QString Filepath=GetDirectory(Primary_Values);
            Filepath.append(currImage.value("File").toString());
            QDir remDir = QDir(Filepath);
            remDir.remove(Filepath);

        }
    }
    return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::editTableStructure
/// Applies the given changes to the structure of the database
/// \param changes
/// \return
///
bool MySqlTableModel::editTableStructure(QList<FieldEdit*> changes){

    if (changes.isEmpty())
        return true;
    QStringList changeCommands;
    bool rewrite = false;
    foreach(FieldEdit *change, changes){
        if (change->mainEdit()){
            rewrite = true;
            break;
        }
    }
    if (rewrite){
        changeCommands.append(QueryGenerator::multipleChanges("Main_table",changes));
    }else{
        foreach (FieldEdit *change, changes){
            changeCommands.append(QueryGenerator::editField(*change));
        }
    }
    QSqlQuery query;
    bool success = true;
    qDebug()<<"Running editing Commands";
    //QFile dbFile = Database.databaseName();
    //QFile dbBckup("db_backup.sqlite");
    //dbFile.copy(dbBckup);
    foreach (QString command, changeCommands){
        qDebug()<<command;
        success = (success && query.exec(command));
        qDebug()<<"Was executed succesfully"<<success;
        if (!success){
            qDebug()<<query.lastError();
        }
    }
    if (success){
       //dbBckup.remove();
        if(rewrite){
            foreach(FieldEdit *change, changes){
                qDebug()<<"MySqlTableModel::editTableStructure(): Situation for "<<change->getOldField().getName();
                qDebug()<<"Action: "<<change->getAction();
                qDebug()<<"Old field had Directory: "<<change->getOldField().extDir();
                qDebug()<<"New Field has directory: "<<change->getField().extDir();
            //The field has been removed and is a field which requires directories
                if (change->getAction()=="Remove" &&
                        (change->getOldField().extDir())){
                    qDebug()<<"MySqlTableModel::editTableStructure(): Removing directory";
                    deleteFieldDirectory(change->getOldField());
            //The field has been added and is a field which requires directories
                }else if (change->getAction()=="New" &&
                          (change->getField().extDir())){
                    qDebug()<<"MySqlTableModel::editTableStructure(): Creating directory";
                    createFieldDirectory(change->getField());
                //The field has been edited
                }else if(change->getAction()=="Edit"){
                    //It used to require directories and still does
                    if (change->getField().extDir() && change->getOldField().extDir()){
                        qDebug()<<"MySqlTableModel::editTableStructure(): Renaming directory";
                        renameFieldDirectory(change->getOldField(),change->getField());
                    //It didn't use to require directories but now it does
                    }else if(change->getField().extDir() && !change->getOldField().extDir()){
                        qDebug()<<"MySqlTableModel::editTableStructure(): Creating directory";
                        createFieldDirectory(change->getField());
                    //It used to require directories but now it doesn't anymore
                    }else if(!change->getField().extDir() && change->getOldField().extDir()){
                        qDebug()<<"MySqlTableModel::editTableStructure(): Removing directory";
                        deleteFieldDirectory(change->getOldField());
                    }
                }
            }
        }
    }else{
        //should cose db and reload the old one;
        return false;
    }

    return true;
}

//Needs to be checked
/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::createFieldDirectory
/// Creates the necessary subdirectory for a new field
/// \param newField
/// \return
///
bool MySqlTableModel::createFieldDirectory(MyField newField){
    QDir currDir = QDir::current();
    QSqlRecord currRecord;
    while(canFetchMore()){
        fetchMore();
    }
    for (int i =0; i < rowCount();i++){
        currRecord = record(i);
        QString fieldDir = GetDirectory(currRecord,newField,true);
        qDebug()<<"Making: "<<fieldDir;
        currDir.mkdir(fieldDir);
    }

}

/////////////////////////////////////////////////////////////////////
/// \brief MySqlTableModel::deleteFieldDirectory
/// Deletes the directories for a specific field
/// \param oldField
/// \return
///
bool MySqlTableModel::deleteFieldDirectory(MyField oldField){
    QDir currDir = QDir::current();
    QDir deleteDir;
    QSqlRecord currRecord;
    while(canFetchMore()){
        fetchMore();
    }
    for (int i =0; i < rowCount();i++){
        deleteDir = currDir;
        currRecord = record(i);
        QString fieldDir = GetDirectory(currRecord,oldField,true);
        deleteDir.cd(fieldDir);
        qDebug()<<"Deleting: "<<deleteDir.absolutePath();
        deleteDir.removeRecursively();
    }
}

bool MySqlTableModel::renameFieldDirectory(MyField oldField, MyField newField){
    QDir currDir = QDir::current();
    QSqlRecord currRecord;
    while(canFetchMore()){
        fetchMore();
    }
    for (int i =0; i < rowCount();i++){
        currRecord = record(i);
        QString oldDir = GetDirectory(currRecord,oldField,true);
        QString newDir = GetDirectory(currRecord,newField,true);
        qDebug()<<"Renaming: "<<oldDir<<" to: "<<newDir;
        currDir.rename(oldDir,newDir);
    }
}
