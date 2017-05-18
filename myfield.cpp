#include "myfield.h"
#include <mysqltablemodel.h>
#include <QString>
#include <QSqlField>



MyField::MyField(){
    Field_Table= "Main_table";
}

MyField::MyField(QString Name, QString Table, DataType::dataType Type, bool Vis_Preview, bool Vis_Table, int Position, bool Primary, bool Unique, QString Comments)
{
    Field_Name = Name;
    Field_Table = Table;
    Field_Type = Type;
    Field_Vis_Preview = Vis_Preview;
    Field_Vis_Table = Vis_Table;
    Field_Pos = Position;
    Field_Primary = Primary;
    Field_Comments = Comments;
}

QSqlRecord MyField::toSqlRecord() const{
    QSqlRecord returnRec;
    returnRec.append(QSqlField("Name",QVariant::String));
    returnRec.setValue("Name",getName());
    returnRec.append(QSqlField("Type",QVariant::Int));
    returnRec.setValue("Type",(int)(getType()));
    returnRec.append(QSqlField("Table",QVariant::String));
    returnRec.setValue("Table",getTable());
    returnRec.append(QSqlField("Vis_Preview",QVariant::Bool));
    returnRec.setValue("Vis_Preview",getVisPreview());
    returnRec.append(QSqlField("Vis_Table",QVariant::Bool));
    returnRec.setValue("Vis_Table",getVisTable());
    returnRec.append(QSqlField("Position",QVariant::Int));
    returnRec.setValue("Position",getPos());
    returnRec.append(QSqlField("Primary",QVariant::Bool));
    returnRec.setValue("Primary",getPrimary());
    returnRec.append(QSqlField("Comments",QVariant::String));
    returnRec.setValue("Comments",getComments());
    return returnRec;
}

QSqlRecord MyField::primaryRecord(){
    QSqlRecord returnRec;
    returnRec.append(QSqlField("Name",QVariant::String));
    returnRec.setValue("Name",getName());
    return returnRec;
}

bool MyField::operator==(const MyField& other) {

    return (Field_Name==other.getName() && Field_Table==other.getTable() && Field_Type==other.getType() &&
          Field_Vis_Preview==other.getVisPreview() && Field_Vis_Table==other.getVisTable() &&
          Field_Pos==other.getPos() && Field_Primary==other.getPrimary() && Field_Comments==other.getComments());
}

bool MyField::operator !=(const MyField& other) {
    return !(*this==other);
}

//---------------FIELD EDIT--------------------//
bool FieldEdit::mainEdit(){
    if (action == "New" || action =="Remove" ||
            field.getName() != oldField.getName() || field.getPrimary()!=oldField.getPrimary() ||
            field.getUnique() != oldField.getUnique())
        return true;
    return false;
}


void FieldEdit::setName(QString newName){
    field.setName(newName);
    if (newName==DataType::Image || newName==DataType::Images){
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
