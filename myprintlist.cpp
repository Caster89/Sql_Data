#include "myprintlist.h"
#include <QScrollArea>
#include "printitem.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QListWidgetItem>
#include <QAbstractTextDocumentLayout>



MyPrintList::MyPrintList(QVector<QSqlRecord> newRecords, QWidget *parent) : DragScrollArea(parent)
    //QScrollArea(parent)
{
    records=newRecords;

}

void MyPrintList::addPrintItem(MyField newField){

    printFields.append(newField);
    addWidget(new PrintItem(printFields.last()));
    connect(widgetList.last(),SIGNAL(closeWidget()),this, SLOT(removePrintItem()));
    connect(widgetList.last(),SIGNAL(itemModified()), this, SIGNAL(itemModified()));
    //emit itemAdded();


}

QTextDocument *MyPrintList::printRecords(QPrinter* printer){
    QTextDocument *doc=new QTextDocument();
    doc->documentLayout()->setPaintDevice(printer);
    doc->setPageSize(QSizeF(printer->pageRect().size()));

    QTextCursor *cursor = new QTextCursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=doc->rootFrame()->frameFormat();


    for(int n=0;n<records.count();n++){
        //QTextFrame *currFrame=cursor->insertFrame(frameFormat);
        for(int i=0;i<widgetList.count();i++){
            dynamic_cast<PrintItem*>(widgetList[i])->paintItem(doc,cursor, &records[n]);
            cursor->setPosition(doc->rootFrame()->lastPosition());

        }
    }
    return doc;
}

QTextDocument *MyPrintList::printRecords(QPdfWriter* printer){
    QTextDocument *doc=new QTextDocument();

    //doc->setPageSize();
    int pageType = printer->pageSize();
    QPageSize pageSize((QPageSize::PageSizeId)(pageType));
    qDebug()<<"============================"<<pageSize.sizePixels(printer->resolution());
    qDebug()<<"Printer resolution: "<<printer->resolution();
    doc->setPageSize(pageSize.sizePixels(printer->resolution()));
    doc->documentLayout()->setPaintDevice(printer);
    //doc->setPageSize(printer->pageSize());
    qDebug()<<"=================";
    qDebug()<<"Document Dimensions: "<<doc->pageSize();

    QTextCursor *cursor = new QTextCursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=doc->rootFrame()->frameFormat();


    for(int n=0;n<records.count();n++){
        //QTextFrame *currFrame=cursor->insertFrame(frameFormat);
        for(int i=0;i<widgetList.count();i++){
            dynamic_cast<PrintItem*>(widgetList[i])->paintItem(doc,cursor, &records[n]);
            cursor->setPosition(doc->rootFrame()->lastPosition());

        }
    }
    return doc;
}

void MyPrintList::removePrintItem(){
    qDebug()<<"test";
    PrintItem* senderItm = qobject_cast<PrintItem*>(QObject::sender());
    removeWidget(senderItm);
}

MyPrintList::~MyPrintList()
{

}


//Q_DECLARE_METATYPE(MyField);
//Q_DECLARE_METATYPE(QVector<MyField>);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
