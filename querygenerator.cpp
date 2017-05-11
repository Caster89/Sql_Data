#include "querygenerator.h"
#include <QString>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>
#include <QDebug>
#include "datatype.h"



QString QueryGenerator::SelectStat(QString Table_name, QVector<QString> Columns){
    QString ReturnString="SELECT";
    if (Columns.isEmpty()){
        ReturnString="SELECT *";
    }else{
        for(int i=0;i<Columns.size();i++){
            ReturnString.append(QString(" \"%1\",").arg(Columns[i]));
        }
        ReturnString.chop(1);
    }

    ReturnString.append(QString(" FROM \'%1\'").arg(Table_name));
    return ReturnString;
}

QString QueryGenerator::DeleteStat(QString Table_name, QSqlRecord delete_rec){
    QString deleteString = QString("DELETE FROM \"%1\"").arg(Table_name);
    QString whereStmt=WhereStat(delete_rec);
    deleteString.append(' ');
    deleteString.append(whereStmt);
    return deleteString;
}

QString QueryGenerator::InsertStat(QString Table_name, QSqlRecord insert_rec){
    QString ReturnString=QString("INSERT INTO \"%1\" ").arg(Table_name);
    QString col="";
    QString val="";
    for(int i=0;i<insert_rec.count();i++){
        col.append(QString(" \"%1\",").arg(insert_rec.fieldName(i)));
        val.append(QString( "\"%1\",").arg(insert_rec.value(i).toString()));
    }
    col.chop(1);
    val.chop(1);
    ReturnString.append(QString("(%1) VALUES (%2)").arg(col,val));
    return ReturnString;
}

QString QueryGenerator::UpdateStat(QString Table_name, QSqlRecord update_rec){
    QString ReturnString=QString("UPDATE \"%1\" SET").arg(Table_name);
    for (int i=0;i<update_rec.count();i++){
        ReturnString.append(QString(" \"%1\" = \"%2\",").arg(update_rec.fieldName(i),update_rec.value(i).toString()));
    }
    ReturnString.chop(1);
    return ReturnString;
}

QString QueryGenerator::WhereStat(QSqlRecord where_rec){
    QString ReturnString = "WHERE";
    for(int i =0;i<where_rec.count();i++){
        ReturnString.append(QString(" \"%1\" LIKE \"%2\" AND").arg(where_rec.fieldName(i),where_rec.value(i).toString()));
    }
    ReturnString.chop(4);
    return ReturnString;
}

QString QueryGenerator::createSelect(QString Table_name, QVector<QVector<QString> > Query_Where){
    QString temp_query;
    temp_query=QString("SELECT * FROM \'%1\' WHERE").arg(Table_name);
    for(int n=0;n<Query_Where.size();n++){
        temp_query.append(QString(" \'%1\' LIKE \'%2\' AND").arg(Query_Where[n][0],Query_Where[n][1]));
    }
    temp_query.chop(4);
    temp_query.append(";");
    return(temp_query);
}

QString QueryGenerator::createDelete(QString Table_name, QVector<QVector<QString> > Query_Where){
    QString temp_query;
    temp_query=QString("DELETE FROM \'%1\' WHERE ").arg(Table_name);
    for(int n=0;n<Query_Where.size();n++){
        temp_query.append(QString(" %1 \'%2\' = \'%3\',").arg(Query_Where[n][2],Query_Where[n][0],Query_Where[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(";");
    return(temp_query);
}

QString QueryGenerator::createInsert(QString Table_name, QVector<QVector<QString> > Query_Insert){
    QString temp_query;
    temp_query=QString("INSERT INTO \'%1\' (").arg(Table_name);
    for (int n=0;n<Query_Insert.size();n++){
        if(!Query_Insert[n][1].isEmpty())
        temp_query.append(QString(" \'%1\',").arg(Query_Insert[n][0]));
    }
    temp_query.chop(1);
    temp_query.append(QString(") VALUES ("));
    for (int n=0;n<Query_Insert.size();n++){
        if(!Query_Insert[n][1].isEmpty())
        temp_query.append(QString(" \'%1\',").arg(Query_Insert[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(");");
    return(temp_query);
}

QString QueryGenerator::createUpdate(QString Table_name, QVector<QVector<QString> > Query_Update, QVector<QVector<QString> > Query_Where){
    QString temp_query;
    temp_query=QString("UPDATE \'%1\' SET").arg(Table_name);
    for(int n=0;n<Query_Update.size();n++){
        temp_query.append(QString( " \'%1\' = \'%2\'").arg(Query_Update[n][0],Query_Update[n][1]));
    }
    temp_query.append(QString(" WHERE"));
    for (int n=0;n<Query_Where.size();n++){
        temp_query.append(QString(" \'%1\' = \'%2\',").arg(Query_Where[n][0],Query_Where[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(";");
    return(temp_query);
}

QString QueryGenerator::createTable(QString Table_name,QVector<QVector<QString> > Fields_values, QVector<QString> Primary, QVector<QString> Unique){
    QString temp_query;
    temp_query=QString("CREATE TABLE \'%1\' (").arg(Table_name);
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

QString QueryGenerator::createTable(QString Table_name, QVector<MyField> fields){
    QString temp_query;
    temp_query=QString("CREATE TABLE \'%1\' (").arg(Table_name);
    QStringList primaryFields;
    QStringList uniqueFields;
    bool primary;
    bool unique;
    foreach(MyField currField, fields){
        primary = false;
        unique = false;
        if (currField.getPrimary()){
            primary = true;
            primaryFields.append(currField.getName());
        }
        if (currField.getUnique()){
            uniqueFields.append(currField.getName());
            unique = true;
        }
        if (primary || unique){
            temp_query.append(QString(" \'%1\' %2 NOT NULL,").arg(currField.getName(),
                                                                  DataType::getSqlType(currField.getType())));
        }else{
            temp_query.append(QString(" \'%1\' %2,").arg(currField.getName(),
                                                              DataType::getSqlType(currField.getType())));
        }
    }
    temp_query.chop(1);
    temp_query.append(", PRIMARY KEY (");
    foreach(QString primaryField, primaryFields){
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
    }

    temp_query.append(");");
    return(temp_query);
}

QString QueryGenerator::createExtTable(QString Table_name, QVector<MyField> fields){
    QString temp_query;
    temp_query=QString("CREATE TABLE \'%1\' (").arg(Table_name);

    foreach(MyField currField, fields){

        if (currField.getPrimary()){
            temp_query.append(QString(" \'%1\' %2 NOT NULL,").arg(currField.getName(),
                                                                  DataType::getSqlType(currField.getType())));
        }
    }
    temp_query.append("\'File\' VARCHAR(50) NOT NULL, \'Description\' TEXT, \'Status\' CHAR");
    //temp_query.chop(1);
    temp_query.append(", PRIMARY KEY (File));");

    return(temp_query);
}

QString QueryGenerator::copyTable(QString origTableName, QString destTableName, QVector<MyField> origFields, QVector<MyField> destFields){
    QString copyTable = QString("INSERT INTO \'%1\' SELECT").arg(destTableName);
    for (int i = 0; i<destFields.count(); i++){
        copyTable.append(QString(" \'%1\',").arg(destFields[i].getName()));
    }
    copyTable.chop(1);
    copyTable.append(") SELECT (");
    for (int i = 0; i<origFields.count(); i++){
        copyTable.append(QString(" \'%1\',").arg(origFields[i].getName()));
    }
    copyTable.chop(1);
    copyTable.append(QString(") FROM \'%1\'").arg(origTableName));
    return copyTable;
}

QString QueryGenerator::copyExtTable(QString origTableName, QString destTableName, QVector<MyField> origFields, QVector<MyField> destFields){

}

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

    QString fieldInsert;
    fieldInsert=QString("INSERT INTO \'Fields_table\' (\'Name\', \'Type\', \'Vis_Preview\', \'Vis_Table\', \'Position\', \'Primary\', \'Comments\') VALUES (");
    fieldInsert.append(QString("\'%1\',").arg(newField.getName()));
    fieldInsert.append(QString(" \'%1\,'").arg((int)(newField.getType())));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisPreview()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisTable()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPos()));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPrimary()?"True":"False"));
    fieldInsert.append(QString(" \'%1\'").arg(newField.getComments()));
    //fieldInsert.append(QString("\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\')").arg(newField.getName(),int(newField.getType()),newField.getVisPreview(),newField.getVisTable(),
                                                                                       // newField.getPos(), newField.getPrimary(), newField.getComments()));

    commands.append(fieldInsert);
    return commands;

}

QString QueryGenerator::insertField(MyField newField){
    QString fieldInsert;
    QString tableName = "Main_table";
    if (newField.getType()==DataType::Images){
        tableName = newField.getName();
    }
    fieldInsert=QString("INSERT INTO \'Fields_table\' (\'Name\', \'Table', \'Type\', \'Vis_Preview\', \'Vis_Table\', \'Position\', \'Primary\', \'Comments\') VALUES (");
    fieldInsert.append(QString("\'%1\',").arg(newField.getName()));
    fieldInsert.append(QString("\'%1\',").arg(tableName));
    fieldInsert.append(QString(" \'%1\',").arg((int)(newField.getType())));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisPreview()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getVisTable()?"True":"False"));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPos()));
    fieldInsert.append(QString(" \'%1\',").arg(newField.getPrimary()?"True":"False"));
    fieldInsert.append(QString(" \'%1\'").arg(newField.getComments()));
    fieldInsert.append(");");
    //fieldInsert.append(QString("\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\')").arg(newField.getName(),int(newField.getType()),newField.getVisPreview(),newField.getVisTable(),
                                                                                       // newField.getPos(), newField.getPrimary(), newField.getComments()));


    return fieldInsert;

}

QString QueryGenerator::editField(FieldEdit fieldChanges){

    QString command;

    command=QString("UPDATE \'Fields_table\' SET Name=\'%1\', ").arg(fieldChanges.getField().getName());
    command.append(QString("Type=\'%1\', ").arg((int)(fieldChanges.getField().getType())));
    command.append(QString("Vis_Preview=\'%1\', ").arg(fieldChanges.getField().getVisPreview()?"True":"False"));
    command.append(QString("Vis_Table=\'%1\', ").arg(fieldChanges.getField().getVisTable()?"True":"False"));
    command.append(QString("Position=\'%1\', ").arg(fieldChanges.getField().getPos()));
    command.append(QString("Primary=\'%1\', ").arg(fieldChanges.getField().getPrimary()?"True":"False"));
    command.append(QString("Comments=\'%1\'").arg(fieldChanges.getField().getComments()));


    command.append(QString(" WHERE Name=\'%1\'").arg(fieldChanges.getOldField().getName()));
    return command;
}

QStringList QueryGenerator::removeField(QString tableName, QVector<MyField> fields, MyField field){
    QStringList commands;
    commands.append("PRAGMA foreign_keys = 0;");
    commands.append(QString("CREATE TABLE temp_table AS SELECT * FROM %1;").arg(tableName));
    commands.append(QString("DROP TABLE %1;").arg(tableName));
    //QVector newFields = fields;
    //newFields.replace(newFields.lastIndexOf(oldField),newField);
    fields.remove(fields.lastIndexOf(field));
    commands.append(createTable(tableName, fields));
    commands.append(copyTable("temp_table",tableName,fields,fields));
    commands.append("DROP TABLE temp_table;");
    commands.append("PRAGMA foreign_keys = 1;");

    QString fieldInsert;
    fieldInsert = QString("DELETE FROM \'Fields_table\' WHERE \'Name\'=\'%1\'").arg(field.getName());
    commands.append(fieldInsert);
    return commands;
}

QStringList QueryGenerator::multipleChanges(QString tableName, QList<FieldEdit*> changes){
    QVector<MyField> oldFields;
    QVector<MyField> newFields;

    QString copyTable = QString("INSERT INTO \'%1\' (").arg(tableName);
    QString origField = QString("");
    QString destField = QString("");
    QStringList deleteCommands;
    QStringList insertCommands;
    foreach (FieldEdit *change, changes){
        oldFields.append(change->getOldField());
        newFields.append(change->getField());

        if(change->getAction()=="Remove"){
            deleteCommands.append(QString("DELETE FROM \'Fields_table\' WHERE \'Name\' = %1;").arg(change->getOldField().getName()));
            if (change->getOldField().getType()==DataType::Images)
                deleteCommands.append(QString("DROP TABLE %1;").arg(change->getOldField().getTable()));
        }else if (change->getAction()=="New"){
            insertCommands.append(insertField(change->getField()));

        }else{
            origField.append(QString(" %1,").arg(change->getOldField().getName()));
            destField.append(QString(" %1,").arg(change->getField().getName()));
        }
    }
    origField.chop(1);
    destField.chop(1);
    copyTable.append(QString(" %1) SELECT %2 FROM \'temp_table\';").arg(destField,origField));

    QStringList commands;
    commands.append("PRAGMA foreign_keys = 0;");
    commands.append(QString("CREATE TABLE temp_table AS SELECT * FROM \'%1\';").arg(tableName));
    commands.append(QString("DROP TABLE %1;").arg(tableName));
    commands.append(createTable(tableName, newFields));
    commands.append(copyTable);
    commands.append("DROP TABLE temp_table;");
    commands.append("PRAGMA foreign_keys = 1;");
    commands.append(deleteCommands);
    commands.append(insertCommands);

    foreach (FieldEdit *change, changes){
        if(change->getAction()=="New" && change->getField().extTable()){
            //Create Table
        }else if(change->getAction()=="Remove" && change->getOldField().extTable()){
            commands.append(QString("DROP TABLE \'%1\';").arg(change->getOldField().getName()));
        }
    }

    return commands;


}



