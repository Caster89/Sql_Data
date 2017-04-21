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
#include <QFileDialog>
#include <QStandardPaths>
#include <QPdfWriter>



PrintSetup::PrintSetup(QVector<MyField> avlbFields, QVector<QSqlRecord> newRecords, QWidget *parent) : QDialog(parent)
{

    Fields=avlbFields;
    Fields.append(MyField("Static String","Main_table", DataType::Static));
    QString allTextFields = "";
    for (int i =0; i<avlbFields.count();i++){
        qDebug()<<"Getting Field: "<<i;
        if (avlbFields[i].getType()==DataType::Text || avlbFields[i].getType()==DataType::LongText){
            allTextFields.append(QString("%1;").arg(avlbFields[i].getName()));
        }
    }
    allTextFields.chop(1);
    Fields.append(MyField("Combined",allTextFields, DataType::Combined));

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
    //savePrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));
    //previewPrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));


    //savePrinter->setResolution(cmbResolution->currentData().toInt());
    //previewPrinter->setResolution(600);

    printPreview = new QPrintPreviewWidget(previewPrinter);
    printPreview->setSinglePageViewMode();
    pageSetupLayout->addWidget(lblPageSize,1,1,1,1);
    pageSetupLayout->addWidget(cmbPageSize,1,2,1,1);
    pageSetupLayout->addWidget(lblResolution,1,3,1,1);
    pageSetupLayout->addWidget(cmbResolution,1,4,1,1);

    mainLayout->addWidget(btnAddField,1,2,1,1);
    mainLayout->addWidget(printItemList,2,1,5,3);
    mainLayout->addWidget(printPreview,1,4,6,6);
    mainLayout->addLayout(pageSetupLayout,7,1,1,4);
    mainLayout->addWidget(btnExport,7,9,1,1);

    connect(btnAddField,SIGNAL(clicked()),this,SLOT(addField()));
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(previewPrint(QPrinter*)));
    connect(printItemList, SIGNAL(itemAdded()), this, SLOT(printUpdated()));
    connect(printItemList, SIGNAL(itemModified()), this, SLOT(printUpdated()));
    connect(printItemList, SIGNAL(itemMoved()), this, SLOT(printUpdated()));
    connect(cmbPageSize, SIGNAL(currentIndexChanged(int)), this, SLOT(pageSetupUpdated()));
    connect(cmbResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(pageSetupUpdated()));
    connect(btnExport, SIGNAL(clicked(bool)),this,SLOT(printRecords()));
    resize(1000,1000);
    setMinimumHeight(400);



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

    savePrinter->setPaperSize(avlbPaperSize->value(cmbPageSize->currentText()));
    savePrinter->setResolution(cmbResolution->currentData().toInt());

    qDebug()<<"Selected page size: "<<cmbPageSize->currentText();
    qDebug()<<"Enum value: "<<avlbPaperSize->value(cmbPageSize->currentText());
    qDebug()<<"Size: "<<savePrinter->pageSize();
    qDebug()<<"Rect: "<<savePrinter->pageRect(QPrinter::DevicePixel);

    qDebug()<<"Printer Preview Res: "<<savePrinter->resolution();
    qDebug()<<"Printer DPI: "<<savePrinter->physicalDpiX()<<"x"<<savePrinter->physicalDpiY();
    qDebug()<<"Printer DPI: "<<savePrinter->logicalDpiX()<<"x"<<savePrinter->logicalDpiY();

    printUpdated();
}

void PrintSetup::printUpdated(){
    printPreview->updatePreview();
}

void PrintSetup::addField(){
    printItemList->addPrintItem(qvariant_cast<MyField>(cmbFields->currentData()));

}

void PrintSetup::printRecords(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save File",
                               QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    fileName.append(".pdf");
    //savePrinter = new QPdfWriter(fileName);
    savePrinter->setOutputFileName(fileName);
    //QPdfWriter *pdfWriter = new QPdfWriter(fileName);
    //pdfWriter->setResolution(cmbResolution->currentData().toInt());
    //pdfWriter->setPageSize(QPageSize(QPageSize::A4));
    //qDebug()<<"Printer Dimensions: "<<pdfWriter->pageSize();
                //avlbPaperSize->value(cmbPageSize->currentText()));
    //QTextDocument *doc = printItemList->printRecords(savePrinter);
    QTextDocument *doc = printItemList->printRecords(savePrinter);
    qDebug()<<"DPI: "<<doc->pageSize();
    qDebug()<<"The size of the page "<<savePrinter->pageRect(QPrinter::DevicePixel);
    qDebug()<<"The size of the page "<<savePrinter->pageRect(QPrinter::Millimeter);
    qDebug()<<"The size of the page "<<savePrinter->pageRect(QPrinter::Inch);
    doc->print(savePrinter); // doc is QTextDocument *
}

void PrintSetup::previewPrint(QPrinter *printer){
    QTextDocument *doc = printItemList->printRecords(printer);

    qDebug()<<doc->toPlainText();
    doc->print(printer); // doc is QTextDocument *
}

PrintSetup::~PrintSetup()
{

}


//Q_DECLARE_METATYPE(MyField);
//Q_DECLARE_METATYPE(QVector<MyField>);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
