#ifndef DISPLAYWIDGETLONGTEXT_H
#define DISPLAYWIDGETLONGTEXT_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include "displaywidgetbase.h"

class DisplayWidgetLongText : public DisplayWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidgetLongText(MyField newField, bool editable=false, QWidget *parent=0);
    QVariant getValue();
    void setValue(QVariant newValue);
    ~DisplayWidgetLongText();
    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

private:
    QTextEdit *longText=new QTextEdit();
    //QLabel *lblTitle = new QLabel();
    //QLabel *lblNonEditTitle=new QLabel();
    QLabel *lblNonEditText=new QLabel();
};

#endif // DISPLAYWIDGETLONGTEXT_H
