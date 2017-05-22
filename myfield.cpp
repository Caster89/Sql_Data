#include "myfield.h"
#include <mysqltablemodel.h>
#include <QString>
#include <QSqlField>



MyField::MyField() : QSqlRecord()
{
    append(QSqlField("Name", QVariant::String));
    append(QSqlField("Table", QVariant::String));
    append(QSqlField("Type",QVariant::Int));
    append(QSqlField("Vis_Preview", QVariant::Bool));
    append(QSqlField("Vis_Table", QVariant::Bool));
    append(QSqlField("Position", QVariant::Int));
    append(QSqlField("Primary", QVariant::Bool));
    append(QSqlField("Comments", QVariant::String));
    append(QSqlField("Unique", QVariant::Bool));
    setValue("Table","Main_table");
}

MyField::MyField(QString Name, QString Table, DataType::dataType Type, bool Vis_Preview, bool Vis_Table, int Position, bool Primary, bool Unique, QString Comments) :
    MyField()
{
    setValue("Name",Name);
    setValue("Table",Table);
    setValue("Type",Type);
    setValue("Vis_Preview",Vis_Preview);
    setValue("Vis_Table",Vis_Table);
    setValue("Position",Position);
    setValue("Primary",Primary);
    setValue("Comments",Comments);
    setValue("Unique", Unique);
    /*
    Field_Name = Name;
    Field_Table = Table;
    Field_Type = Type;
    Field_Vis_Preview = Vis_Preview;
    Field_Vis_Table = Vis_Table;
    Field_Pos = Position;
    Field_Primary = Primary;
    Field_Comments = Comments;*/
}

MyField::MyField(QSqlRecord newField) : MyField(){
    //qDebug()<<"MyField::MyField() creating a field from sqlRecord: "<<newField;
    if (!(newField.contains("Name")&&newField.contains("Table")&&newField.contains("Type")&&
          newField.contains("Vis_Preview")&&newField.contains("Vis_Table")&&newField.contains("Position")&&
          newField.contains("Primary")&&newField.contains("Unique")&&newField.contains("Comments")))
    {
        return;
    }
    setValue("Name",newField.value("Name").toString());
    setValue("Table",newField.value("Table").toString());
    setValue("Type",newField.value("Type").toInt());
    setValue("Vis_Preview",newField.value("Vis_Preview").toBool());
    setValue("Vis_Table",newField.value("Vis_Table").toBool());
    setValue("Position",newField.value("Position").toInt());
    setValue("Primary",newField.value("Primary").toBool());
    setValue("Comments",newField.value("Comments").toString());
    setValue("Unique", newField.value("Unique").toBool());
}

QSqlField MyField::toSqlField(){
    QSqlField returnField;
    returnField.setName(value("Name").toString());
    return returnField;
}



QSqlRecord MyField::primaryRecord(){
    QSqlRecord returnRec;
    returnRec.append(QSqlField("Name",QVariant::String));
    returnRec.setValue("Name",getName());
    return returnRec;
}

bool operator< (const MyField &field1, const MyField &field2)
{
    return (field1.getPos()<field2.getPos());
}
bool operator<= (const MyField &field1, const MyField &field2)
{
    return (field1.getPos()<=field2.getPos());
}

bool operator> (const MyField &field1, const MyField &field2)
{
    return (field1.getPos()>field2.getPos());
}
bool operator>= (const MyField &field1, const MyField &field2)
{
    return (field1.getPos()>=field2.getPos());
}

/*
bool MyField::operator==(const MyField& other) {

    return (Field_Name==other.getName() && Field_Table==other.getTable() && Field_Type==other.getType() &&
          Field_Vis_Preview==other.getVisPreview() && Field_Vis_Table==other.getVisTable() &&
          Field_Pos==other.getPos() && Field_Primary==other.getPrimary() && Field_Comments==other.getComments());
}

bool MyField::operator !=(const MyField& other) {
    return !(*this==other);
}*/

//---------------FIELD EDIT--------------------//
bool FieldEdit::mainEdit(){
    if (action == "New" || action =="Remove" ||
            field.getName() != oldField.getName() ||
            field.getUnique() != oldField.getUnique() || field.getPos()!=oldField.getPos())
        return true;
    return false;
}


void FieldEdit::setName(QString newName){
    field.setName(newName);
    if (field.extTable()){
        field.setTable(field.getName());
    }
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setType(DataType::dataType newType){
    field.setType(newType);
    if (newType==DataType::Image || newType==DataType::Images){
        field.setTable(field.getName());
    }
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setTable(QString newTable){
    field.setTable(newTable);
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setPreview(bool newPreview){
    field.setVisPreview(newPreview);
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setVisTable(bool newVisTable){
    field.setVisTable(newVisTable);
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setPrimary(bool newPrimary){
    field.setPrimary(newPrimary);
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setUnique(bool newUnique){
    field.setUnique(newUnique);
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}

void FieldEdit::setPos(int newPos){
    field.setPos(newPos);
    if (action == "Remove" || action == "New")
        return;
    if (oldField==field){
        action = "";
    }else{
        action = "Edit";
    }
}
