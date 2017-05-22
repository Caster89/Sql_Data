#pragma once
#ifndef DATATYPE_H
#define DATATYPE_H
#include <QMap>
#include <QObject>

struct DataType : public QObject{
    Q_OBJECT
    Q_ENUMS(dataType)
public:
    enum dataType{
        Text,
        LongText,
        Image,
        Images,
        Static,
        Combined,
        MultipleChoice,
        ID,
        Other,
    };

    QMap<dataType,QString> dataTypeMap{{dataType::Text,"Text"},
                                   {dataType::LongText, "Long Text"},
                                   {dataType::Image, "Image"},
                                   {dataType::Images, "Images"},
                                   {dataType::Static, "Static"},
                                   {dataType::Combined, "Combined"},
                                   {dataType::MultipleChoice, "Multiple Choice"},
                                   {DataType::Other, "Other"}};
    QMap<dataType,QString> sqlType{{dataType::Text,"VARCHAR (50)"},
                                   {dataType::LongText, "TEXT"},
                                   {dataType::Image, "TEXT"},
                                   {dataType::Images, "TEXT"},
                                   {dataType::Static, "TEXT"},
                                   {dataType::Combined, "TEXT"},
                                   {dataType::MultipleChoice, "TEXT"},
                                   {DataType::Other, "BLOB"}};
    static QString getSqlType(dataType type){
        switch(type){
            case Text:
                return "VARCHAR (50)";
            case LongText:
                return "TEXT";
            case Image:
                return "TEXT";
            case Images:
                return "TEXT";
            case Static:
                return "TEXT";
            case Combined:
                return "TEXT";
            case MultipleChoice:
                return "TEXT";
            case Other:
                return "BLOBL";
            default:
                return "TEXT";
        }
    }

};
Q_DECLARE_METATYPE(DataType::dataType);
#endif // DATATYPE_H
