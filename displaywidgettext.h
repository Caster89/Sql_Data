#pragma once
#ifndef DISPLAYWIDGETTEXT_H
#define DISPLAYWIDGETTEXT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include "displaywidgetbase.h"

class DisplayWidgetText : public DisplayWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidgetText(MyField newField, bool editable=false,QWidget *parent=0);
    QVariant getValue();
    void setValue(QVariant newValue);
    ~DisplayWidgetText();
    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }


private:
    QLineEdit *shortText=new QLineEdit();
    //QLabel *lblTitle = new QLabel();
    //QLabel *lblNonEditTitle=new QLabel();
    QLabel *lblNonEditText=new QLabel();
};

#endif // DISPLAYWIDGETTEXT_H
