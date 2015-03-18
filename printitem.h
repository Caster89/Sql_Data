#ifndef PRINTITEM_H
#define PRINTITEM_H

#include <QObject>
#include "printitembase.h"
#include <QStackedWidget>

#include <QToolBox>
#include "mystackedwidget.h"


class PrintItem : public QObject
{
    Q_OBJECT
public:
    explicit PrintItem(MyField newField, QObject *parent = 0);

    inline QFrame* getWidget(){
        return printItemPointer;
    }

    ~PrintItem(){}


signals:

public slots:

private:
    PrintItemBase *printItemPointer=0;

};

#endif // PRINTITEM_H
