#pragma once
#ifndef MYFIELD_H
#define MYFIELD_H
#include <datatype.h>
#include <QString>
//#include <QVector>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>

class MyField
{

public:
    //enum Type{"TEXT","LONG_TEXT","IMAGE","IMAGES"};
    MyField();

    MyField(QString Name, QString Table="Main_table", DataType::dataType Field_Type = DataType::Text, bool Vis_Preview = false, bool Vis_Table = false, int Position = 0, bool Primary = false, bool Unique = false,  QString Comments = "");
    /*
    MyField(const MyField &origField);

    MyField(MyField&& origField);

    MyField &operator=(MyField &orgField);
    */
    //MyField &operator=(const MyField &orgField);

    bool operator==(const MyField& other);
    bool operator!=(const MyField& other);
    /*
    friend inline void swap(MyField& first,MyField& second){
        using std::swap;
        swap(first.Field_Name,second.Field_Name);
        swap(first.Field_Pos,second.Field_Pos);
        swap(first.Field_Primary,second.Field_Primary);
        swap(first.Field_Table,second.Field_Table);
        swap(first.Field_Type,second.Field_Type);
        swap(first.Field_Vis_Preview,second.Field_Vis_Preview);
        swap(first.Field_Table,second.Field_Table);
        swap(first.Field_Comments, second.Field_Comments);
    }
    */




    QSqlRecord primaryRecord();

    inline QString getName() const{
        return Field_Name;
    }
    inline void setName(QString newName) {
        Field_Name = newName;
    }

    inline QString getTable() const{
        return Field_Table;
    }
    inline void setTable(QString newTable) {
        Field_Table=newTable;
    }

    inline DataType::dataType getType() const{
        return Field_Type;
    }
    inline void setType(DataType::dataType newType) {
        Field_Type = newType;
    }

    inline bool getVisPreview() const{
        return Field_Vis_Preview;
    }
    inline void setVisPreview(bool newPreview) {
        Field_Vis_Preview = newPreview;
    }

    inline bool getVisTable() const{
        return Field_Vis_Table;
    }
    inline void setVisTable(bool newTable) {
        Field_Vis_Table = newTable;
    }

    inline int getPos() const{
        return Field_Pos;
    }
    inline void setPos(int newPos) {
        Field_Pos = newPos;
    }

    inline bool getPrimary() const{
        return Field_Primary;
    }
    inline void setPrimary(bool newPrimary) {
        Field_Primary = newPrimary;
    }

    inline QString getComments() const{
        return Field_Comments;
    }
    inline void setComments(QString newComments) {
        Field_Comments = newComments;
    }

    inline bool getUnique() const{
        return Field_Unique;
    }
    inline void setUnique(bool newUnique) {
        Field_Unique = newUnique;
    }

    inline bool extTable() const{
        if (Field_Type == DataType::Images)
            return true;
        return false;
    }

    inline bool extDir() const{
        if (Field_Type == DataType::Image ||Field_Type == DataType::Images)
            return true;
        return false;
    }

    QSqlRecord toSqlRecord() const;



private:
    QString Field_Name = "";
    QString Field_Table = "";
    QString Field_Comments = "";
    DataType::dataType Field_Type = DataType::Text;
    bool Field_Vis_Preview = false;
    bool Field_Vis_Table = false;
    int Field_Pos = false;
    bool Field_Primary = false;
    bool Field_Unique = false;




};

class FieldEdit{

public:
    inline FieldEdit(){

    }

    ~FieldEdit(){
    }

    inline FieldEdit(QString act, MyField origField){
        action = act;
        field = origField;
        oldField = origField;
    }

    inline FieldEdit(QString act, MyField edtField, MyField origField){
        action = act;
        field = edtField;
        oldField = origField;
    }

    inline void setAction(QString newAction){
        action = newAction;
    }

    inline QString getAction(){
        return action;
    }

    inline void setField(MyField newField){
        field = newField;
    }

    inline MyField getField(){
        return field;
    }

    inline void setOldField(MyField newField){
        oldField = newField;
    }
    inline MyField getOldField(){
        return oldField;
    }

    QString Field_Name = "";
    QString Field_Table = "";
    QString Field_Comments = "";
    DataType::dataType Field_Type = DataType::Text;
    bool Field_Vis_Preview = false;
    bool Field_Vis_Table = false;
    int Field_Pos = -1;
    bool Field_Primary = false;
    bool Field_Unique =false;

    void setName(QString newName);
    void setType(DataType::dataType newType);
    void setTable(QString newTable);
    void setPreview(bool newPreview);
    void setVisTable(bool newVisTable);
    void setPrimary(bool newPrimary);
    void setUnique(bool newUnique);
    void setPos(int newPos);

    bool mainEdit();

private:
    QString action;
    MyField field;
    MyField oldField;
};

Q_DECLARE_METATYPE(QVector<MyField>);
Q_DECLARE_METATYPE(MyField);
Q_DECLARE_METATYPE(FieldEdit)

#endif // MYFIELD_H

