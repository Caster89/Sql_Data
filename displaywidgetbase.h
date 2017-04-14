#pragma once
#ifndef DISPLAYWIDGETBASE_H
#define DISPLAYWIDGETBASE_H

#include <QWidget>
#include <QStackedWidget>
#include <QVariant>
#include <QLabel>
#include <QSqlRecord>
#include <QDebug>
//#include "displaywidget.h"
#include "myfield.h"
#include "metatypedeclaration.h"

class DisplayWidgetBase : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
    //friend class DisplayWidget;
public:
    explicit DisplayWidgetBase(MyField newField, bool editable, QWidget *parent = 0);
    virtual QVariant getValue()=0;
    virtual void setValue(QVariant newValue)=0;
    ~DisplayWidgetBase();
    virtual QSizePolicy sizePolicy()=0;

protected:
    MyField field;
    QLabel *editTitle = new QLabel();
    QLabel *nonEditTitle = new QLabel();
    QWidget *editWidget=new QWidget(this);
    QWidget *nonEditWidget=new QWidget(this);
    enum position{
        Editable=0,
        NonEditable
    };

};

//Q_DECLARE_METATYPE(QSqlRecord);
//Q_DECLARE_METATYPE(QVector<QVector<QString> >);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
//Q_DECLARE_METATYPE(QSqlRecord);
#endif // DISPLAYWIDGETBASE_H
