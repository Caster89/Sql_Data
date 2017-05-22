#pragma once
#ifndef MYFIELD_H
#define MYFIELD_H
#include <datatype.h>
#include <QString>
//#include <QVector>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>

class MyField  : public QSqlRecord
{

public:
    //enum Type{"TEXT","LONG_TEXT","IMAGE","IMAGES"};
    MyField();

    MyField(QString Name, QString Table="Main_table", DataType::dataType Field_Type = DataType::Text, bool Vis_Preview = false, bool Vis_Table = false, int Position = 0, bool Primary = false, bool Unique = false,  QString Comments = "");
    MyField(QSqlRecord newField);
    QSqlField toSqlField();
    /*
    MyField(const MyField &origField);

    MyField(MyField&& origField);

    MyField &operator=(MyField &orgField);
    */
    //MyField &operator=(const MyField &orgField);

    //bool operator==(const MyField& other);
    //bool operator!=(const MyField& other);
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
        return value("Name").toString();
    }
    inline void setName(QString newName) {
        setValue("Name", newName);
    }

    inline QString getTable() const{
        return value("Table").toString();
    }
    inline void setTable(QString newTable) {
        setValue("Table", newTable);
    }

    inline DataType::dataType getType() const{
        return DataType::dataType(value("Type").toInt());
    }
    inline void setType(DataType::dataType newType) {
        setValue("Type", newType);
    }

    inline bool getVisPreview() const{
        return value("Vis_Preview").toBool();
    }
    inline void setVisPreview(bool newPreview) {
        setValue("Vis_Preview", newPreview);
    }

    inline bool getVisTable() const{
        return value("Vis_Table").toBool();
    }
    inline void setVisTable(bool newTable) {
        setValue("Table", newTable);
    }

    inline int getPos() const{
        return value("Position").toInt();
    }
    inline void setPos(int newPos) {
        setValue("Position", newPos);
    }

    inline bool getPrimary() const{
        return value("Primary").toBool();
    }
    inline void setPrimary(bool newPrimary) {
        setValue("Primary", newPrimary);
    }

    inline QString getComments() const{
        return value("Comments").toString();
    }
    inline void setComments(QString newComments) {
        setValue("Comments", newComments);
    }

    inline bool getUnique() const{
        return value("Unique").toBool();
    }
    inline void setUnique(bool newUnique) {
        setValue("Unique", newUnique);
    }

    inline bool extTable() const{
        if (getType() == DataType::Images)
            return true;
        return false;
    }

    inline bool extDir() const{
        if (getType() == DataType::Image || getType() == DataType::Images)
            return true;
        return false;
    }

    friend bool operator< (const MyField &field1, const MyField &field2);
    friend bool operator<= (const MyField &field1, const MyField &field2);
    friend bool operator> (const MyField &field1, const MyField &field2);
    friend bool operator>= (const MyField &field1, const MyField &field2);
    //QSqlRecord toSqlRecord() const;



private:
    /*
    QString Field_Name = "";
    QString Field_Table = "";
    QString Field_Comments = "";
    DataType::dataType Field_Type = DataType::Text;
    bool Field_Vis_Preview = false;
    bool Field_Vis_Table = false;
    int Field_Pos = false;
    bool Field_Primary = false;
    bool Field_Unique = false;*/




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

    inline QString getAction() const{
        return action;
    }

    inline void setField(MyField newField){
        field = newField;
    }

    inline MyField getField() const{
        return field;
    }

    inline void setOldField(MyField newField){
        oldField = newField;
    }
    inline MyField getOldField() const{
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

