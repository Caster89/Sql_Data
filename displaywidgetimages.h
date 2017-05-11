#pragma once
#ifndef DISPLAYWIDGETIMAGES_H
#define DISPLAYWIDGETIMAGES_H
#include <QPushButton>
#include <QLabel>
#include "displaywidgetbase.h"
#include <medialistwidget.h>
#include <QSqlRecord>


class DisplayWidgetImages : public DisplayWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidgetImages(MyField newField, bool editable=false,QWidget *parent=0);
    ~DisplayWidgetImages();
    QVariant getValue();
    void setValue(QVariant newValue);
    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    }

    inline QSize sizeHint() const{
        QSize widgetSize = ImagesDisplay->sizeHint();
        widgetSize.scale(widgetSize.width()*1,widgetSize.height()*4,Qt::IgnoreAspectRatio);
    qDebug()<<"The reccomended size is: "<<widgetSize;
        return widgetSize;
    }

private:

    QLabel *lblNonEditImage=new QLabel();
    QLabel *lblEditImage=new QLabel();
    MediaListWidget *ImagesDisplay;
    MediaListWidget *ImagesEdit;
    QPushButton *AddImages = new QPushButton("Add Images");
    QPushButton *RemoveImages= new QPushButton("Remove Images");
    QVector<QSqlRecord> value;


    void updateImages();

    void retrieveValue();

private slots:
    void addImages();
    void removeImages();
    void imagesChanged();
};

//Q_DECLARE_METATYPE(QVector<QSqlRecord>);

#endif // DISPLAYWIDGETIMAGES_H
