#include "myfield.h"
#include <mysqltablemodel.h>
#include <QString>


MyField::MyField(){

}

MyField::MyField(QString Name, QString Table, DataType::dataType Type, bool Vis_Preview, bool Vis_Table, int Position, bool Primary, QString Comments)
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

MyField::MyField(const MyField &origField){
    Field_Name=origField.getName();
    Field_Table=origField.getTable();
    Field_Type=origField.getType();
    Field_Vis_Preview=origField.getVisPreview();
    Field_Vis_Table=origField.getVisTable();
    Field_Pos=origField.getPos();
    Field_Primary=origField.getPrimary();
    Field_Comments = origField.getComments();
}

MyField& MyField::operator=( MyField &orgField){


    swap(*this,orgField);

    return *this;
}

MyField& MyField::operator=( const MyField &orgField){

    MyField temp(orgField);
    swap(*this,temp);
    //MyField newField(orgField);
    qDebug()<<"Creating Field from :"<<orgField.getName();
    qDebug()<<"Created: "<<this->getName();
    return *this;
}
