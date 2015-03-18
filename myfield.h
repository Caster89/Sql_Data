#ifndef MYFIELD_H
#define MYFIELD_H
#include <datatype.h>
#include <QString>

class MyField
{
public:
    //enum Type{"TEXT","LONG_TEXT","IMAGE","IMAGES"};
    MyField();

    MyField(QString Name, QString Table="Main_table", DataType::dataType Field_Type = DataType::Text, bool Vis_Preview = false, bool Vis_Table = false, int Position = 0, bool Primary = false);

    MyField(const MyField &orgField);

    MyField &operator=(const MyField &orgField);




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

#endif // MYFIELD_H

