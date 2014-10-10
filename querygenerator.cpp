#include "querygenerator.h"
#include <QString>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>

QueryGenerator::QueryGenerator()
{

}


QString QueryGenerator::SelectStat(QString Table_name, QVector<QString> Columns){
    QString ReturnString="SELECT";
    if (Columns.isEmpty()){
        ReturnString="SELECT *";
    }else{
        for(int i=0;i<Columns.size();i++){
            ReturnString.append(QString(" %1,").arg(Columns[i]));
        }
        ReturnString.chop(1);
    }

    ReturnString.append(QString(" FROM \"%1\"").arg(Table_name));
    return ReturnString;
}

QString QueryGenerator::DeleteStat(QString Table_name){
    QString ReturnString = QString("DELETE FROM %1").arg(Table_name);
    return ReturnString;
}

QString QueryGenerator::InsertStat(QString Table_name, QSqlRecord insert_rec){
    QString ReturnString=QString("INSERT INTO %1 ").arg(Table_name);
    QString col="";
    QString val="";
    for(int i=0;i<insert_rec.count();i++){
        col.append(QString(" %1,").arg(insert_rec.fieldName(i)));
        val.append(QString( "%1,").arg(insert_rec.value(i).toString()));
    }
    col.chop(1);
    val.chop(1);
    ReturnString.append(QString("(%1) VALUES (%2)").arg(col,val));
    return ReturnString;
}

QString QueryGenerator::UpdateStat(QString Table_name, QSqlRecord update_rec){
    QString ReturnString=QString("UPDATE %1 SET").arg(Table_name);
    for (int i=0;i<update_rec.count();i++){
        ReturnString.append(QString(" %1 =\" %2 \",").arg(update_rec.fieldName(i),update_rec.value(i).toString()));
    }
    ReturnString.chop(1);
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
    temp_query=QString("SELECT * FROM %1 WHERE").arg(Table_name);
    for(int n=0;n<Query_Where.size();n++){
        temp_query.append(QString(" \"%1\" LIKE \"%2\" AND").arg(Query_Where[n][0],Query_Where[n][1]));
    }
    temp_query.chop(4);
    temp_query.append(";");
    return(temp_query);
}



QString QueryGenerator::createDelete(QString Table_name, QVector<QVector<QString> > Query_Where){
    QString temp_query;
    temp_query=QString("DELETE FROM %1 WHERE ").arg(Table_name);
    for(int n=0;n<Query_Where.size();n++){
        temp_query.append(QString(" %1 \"%2\" = \"%3\",").arg(Query_Where[n][2],Query_Where[n][0],Query_Where[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(";");
    return(temp_query);
}



QString QueryGenerator::createInsert(QString Table_name, QVector<QVector<QString> > Query_Insert){
    QString temp_query;
    temp_query=QString("INSERT INTO %1 (").arg(Table_name);
    for (int n=0;n<Query_Insert.size();n++){
        if(!Query_Insert[n][1].isEmpty())
        temp_query.append(QString(" \"%1\",").arg(Query_Insert[n][0]));
    }
    temp_query.chop(1);
    temp_query.append(QString(") VALUES ("));
    for (int n=0;n<Query_Insert.size();n++){
        if(!Query_Insert[n][1].isEmpty())
        temp_query.append(QString(" \"%1\",").arg(Query_Insert[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(");");
    return(temp_query);
}


QString QueryGenerator::createUpdate(QString Table_name, QVector<QVector<QString> > Query_Update, QVector<QVector<QString> > Query_Where){
    QString temp_query;
    temp_query=QString("UPDATE %1 SET").arg(Table_name);
    for(int n=0;n<Query_Update.size();n++){
        temp_query.append(QString( " \"%1\" = \"%2\"").arg(Query_Update[n][0],Query_Update[n][1]));
    }
    temp_query.append(QString(" WHERE"));
    for (int n=0;n<Query_Where.size();n++){
        temp_query.append(QString(" \"%1\" = \"%2\",").arg(Query_Where[n][0],Query_Where[n][1]));
    }
    temp_query.chop(1);
    temp_query.append(";");
    return(temp_query);
}

QString QueryGenerator::createTable(QString Table_name,QVector<QVector<QString> > Fields_values, QVector<QString> Primary, QVector<QString> Unique){
    QString temp_query;
    temp_query=QString("CREATE TABLE %1 (").arg(Table_name);
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



