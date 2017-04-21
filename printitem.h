#pragma once
#ifndef PRINTITEM_H
#define PRINTITEM_H

#include <QObject>
#include "printitembase.h"
#include <QStackedWidget>
#include <QVector>
#include <QToolBox>
#include "mystackedwidget.h"
#include <QTextDocument>
#include "draggablewidget.h"
//#include "metatypedeclaration.h"


class PrintItem : public QWidget
{
    Q_OBJECT
public:
    PrintItem(MyField newField, QWidget *parent = 0);

    inline QFrame* getWidget(){
        return printItemPointer;
    }
    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
        printItemPointer->paintItem(doc, cursor, record);
    }

    ~PrintItem(){}


signals:
    void closeWidget();
    void itemModified();

public slots:

private:
    PrintItemBase *printItemPointer=0;

};
//Q_DECLARE_METATYPE(PrintItem);
//Q_DECLARE_METATYPE(QVector<PrintItem>);

#endif // PRINTITEM_H
