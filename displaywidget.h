#pragma once
#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVariant>
#include <QStackedWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <medialistwidget.h>
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <mysqltablemodel.h>
#include <datatype.h>
#include "displaywidgettext.h"
#include <myfield.h>
//#include "metatypedeclaration.h"

class DisplayWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidget(MyField newField, bool Edit=0, QWidget *parent = 0);

    inline void setValue(QVariant newValue){
        displayWidgetPointer->setValue(newValue);
    }

    inline QVariant getValue(){
        return displayWidgetPointer->getValue();
    }

    inline QSizePolicy sizePolicy(){
        return displayWidgetPointer->sizePolicy();
    }

    inline QSize sizeHint() const{
        //qDebug()<<"Display widget recommends a size of: "<< displayWidgetPointer->sizeHint();
        return displayWidgetPointer->sizeHint();
    }

private:
    DisplayWidgetBase *displayWidgetPointer;




};
//Q_DECLARE_METATYPE(QSqlRecord);
//Q_DECLARE_METATYPE(QVector<QVector<QString> >);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
#endif // DISPLAYWIDGET_H

