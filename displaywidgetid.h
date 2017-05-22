#ifndef DISPLAYWIDGETID_H
#define DISPLAYWIDGETID_H

#include <QObject>
#include <QWidget>
#include "displaywidgetbase.h"

class DisplayWidgetID : public DisplayWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidgetID(MyField newField, bool editable=false,QWidget *parent=0);
    QVariant getValue();
    void setValue(QVariant newValue);
    ~DisplayWidgetID();
    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }


private:
    QLabel *lblID=new QLabel();
    //QLabel *lblTitle = new QLabel();
    //QLabel *lblNonEditTitle=new QLabel();
    QLabel *lblNonEditText=new QLabel();
};

#endif // DISPLAYWIDGETID_H
