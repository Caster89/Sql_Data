#ifndef PRINTSETUP_H
#define PRINTSETUP_H

#include <QWidget>
#include <QVector>
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


class PrintSetup : public QWidget
{
    Q_OBJECT
public:
    explicit PrintSetup(QVector<MyField> avlbFields, QWidget *parent = 0);
    ~PrintSetup();

signals:

public slots:

private:
    //QToolBox *mainTools = new QToolBox();
    QStringList fieldsNames;
    QVector<MyField> Fields;
    QVector<MyField> printFields;
    QVector<PrintItem*> printItems;
    QComboBox *cmbFields;
    QGridLayout *mainLayout=new QGridLayout(this);
    QScrollArea *scrollArea=new QScrollArea();
    QWidget *scrollWidget =new QWidget();
    QVBoxLayout *scrollLayout=new QVBoxLayout();
    QPushButton *btnAddField = new QPushButton("Add Field");

private slots:
    void addField();

};

#endif // PRINTSETUP_H
