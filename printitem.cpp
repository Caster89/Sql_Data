#include "printitem.h"
#include "datatype.h"
//#include "printitembase.h"
#include "printitemimage.h"
#include "printitemimages.h"
#include "printitemtext.h"
#include "printitemlongtext.h"
#include "printitemstatic.h"
#include "printitemcombined.h"
#include <QStackedWidget>
#include <QDebug>
#include <QWidget>
#include <QBoxLayout>



PrintItem::PrintItem(MyField newField, QWidget *parent) : QWidget(parent)
{

    QHBoxLayout *displayLayout = new QHBoxLayout;
    switch(newField.getType()){
        case(DataType::Text):
        case( DataType::MultipleChoice):
            printItemPointer= new PrintItemText(newField);
            break;
        case(DataType::LongText):
            printItemPointer= new PrintItemLongText(newField);
            break;
        case(DataType::Image):
            printItemPointer=new PrintItemImage(newField);
            break;
        case(DataType::Images):
            printItemPointer=new PrintItemImages(newField);
            break;
        case(DataType::Static):
            printItemPointer=new PrintItemStatic(newField);
        case(DataType::Combined):
            printItemPointer=new PrintItemCombined(newField);
        break;
    }
    displayLayout->addWidget(printItemPointer);
    displayLayout->setMargin(1);
    displayLayout->setSpacing(1);
    setLayout(displayLayout);
    qDebug()<<"PrinterItem::PrinterItem() connectiong close signals"<<connect(printItemPointer, SIGNAL(closeWidget()),this,SLOT(closeWidgetSlot()));
    connect(printItemPointer, SIGNAL(itemModified()),this, SIGNAL(itemModified()));
}

void PrintItem::closeWidgetSlot(){
    qDebug()<<"PrintItem::closeWidgetSlot called";
    emit closeWidget();
}

