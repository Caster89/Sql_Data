#pragma once
#ifndef MYPRINTLIST_H
#define MYPRINTLIST_H

#include <QWidget>
#include <QVector>
#include <QSqlRecord>
#include "myfield.h"
#include "printitem.h"
#include <QStringList>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QDialog>
#include <QToolBox>
#include "metatypedeclaration.h"
#include <QListWidget>
#include <QScrollArea>
#include <QSignalMapper>
//#include "myprintlist.h"
#include "dragscrollarea.h"
#include <QTextDocument>
#include <QPrinter>
#include <QPdfWriter>

class MyPrintList : public DragScrollArea
        //QScrollArea
{
    Q_OBJECT
public:
    explicit MyPrintList(QVector<QSqlRecord> newRecords, QWidget *parent = 0);
    ~MyPrintList();

signals:
    //void itemAdded();

private:
    //QToolBox *mainTools = new QToolBox();
    QStringList fieldsNames;

    QList<MyField> printFields;
    //QList<QWidget*> printItems;

    QVector<QSqlRecord> records;

    //QVBoxLayout *mainLayout=new QVBoxLayout(this);
    //QScrollArea *scrollArea=new QScrollArea();

    //QWidget *scrollWidget =new QWidget();
    //QVBoxLayout *scrollLayout=new QVBoxLayout();
    //QPushButton *btnAddField = new QPushButton("Add Field");

    //QSignalMapper *widgetSignalMapper = new QSignalMapper();


public slots:
    void addPrintItem(MyField newField);
    //void removePrintItem(QWidget *removeWidget);
    void removePrintItem();
    QTextDocument* printRecords(QPrinter *printer);
    QTextDocument* printRecords(QPdfWriter *printer);


private:
    //void clearPrintItems();
    //void reloadPrintItems();

};
//Q_DECLARE_METATYPE(MyField);
//Q_DECLARE_METATYPE(QVector<MyField>);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
#endif // MYPRINTLIST_H
