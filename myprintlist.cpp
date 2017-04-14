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
    qDebug()<<records[0];
    qDebug()<<'ScreenShots unpacked'<<records[0].value("Screenshots");
    //cmbFields=new QComboBox();
    //for(int i=0;i<avlbFields.size();i++)
        //cmbFields->addItem(Fields[i].getName(),QVariant::fromValue(Fields[i]));

    //setLayout(mainLayout);
    //mainLayout->addWidget(cmbFields,1,1,1,1);
    qDebug()<<"Print Setup";

    /*
    scrollWidget->setLayout(scrollLayout);

    scrollWidget->layout()->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    scrollWidget->setContentsMargins(0,0,0,0);
    scrollWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    this->setWidget(scrollWidget);
    this->setWidgetResizable(true);


    setMinimumWidth(400);
    */


}

void MyPrintList::addPrintItem(MyField newField){
    /*
    printFields.append(newField);
    printItems.append(new PrintItem(printFields.last()));
    qDebug()<<"printItems: "<< printItems.last();
    connect(printItems.last(),SIGNAL(closeWidget()),this, SLOT(removePrintItem()));
    //widgetSignalMapper->setMapping(printItems.last(),printItems.last());
    //connect(widgetSignalMapper, SIGNAL(mapped(PrintItems*)), this, SIGNAL(removePrintItem(QWidget*)));
    scrollLayout->addWidget(printItems.last());
    scrollLayout->addStretch(0);
    scrollArea->update();
    */
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


    //currFrame->setFrameFormat(frameFormat);
    for(int n=0;n<records.count();n++){
        QTextFrame *currFrame=cursor->insertFrame(frameFormat);
        for(int i=0;i<widgetList.count();i++){
            //dynamic_cast<PrintItem*>(widgetList[i])->paintItem(currFrame,&records[n]);
            dynamic_cast<PrintItem*>(widgetList[i])->paintItem(doc,cursor, &records[n]);
            cursor->setPosition(doc->rootFrame()->lastPosition());
            //currFrame=cursor->insertFrame(frameFormat);

        }
    }
    return doc;
}
/*
void MyPrintList::clearPrintItems(){
    for(int i = scrollLayout->count()-1; i>=0;--i){
        scrollLayout->removeItem(scrollLayout->itemAt(i));
    }
}

void MyPrintList::reloadPrintItems(){
    qDebug()<<"Reloading: "<<printItems.count()<<" items";
    for (int i = 0; i<printItems.count();++i){
        scrollLayout->addWidget(printItems[i]);
    }
}
*/
void MyPrintList::removePrintItem(){
    qDebug()<<"test";
    PrintItem* senderItm = qobject_cast<PrintItem*>(QObject::sender());
    removeWidget(senderItm);
}
/*
void MyPrintList::removePrintItem(QWidget *removeWidget){
    qDebug()<<"Removing Item";
    qDebug()<<"There are: "<<printItems.count()<<" items";
    //clearPrintItems();
    int posDelete = printItems.lastIndexOf(removeWidget);
    qDebug()<<"Deliting the one at position: "<<posDelete;
    removeWidget->setParent(NULL);
    printItems.removeAt(posDelete);
    qDebug()<<"There are now: "<< printItems.count()<<" items";
    //reloadPrintItems();
}*/

/*
void MyPrintList::printRecords(){
    QTextDocument *doc=new QTextDocument();
    QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor.currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    QTextFrame *currFrame=cursor.insertFrame(frameFormat);
    for(int n=0;n<records.count();n++){

        for(int i=0;i<printItems.count();i++){
            printItems[i]->paintItem(currFrame,&records[n]);
            cursor.setPosition(topFrame->lastPosition());
            currFrame=cursor.insertFrame(frameFormat);

        }
    }
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    QString fileName("/Users/Nick/Desktop/prova.pdf");
    printer.setOutputFileName(fileName);
    doc->print(&printer); // doc is QTextDocument *
}
*/
MyPrintList::~MyPrintList()
{

}


//Q_DECLARE_METATYPE(MyField);
//Q_DECLARE_METATYPE(QVector<MyField>);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
