#pragma once
#ifndef MYFIELD_H
#define MYFIELD_H
#include <datatype.h>
#include <QString>
//#include <QVector>
#include <QVariant>

class MyField
{
public:
    //enum Type{"TEXT","LONG_TEXT","IMAGE","IMAGES"};
    MyField();

    MyField(QString Name, QString Table="Main_table", DataType::dataType Field_Type = DataType::Text, bool Vis_Preview = false, bool Vis_Table = false, int Position = 0, bool Primary = false);

    MyField(const MyField &orgField);

    MyField &operator=(MyField &orgField);

    MyField &operator=(const MyField &orgField);


    friend void swap(MyField& first,MyField& second){
        using std::swap;
        swap(first.Field_Name,second.Field_Name);
        swap(first.Field_Pos,second.Field_Pos);
        swap(first.Field_Primary,second.Field_Primary);
        swap(first.Field_Table,second.Field_Table);
        swap(first.Field_Type,second.Field_Type);
        swap(first.Field_Vis_Preview,second.Field_Vis_Preview);
        swap(first.Field_Table,second.Field_Table);
    }






    inline QString getName() const{
        return Field_Name;
    }
    inline QString getTable() const{
        return Field_Table;
    }
    inline DataType::dataType getType() const{
        return Field_Type;
    }
    inline bool getVisPreview() const{
        return Field_Vis_Preview;
    }
    inline bool getVisTable() const{
        return Field_Vis_Table;
    }
    inline int getPos() const{
        return Field_Pos;
    }
    inline bool getPrimary() const{
        return Field_Primary;
    }

private:
    QString Field_Name;
    QString Field_Table;
    DataType::dataType Field_Type;
    bool Field_Vis_Preview;
    bool Field_Vis_Table;
    int Field_Pos;
    bool Field_Primary;




};
Q_DECLARE_METATYPE(QVector<MyField>);
Q_DECLARE_METATYPE(MyField);

#endif // MYFIELD_H

