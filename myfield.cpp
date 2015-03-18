#include "myfield.h"
#include <mysqltablemodel.h>
#include <QString>


MyField::MyField(){

}

MyField::MyField(QString Name, QString Table, DataType::dataType Type, bool Vis_Preview, bool Vis_Table, int Position, bool Primary)
{
    Field_Name=Name;
    Field_Table=Table;
    Field_Type=Type;
    Field_Vis_Preview=Vis_Preview;
    Field_Vis_Table=Vis_Table;
    Field_Pos=Position;
    Field_Primary=Primary;
}

MyField::MyField(const MyField &orgField){
    Field_Name=orgField.getName();
    Field_Table=orgField.getTable();
    Field_Type=orgField.getType();
    Field_Vis_Preview=orgField.getVisPreview();
    Field_Vis_Table=orgField.getVisTable();
    Field_Pos=orgField.getPos();
    Field_Primary=orgField.getPrimary();
}

MyField& MyField::operator=( const MyField &orgField){
    MyField newField(orgField.getName(),orgField.getTable(),orgField.getType(),orgField.getVisPreview(),orgField.getVisTable(),orgField.getPos(),orgField.getPrimary());
    return newField;
}
