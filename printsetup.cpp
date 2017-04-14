#include "printsetup.h"
#include "datatype.h"
#include <QScrollArea>
#include "printitem.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QListWidgetItem>
#include <QStringList>



PrintSetup::PrintSetup(QVector<MyField> avlbFields, QVector<QSqlRecord> newRecords, QWidget *parent) : QWidget(parent)
{
    Fields=avlbFields;
    Fields.append(MyField("Static String","Main_table", DataType::Static));

    records=newRecords;

    cmbFields=new QComboBox();
    for(int i=0;i<Fields.size();i++){
        cmbFields->addItem(Fields[i].getName(),QVariant::fromValue(Fields[i]));
    }
    setLayout(mainLayout);
    mainLayout->addWidget(cmbFields,1,1,1,1);

    printItemList = new MyPrintList(newRecords, this);

    savePrinter->setOutputFormat(QPrinter::PdfFormat);

    populateSetup();
    savePrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));
    //previewPrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));


    savePrinter->setResolution(cmbResolution->currentData().toInt());
    //previewPrinter->setResolution(600);

    printPreview = new QPrintPreviewWidget(previewPrinter);
    printPreview->setSinglePageViewMode();
    pageSetupLayout->addWidget(lblPageSize,1,1,1,1);
    pageSetupLayout->addWidget(cmbPageSize,1,2,1,1);
    pageSetupLayout->addWidget(lblResolution,1,3,1,1);
    pageSetupLayout->addWidget(cmbResolution,1,4,1,1);

    mainLayout->addWidget(btnAddField,1,2,1,1);
    mainLayout->addWidget(printItemList,2,1,5,4);
    mainLayout->addWidget(printPreview,1,5,6,6);
    mainLayout->addLayout(pageSetupLayout,7,1,1,4);


    connect(btnAddField,SIGNAL(clicked()),this,SLOT(addField()));
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(previewPrint(QPrinter*)));
    connect(printItemList, SIGNAL(itemAdded()), this, SLOT(printUpdated()));
    connect(printItemList, SIGNAL(itemModified()), this, SLOT(printUpdated()));
    connect(cmbPageSize, SIGNAL(currentIndexChanged(int)), this, SLOT(pageSetupUpdated()));
    connect(cmbResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(pageSetupUpdated()));
    resize(1000,1000);



}

void PrintSetup::populateSetup(){

    avlbPaperSize->insert("A0 (841 x 1189 mm)",QPrinter::A0);
    avlbPaperSize->insert("A1 (594 x 841 mm)",QPrinter::A1);
    avlbPaperSize->insert("A2 (420 x 594 mm)",QPrinter::A2);
    avlbPaperSize->insert("A3 (297 x 420 mm)",QPrinter::A3);
    avlbPaperSize->insert("A4 (210 x 297 mm)",QPrinter::A4);
    avlbPaperSize->insert("A5 (148 x 210 mm)",QPrinter::A5);
    avlbPaperSize->insert("A6 (105 x 148 mm)",QPrinter::A6);
    avlbPaperSize->insert("Legal (215.9 x 355.6 mm)",QPrinter::Legal);
    avlbPaperSize->insert("Letter (215.9 x 279.4 mm)",QPrinter::Letter);
    cmbPageSize->addItems(QStringList(avlbPaperSize->keys()));

    cmbPageSize->setCurrentIndex(4);

    cmbResolution->addItem("36 dpi", 36);
    cmbResolution->addItem("72 dpi", 72);
    cmbResolution->addItem("96 dpi", 96);
    cmbResolution->addItem("150 dpi", 150);
    cmbResolution->addItem("200 dpi", 200);
    cmbResolution->addItem("300 dpi", 300);
    cmbResolution->addItem("600 dpi", 600);
    cmbResolution->setCurrentIndex(3);
}

void PrintSetup::pageSetupUpdated(){
    /*
    savePrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));
    previewPrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));
    savePrinter->setResolution(cmbResolution->currentData().toInt());
    previewPrinter->setResolution(cmbResolution->currentData().toInt());
    */
    savePrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));
    savePrinter->setResolution(cmbResolution->currentData().toInt());

    qDebug()<<"Selected page size: "<<cmbPageSize->currentText();
    qDebug()<<"Enum value: "<<avlbPaperSize->value(cmbPageSize->currentText());
    qDebug()<<"Size: "<<previewPrinter->pageSize();
    qDebug()<<"Rect: "<<previewPrinter->pageRect(QPrinter::DevicePixel);

    qDebug()<<"Printer Preview Res: "<<previewPrinter->resolution();
    qDebug()<<"Printer DPI: "<<previewPrinter->physicalDpiX()<<"x"<<previewPrinter->physicalDpiY();
    qDebug()<<"Printer DPI: "<<previewPrinter->logicalDpiX()<<"x"<<previewPrinter->logicalDpiY();

    printUpdated();
}

void PrintSetup::printUpdated(){
    printPreview->updatePreview();
}

void PrintSetup::addField(){
    printItemList->addPrintItem(qvariant_cast<MyField>(cmbFields->currentData()));

}

void PrintSetup::printRecords(){


    QString fileName("/Users/Nick/Desktop/prova.pdf");
    savePrinter->setOutputFileName(fileName);
    QTextDocument *doc = printItemList->printRecords(savePrinter);
    doc->print(savePrinter); // doc is QTextDocument *
}

void PrintSetup::previewPrint(QPrinter *printer){
    qDebug()<<"Preview print called";
    QTextDocument *doc = printItemList->printRecords(printer);
    /*QTextDocument *doc=new QTextDocument();
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
    }*/
    qDebug()<<doc->toPlainText();
    doc->print(printer); // doc is QTextDocument *
}

PrintSetup::~PrintSetup()
{

}


//Q_DECLARE_METATYPE(MyField);
//Q_DECLARE_METATYPE(QVector<MyField>);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
