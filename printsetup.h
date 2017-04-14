#pragma once
#ifndef PRINTSETUP_H
#define PRINTSETUP_H

#include <QWidget>
#include <QVector>
#include <QLabel>
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
#include <QPrintPreviewWidget>
#include <QPrinter>
#include <QMap>

#include "myprintlist.h"
class PrintSetup : public QWidget
{
    Q_OBJECT
public:
    explicit PrintSetup(QVector<MyField> avlbFields,  QVector<QSqlRecord> printRecords,QWidget *parent = 0);
    ~PrintSetup();

signals:

public slots:

private:
    //QToolBox *mainTools = new QToolBox();
    QStringList fieldsNames;
    QVector<MyField> Fields;
    QVector<QSqlRecord> records;
    QComboBox *cmbFields;
    QLabel *lblPageSize = new QLabel("Page Format:");
    QLabel *lblResolution = new QLabel("Resolution");
    QComboBox *cmbPageSize = new QComboBox();
    QComboBox *cmbResolution = new QComboBox();
    QGridLayout *pageSetupLayout = new QGridLayout();
    QMap <QString, QPrinter::PaperSize> *avlbPaperSize = new QMap <QString, QPrinter::PaperSize>;
    //QMap <QString, int> avlbResolution;

    QGridLayout *mainLayout=new QGridLayout();
    QScrollArea *scrollArea=new QScrollArea();
    MyPrintList *printItemList;
    QPrintPreviewWidget *printPreview;
    //MyPrintList *printItemList;
    QWidget *scrollWidget =new QWidget();
    QVBoxLayout *scrollLayout=new QVBoxLayout();
    QPushButton *btnAddField = new QPushButton("Add Field");
    //QListWidget *printItemList = new QListWidget(this);

    QPrinter *savePrinter = new QPrinter();
    QPrinter *previewPrinter = new QPrinter();

    void populateSetup();
private slots:
    void addField();
    void printRecords();
    void previewPrint(QPrinter *printer);
    void printUpdated();
    void pageSetupUpdated();

};
//Q_DECLARE_METATYPE(MyField);
//Q_DECLARE_METATYPE(QVector<MyField>);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
#endif // PRINTSETUP_H
