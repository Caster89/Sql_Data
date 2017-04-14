#pragma once
#ifndef DISPLAYWIDGETIMAGE_H
#define DISPLAYWIDGETIMAGE_H


#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSqlRecord>
#include "displaywidgetbase.h"
#include "metatypedeclaration.h"


class DisplayWidgetImage : public DisplayWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidgetImage(MyField newField, bool editable=false,QWidget *parent=0);
    ~DisplayWidgetImage();    
    QVariant getValue();
    void setValue(QVariant newValue);
    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }

private:
    //QLineEdit *shortText=new QLineEdit();
    //QLabel *lblTitle = new QLabel();
    //QLabel *lblNonEditTitle=new QLabel();
    QLabel *lblNonEditImage=new QLabel();
    QLabel *lblEditImage=new QLabel();
    QPushButton *btnSelectImg = new QPushButton("Select Image");
    QPushButton *btnRemoveImg = new QPushButton("Remove Image");
    QSqlRecord value;
    QPixmap noImage;
    void updateImage();

private slots:
    void selectImage();
    void removeImage();
};
//Q_DECLARE_METATYPE(QSqlRecord);

#endif // DISPLAYWIDGETIMAGE_H
