#include "querygenerator.h"
#include <QString>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>
#include <QDebug>
#include <QSqlField>
#include "datatype.h"


/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::SelectStat
/// Generates a SQL Select statement
/// \param Table_name
/// \param Columns
/// \return
///
QString QueryGenerator::SelectStat(const QString Table_name, const QSqlRecord *columns){
    QString ReturnString="SELECT";

    if (columns->isEmpty()){
        ReturnString="SELECT *";
    }else{
        for(int i=0;i<columns->count();i++){
            ReturnString.append(QString(" \"%1\",").arg(columns->field(i).name()));
        }
        ReturnString.chop(1);
    }

    ReturnString.append(QString(" FROM \"%1\"").arg(Table_name));
    ReturnString.append(";");
    return ReturnString;
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::SelectStat
/// Generates a SQL Select statement
/// \param Table_name
/// \param Columns
/// \return
///
QString QueryGenerator::SelectStat(const QString Table_name, const QVector<MyField> Columns){
    QString ReturnString="SELECT";
    if (Columns.isEmpty()){
        ReturnString="SELECT *";
    }else{
        for(int i=0;i<Columns.size();i++){
            ReturnString.append(QString(" \"%1\",").arg(Columns[i].getName()));
        }
        ReturnString.chop(1);
    }

    ReturnString.append(QString(" FROM \"%1\"").arg(Table_name));
    ReturnString.append(";");
    return ReturnString;
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::WhereStat
/// Generates SQL Where statement
/// \param where_rec
/// \param comparison
/// \return
///
QString QueryGenerator::WhereStat(const QSqlRecord *where_rec,const QString comparison){
    QString ReturnString = "WHERE";
    QStringList comparisons({"LIKE", "=", "<>", ">", "<", "<="});
    QString comp = comparison;
    if (!comparisons.contains(comparison)){
        comp = "LIKE";
    }
    for(int i =0;i<where_rec->count();i++){
        ReturnString.append(QString(" \"%1\" %3 \'%2\' AND").arg(where_rec->fieldName(i),where_rec->value(i).toString(),comp));
    }
    ReturnString.chop(4);
    ReturnString.append(";");
    return ReturnString;
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::DeleteStat
/// Generates SQL Delete statement for delete_rec
/// \param Table_name
/// \param delete_rec
/// \return
///
QString QueryGenerator::DeleteWhereStat(const QString Table_name, const QSqlRecord *delete_rec, const QString comparison){
    QString deleteString = QString("DELETE FROM \"%1\"").arg(Table_name);
    deleteString.append(' ');
    deleteString.append(WhereStat(delete_rec, comparison));
    //deleteString.append(";");
    return deleteString;
}



/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::UpdateStat
/// Generates SQL Update statement (requires a where statement)
/// \param Table_name
/// \param update_rec
/// \return
///
QString QueryGenerator::UpdateStat(const QString Table_name, const QSqlRecord *update_rec){
    QString ReturnString=QString("UPDATE \"%1\" SET").arg(Table_name);
    for (int i=0;i<update_rec->count();i++){
        ReturnString.append(QString(" \"%1\" = \'%2\',").arg(update_rec->fieldName(i),update_rec->value(i).toString()));
    }
    ReturnString.chop(1);
    ReturnString.append(";");
    return ReturnString;
}


QString QueryGenerator::Where2TableStat(const QString origTable, const QString destTable, const QVector<MyField> fields){
    QString returnStatement = "WHERE ";
    foreach(MyField field, fields ){
        returnStatement.append(QString("\"%1\".\"%3\"=\"%2\".\"%3\" AND ").arg(origTable,destTable, field.getName()));
    }
    returnStatement.chop(5);
    return returnStatement;
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::SelectWhereStat
/// \param Table_name
/// \param Query_Where
/// \param comparison
/// \return
///
QString QueryGenerator::SelectWhereStat(const QString Table_name, const QSqlRecord *where_rec, const QString comparison){
    QString temp_query;
    temp_query = SelectStat(Table_name);
    temp_query.chop(1);
    temp_query.append(" ");

    temp_query.append(WhereStat(where_rec,comparison));
    qDebug()<<"Select Where Stat: "<<temp_query;
    //temp_query.append(";");
    return(temp_query);
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::SelectWhereStat
/// \param Table_name
/// \param Query_Where
/// \param comparison
/// \return
///
QString QueryGenerator::SelectWhereStat(const QString Table_name, const QSqlRecord *where_rec, const QSqlIndex *selFields, const QString comparison){
    QString temp_query;
    temp_query = SelectStat(Table_name, selFields);
    temp_query.chop(1);
    temp_query.append(" ");

    temp_query.append(WhereStat(where_rec,comparison));
    //temp_query.append(";");
    qDebug()<<"Select Where Stat: "<<temp_query;
    return(temp_query);
}


/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::createInsert
/// Generates Insert statement for the rec_Insert record
/// \param Table_name
/// \param rec_Insert
/// \return
///
QString QueryGenerator::InsertStat(const QString Table_name,const QSqlRecord *rec_Insert){
    QString temp_query;
    QString values;
    QString fields;
    temp_query=QString("INSERT INTO \"%1\" (").arg(Table_name);
    qDebug()<<rec_Insert;
    for (int i=0;i<rec_Insert->count();i++){
        fields.append(QString("\"%1\",").arg(rec_Insert->fieldName(i)));
        values.append(QString("\'%1\',").arg(rec_Insert->value(i).toString()));
    }

    values.chop(1);
    fields.chop(1);
    temp_query.append(QString("%1) VALUES (%2);").arg(fields, values));

    return temp_query;
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::UpdateWhereStat
/// Generates Update statement with where clause
/// \param Table_name
/// \param rec_Update
/// \param rec_Where
/// \param comparison
/// \return
///
QString QueryGenerator::UpdateWhereStat(const QString Table_name, const QSqlRecord *rec_Update, const QSqlRecord *rec_Where, const QString comparison) {
    QString temp_query = UpdateStat(Table_name,rec_Update);
    temp_query.chop(1);
    temp_query.append(" ");
    temp_query.append(WhereStat(rec_Where, comparison));
    return temp_query;
}

QString QueryGenerator::UpdateFromTable(const QString origTable, const QString destTable, const QVector<MyField> copyRec, const QVector<MyField> refRec){
    QString returnStat = QString("UPDATE \"%1\" SET ").arg(destTable);
    QString colStat;
    foreach(MyField rec, copyRec){
        colStat = QString("\"%1\" = (SELECT \"%1\" FROM  \"%2\" %3), ").arg(rec.getName(),
                                                                                origTable,Where2TableStat(origTable, destTable, refRec));
        returnStat.append(colStat);
    }
    returnStat.chop(2);
    returnStat.append(";");
    return returnStat;
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::createTable
/// Old create Table statement generator
/// \param Table_name
/// \param Fields_values
/// \param Primary
/// \param Unique
/// \return
///
QString QueryGenerator::createTable(const QString Table_name,const QVector<QVector<QString> > Fields_values,const QVector<QString> Primary,const QVector<QString> Unique){
    QString temp_query;
    temp_query=QString("CREATE TABLE \"%1\" (").arg(Table_name);
    for(int n=0;n<Fields_values.size();n++){
        temp_query.append(QString(" \"%1 %2,").arg(Fields_values[n][0],Fields_values[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(" PRIMARY (");
    for(int n=0;n<Primary.size();n++){
        temp_query.append(QString(" \"%1\",").arg(Primary[n]));
    }
    temp_query.chop(1);
    temp_query.append(")");
    if(!Unique.isEmpty()){
        temp_query.append(QString(" UNIQUE ("));
        for(int n=0;n<Unique.size();n++){
            temp_query.append(QString(" \"%1,").arg(Unique[n]));
        }
        temp_query.chop(1);
        temp_query.append(")");
    }
    temp_query.append(";");
    return(temp_query);
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::createTable
/// Generates a create table statement
/// \param Table_name
/// \param fields
/// \return
///
QString QueryGenerator::createTable(const QString Table_name,const QVector<MyField> fields){
    QString temp_query;
    temp_query=QString("CREATE TABLE \"%1\" (").arg(Table_name);
    //QStringList primaryFields;
    //QStringList uniqueFields;
    QString primaryFields = "";
    QString uniqueFields = "";
    bool primary;
    bool unique;
    foreach(MyField currField, fields){
        primary = false;
        unique = false;
        if (currField.getPrimary()){
            primary = true;
            primaryFields.append(QString(" \'%1\',").arg(currField.getName()));
        }
        if (currField.getUnique()){
            uniqueFields.append(QString(" \'%1\',").arg(currField.getName()));
            unique = true;
        }
        /*if (primary || unique){
            temp_query.append(QString(" \'%1\' %2 NOT NULL DEFAULT,").arg(currField.getName(),
                                                                  DataType::getSqlType(currField.getType())));
        }else{*/
        temp_query.append(QString(" \"%1\" %2,").arg(currField.getName(),
                                                              DataType::getSqlType(currField.getType())));
        //}
    }
    temp_query.chop(1);
    uniqueFields.chop(1);
    primaryFields.chop(1);
    if (primaryFields!=""){
        temp_query.append(QString(", PRIMARY KEY (%1)").arg(primaryFields));
    }
    if (uniqueFields!=""){
        temp_query.append(QString(", Unique (%1)").arg(uniqueFields));
    }
    /*foreach(QString primaryField, primaryFields){
        temp_query.append(QString(" \'%1\',").arg(primaryField));
    }
    temp_query.chop(1);
    temp_query.append(")");
    if(uniqueFields.count()>0){
        temp_query.append(QString(", UNIQUE ("));
        foreach(QString uniqueField, uniqueFields){
            temp_query.append(QString(" \'%1\',").arg(uniqueField));
        }
        temp_query.chop(1);
        temp_query.append(")");
    }*/

    temp_query.append(");");
    return(temp_query);
}


/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::createExtTable
/// Generates a create table statement for an external table, createTable should suffice
/// \param Table_name
/// \param fields
/// \return
///
QString QueryGenerator::createExtTable(const QString Table_name,const QVector<MyField> fields){
    QString temp_query;
    temp_query=QString("CREATE TABLE \'%1\' (").arg(Table_name);

    foreach(MyField currField, fields){

        if (currField.getPrimary()){
            temp_query.append(QString(" \"%1\" %2 NOT NULL,").arg(currField.getName(),
                                                                  DataType::getSqlType(currField.getType())));
        }
    }
    temp_query.append("\"File\" VARCHAR(50) NOT NULL, \"Description\" TEXT, \"Status\" CHAR");
    //temp_query.chop(1);
    temp_query.append(", PRIMARY KEY (File));");

    return(temp_query);
}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::copyTable
/// Generates the statement to copy origFields from orig table to
/// destFields in destTable
/// \param origTableName
/// \param destTableName
/// \param origFields
/// \param destFields
/// \return
///
QString QueryGenerator::copyTable(const QString origTableName, const QString destTableName, const QVector<MyField> origFields, const QVector<MyField> destFields){
    QString copyTable = QString("INSERT INTO \"%1\" SELECT").arg(destTableName);
    for (int i = 0; i<destFields.count(); i++){
        copyTable.append(QString(" \"%1\",").arg(destFields[i].getName()));
    }
    copyTable.chop(1);
    copyTable.append(") SELECT (");
    for (int i = 0; i<origFields.count(); i++){
        copyTable.append(QString(" \"%1\",").arg(origFields[i].getName()));
    }
    copyTable.chop(1);
    copyTable.append(QString(") FROM \"%1\"").arg(origTableName));
    return copyTable;
}

QString QueryGenerator::copyExtTable(const QString origTableName, const QString destTableName, const QVector<MyField> origFields, const QVector<MyField> destFields){

}

/////////////////////////////////////////////////////////////////////
/// \brief QueryGenerator::insertField
/// Generates the statements necessary to insert a new field in a table
/// \param tableName
/// \param fields
/// \param newField
/// \return
///
QStringList QueryGenerator::insertField(QString tableName,QVector<MyField> fields, MyField newField){
    QStringList commands;
    commands.append("PRAGMA foreign_keys = 0;");
    commands.append(QString("CREATE TABLE temp_table AS SELECT * FROM %1;").arg(tableName));
    commands.append(QString("DROP TABLE %1;").arg(tableName));
    QVector<MyField> newFields = fields;
    newFields.append(newField);
    commands.append(createTable(tableName, newFields));
    commands.append(copyTable("temp_table",tableName,fields,fields));
    commands.append("DROP TABLE temp_table;");
    commands.append("PRAGMA foreign_keys = 1;");

    QSqlRecord rec_field = newField;//.toSqlRecord();
    //QSqlField tempField("Name",QVariant::String);
    /*rec_field.append(QSqlField("Name",QVariant::String));
    rec_field.setValue("Name",newField.getName());
    rec_field.append(QSqlField("Type",QVariant::Int));
    rec_field.setValue("Type",(int)(newField.getType()));
    rec_field.append(QSqlField("Vis_Preview",QVariant::Bool));
    rec_field.setValue("Vis_Preview",newField.getVisPreview());
    rec_field.append(QSqlField("Vis_Table",QVariant::Bool));
    rec_field.setValue("Vis_Table",newField.getVisTable());
    rec_field.append(QSqlField("Position",QVariant::Int));
    rec_field.setValue("Position",newField.getPos());
    rec_field.append(QSqlField("Primary",QVariant::Bool));
    rec_field.setValue("Primary",newField.getPrimary());
    rec_field.append(QSqlField("Comments",QVariant::String));
    rec_field.setValue("Comments",newField.getComments());*/
    commands.append(InsertStat("Fields_table", &rec_field));
    /*QString fieldInsert;
    fieldInsert=QString("INSERT INTO \'Fields_table\' (\'Name\', \'Type\', \'Vis_Preview\', \'Vis_Table\', \'Position\', \'Primary\', \'Comments\') VALUES (");
    fieldInsert.append(QString("\'%1\',").arg(newField.getName()));
    fieldInsert.append(QString(" \'%1\,'").arg((int)(newField.getType())));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisPreview()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisTable()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPos()));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPrimary()?"True":"False"));
    fieldInsert.append(QString(" \'%1\'").arg(newField.getComments()));*/

    //fieldInsert.append(QString("\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\')").arg(newField.getName(),int(newField.getType()),newField.getVisPreview(),newField.getVisTable(),
                                                                                       // newField.getPos(), newField.getPrimary(), newField.getComments()));

    //commands.append(fieldInsert);
    return commands;

}

QString QueryGenerator::insertField(const MyField newField){
    QString fieldInsert;
    QString tableName = "Main_table";
    if (newField.getType()==DataType::Images){
        tableName = newField.getName();
    }
    /*fieldInsert=QString("INSERT INTO \'Fields_table\' (\'Name\', \'Table', \'Type\', \'Vis_Preview\', \'Vis_Table\', \'Position\', \'Primary\', \'Comments\') VALUES (");
    fieldInsert.append(QString("\'%1\',").arg(newField.getName()));
    fieldInsert.append(QString("\'%1\',").arg(tableName));
    fieldInsert.append(QString(" \'%1\',").arg((int)(newField.getType())));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisPreview()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisTable()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPos()));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPrimary()?"True":"False"));
    fieldInsert.append(QString(" \'%1\'").arg(newField.getComments()));
    fieldInsert.append(");");*/
    fieldInsert = InsertStat(tableName, &newField);//.toSqlRecord());
    //fieldInsert.append(QString("\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\')").arg(newField.getName(),int(newField.getType()),newField.getVisPreview(),newField.getVisTable(),
                                                                                       // newField.getPos(), newField.getPrimary(), newField.getComments()));
    return fieldInsert;

}

QString QueryGenerator::editField(const FieldEdit fieldChanges){
    MyField oldField = fieldChanges.getOldField();
    MyField field = fieldChanges.getField();
    QString command = UpdateWhereStat("Fields_table", &field,
                                      &oldField);//.toSqlRecord()

    /*command=QString("UPDATE \'Fields_table\' SET Name=\'%1\', ").arg(fieldChanges.getField().getName());
    command.append(QString("Type=\'%1\', ").arg((int)(fieldChanges.getField().getType())));
    command.append(QString("Vis_Preview=\'%1\', ").arg(fieldChanges.getField().getVisPreview()?"True":"False"));
    command.append(QString("Vis_Table=\'%1\', ").arg(fieldChanges.getField().getVisTable()?"True":"False"));
    command.append(QString("Position=\'%1\', ").arg(fieldChanges.getField().getPos()));
    command.append(QString("Primary=\'%1\', ").arg(fieldChanges.getField().getPrimary()?"True":"False"));
    command.append(QString("Comments=\'%1\'").arg(fieldChanges.getField().getComments()));



    command.append(QString(" WHERE Name=\'%1\'").arg(fieldChanges.getOldField().getName()));*/
    return command;
}

QStringList QueryGenerator::removeField(const QString tableName, const QVector<MyField> fields, const MyField field){
    QVector<MyField> oFields;
    QStringList commands;
    commands.append("PRAGMA foreign_keys = 0;");
    commands.append(QString("CREATE TABLE \"%1_temp_table\" AS SELECT * FROM \"%1\";").arg(tableName));
    commands.append(QString("DROP TABLE \"%1\";").arg(tableName));
    //QVector newFields = fields;
    //newFields.replace(newFields.lastIndexOf(oldField),newField);
    oFields.remove(fields.lastIndexOf(field));
    commands.append(createTable(tableName, oFields));
    commands.append(copyTable(QString("%1_temp_table").arg(tableName),tableName,oFields,oFields));
    commands.append(QString("DROP TABLE \"%1_temp_table\";").arg(tableName));
    commands.append("PRAGMA foreign_keys = 1;");

    //QString fieldInsert;
    //fieldInsert = QString("DELETE FROM \'Fields_table\' WHERE \'Name\'=\'%1\'").arg(field.getName());
    //commands.append(fieldInsert);
    return commands;
}

QStringList QueryGenerator::multipleChanges(const QString tableName, const QList<FieldEdit *> changes){
    QStringList commands;

    //Create two vectors to have both the new and old fields
    QVector<MyField> newFields;
    QVector<MyField> newExtTabFields;

    QVector<MyField> constPrim;
    QStringList oldExtTables;
    QStringList newExtTables;
    bool editExtTable = false;
    //First loop to store the names of all external tables
    foreach (FieldEdit * change, changes){
        if (change->getOldField().extTable() && !change->getField().extTable())
            commands.append(QString("DROP TABLE \"%1\"").arg(change->getOldField().getName()));
        if (change->getOldField().extTable() && change->getField().extTable())
            oldExtTables.append(change->getField().getName());
        if ((change->getField().extTable() && change->getAction()=="New") ||
            (change->getField().extTable() && !change->getOldField().extTable())){
            newExtTables.append(change->getField().getName());
        }

        if(change->getAction()!="New" && change->getAction()!="Remove" &&
                change->getField().getPrimary() && change->getOldField().getPrimary()){
            constPrim.append(change->getField());
        }
        /*
         * fairly long and complex if statement to determin whether the external
         * tables have to be updated or not. The hole if statement could be done in one
         * statement but it would make it complex, so it's divided in a series of smaller
         * checks. The first check is to minimize the work. If the tables have to be edited
         * for one reason they will be edited for all reasons, so once editExtTable is true
         * the check will no longer be performed
         */
        if (!editExtTable){
            qDebug()<<"QueryGenerator::multipleChanges: action: "<<change->getAction();
            qDebug()<<"QueryGenerator::multipleChanges: new Primary: "<<change->getField().getPrimary();
            qDebug()<<"QueryGenerator::multipleChanges: old Primary: "<<change->getOldField().getPrimary();
            if (change->getAction()=="New" && change->getField().getPrimary()){
                qDebug()<<"Setting editExtTableTrue becasue a new primary field was added";
                editExtTable=true;
            }else if (change->getAction()=="Remove" && change->getOldField().getPrimary()){
                qDebug()<<"Setting editExtTableTrue becasue a primary field was removed";
                editExtTable=true;
            }else if (change->getAction()=="Edit" &&
                (change->getOldField().getPrimary()||change->getField().getPrimary())){
                qDebug()<<"The field was primery either before of after or both";
                if(change->getOldField().getName()!=change->getField().getName()){
                    qDebug()<<"Setting editExtTableTrue becasue a Primary Field changed name";
                    editExtTable = true;
                }else if(change->getOldField().getPrimary()!=change->getField().getPrimary()){
                    qDebug()<<"Setting editExtTableTrue becasue a film was changed to Primary/Non-Primary";
                    editExtTable = true;
                }
            }
        }
    }
    qDebug()<<"QueryGenerator::multipleChanges has done a first loop around. The old Primary fields are:";




    /*
     * The copy table is used to copy all the required old fields in the new table
     * in the correct position
     */
    QString copyTable = QString("INSERT INTO \"%1\" (").arg(tableName);
    QString origField = QString("");
    QString destField = QString("");

    /*
     * A similar thing is done to copy the fields from external tables
     */
    QString origExtField = QString("");
    QString destExtField = QString("");


    QStringList deleteCommands;
    QStringList insertCommands;
    QStringList editCommands;

    foreach (FieldEdit *change, changes){
        MyField oldField = change->getOldField();
        MyField field = change->getField();
        qDebug()<<"QueryGenerator::multipleChanges is studying change to: "<<change->getOldField().getName();
        qDebug()<<"The action is: "<<change->getAction();
        if (change->getAction()!= "Remove"){
            newFields.append(change->getField());
            if (change->getField().getPrimary()){
                newExtTabFields.append(change->getField());
            }
        }

        if(change->getAction()=="Edit")
            editCommands.append(editField(*change));


        ////OPERATIONS TO DO IS REMOVING A FIELD
        if(change->getAction()=="Remove"){
            //Delete any field from the field's statement which was removed
            deleteCommands.append(DeleteWhereStat("Fields_table",&oldField,"="));
            //If the old field was an external table, the table is dropped
            if (change->getOldField().extTable())
                deleteCommands.append(QString("DROP TABLE \"%1\";").arg(change->getOldField().getTable()));
        ////OPERATIONS TO DO FOR A NEW FIELD
        }else if (change->getAction()=="New"){
            //Fields are inserted in the the field table
            insertCommands.append(InsertStat("Fields_table", &field));//.toSqlRecord()));
                        //insertField(change->getField()));
        ////OPERATIONS TO DO FOR EDITING A FIELD
        }else{
            origField.append(QString(" \"%1\",").arg(change->getOldField().getName()));
            destField.append(QString(" \"%1\",").arg(change->getField().getName()));
            /*
             * If the field was a primary before and after then it is stored to copy the old
             * fields to the new table
             */
            if (change->getOldField().getPrimary() && change->getField().getPrimary()){
                origExtField.append(QString(" \"%1\",").arg(change->getOldField().getName()));
                destExtField.append(QString(" \"%1\",").arg(change->getField().getName()));
            }


        }
    }
    origField.chop(1);
    destField.chop(1);
    copyTable.append(QString(" %1) SELECT %2 FROM \"temp_table\";").arg(destField,origField));

    origExtField.append(" \"File\", \"Description\", \"Status\"");
    destExtField.append(" \"File\", \"Description\", \"Status\"");

    newExtTabFields.append(MyField("File","",DataType::LongText,false,false,0,true));
    newExtTabFields.append(MyField("Description","",DataType::LongText));
    newExtTabFields.append(MyField("Status","",DataType::Text));

    commands.append("PRAGMA foreign_keys = 0;");
    commands.append(QString("CREATE TABLE \"temp_table\" AS SELECT * FROM \"%1\";").arg(tableName));
    commands.append(QString("DROP TABLE \"%1\";").arg(tableName));
    commands.append(createTable(tableName, newFields));
    commands.append(copyTable);
    commands.append("DROP TABLE \"temp_table\";");
    commands.append("PRAGMA foreign_keys = 1;");
    commands.append(deleteCommands);
    commands.append(insertCommands);
    commands.append(editCommands);
    if (editExtTable){
        foreach(QString extTable, oldExtTables){
            copyTable = QString("INSERT INTO \"%1\" (").arg(extTable);
            copyTable.append(QString(" %1) SELECT %2 FROM \"%3_temp_table\";").arg(destExtField,origExtField, extTable));
            commands.append(QString("CREATE TABLE \"%1_temp_table\" AS SELECT * FROM \"%1\";").arg(extTable));
            commands.append(QString("DROP TABLE \"%1\";").arg(extTable));
            commands.append(createTable(extTable, newExtTabFields));
            commands.append(copyTable);
            commands.append(QString("DROP TABLE \"%1_temp_table\";").arg(extTable));


        }

        foreach(QString extTable, newExtTables){
            commands.append(createTable(extTable, newExtTabFields));
        }
        newExtTabFields.pop_back();
        newExtTabFields.pop_back();
        newExtTabFields.pop_back();
        foreach(QString extTable, oldExtTables){
            commands.append(UpdateFromTable("Main_table", extTable,newExtTabFields,constPrim));
        }
    }



    return commands;


}

QStringList QueryGenerator::editTable(const QString tableName, const QList<FieldEdit *> changes){
    QStringList commands;
    QString origField = "";
    QString destField = "";
    QVector<MyField> newFields;
    //QVector<MyField>;

    QString copyTable = QString("INSERT INTO \"%1\" (").arg(tableName);

    foreach(FieldEdit *change,changes){
        if (change->getAction()!= "Remove"){
            newFields.append(change->getField());
        //if (change->getAction() == "New"){
            //newFields.append(change->getField());
        }
        if (change->getAction() == "Edit" || change->getAction() == ""){
            origField.append(QString(" \"%1\",").arg(change->getOldField().getName()));
            destField.append(QString(" \"%1\",").arg(change->getField().getName()));
        }
    }

    origField.chop(1);
    destField.chop(1);
    copyTable.append(QString(" %1) SELECT %2 FROM \"temp_table\";").arg(destField,origField));

    commands.append("PRAGMA foreign_keys = 0;");
    commands.append(QString("CREATE TABLE \"temp_table\" AS SELECT * FROM \"%1\";").arg(tableName));
    commands.append(QString("DROP TABLE \"%1\";").arg(tableName));
    commands.append(createTable(tableName, newFields));
    commands.append(copyTable);
    commands.append("DROP TABLE \"temp_table\";");
    commands.append("PRAGMA foreign_keys = 1;");
    return commands;
}

