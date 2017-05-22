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

    qDebug()<<"PrintItem::PrintItem() "<<newField.getName()<<" tye: "<<newField.getType();
    QHBoxLayout *displayLayout = new QHBoxLayout;
    switch(newField.getType()){
        case(DataType::Text):
        case( DataType::MultipleChoice):
        case( DataType::ID):
            qDebug()<<"Type Text/MC/ID";
            printItemPointer= new PrintItemText(newField);
            break;
        case(DataType::LongText):
            qDebug()<<"Type LongText";
            printItemPointer= new PrintItemLongText(newField);
            break;
        case(DataType::Image):
            qDebug()<<"Type Image";
            printItemPointer=new PrintItemImage(newField);
            break;
        case(DataType::Images):
            qDebug()<<"Type Images";
            printItemPointer=new PrintItemImages(newField);
            break;
        case(DataType::Static):
            qDebug()<<"Type Static";
            printItemPointer=new PrintItemStatic(newField);
            break;
        case(DataType::Combined):
            qDebug()<<"Type Combined";
            printItemPointer=new PrintItemCombined(newField);
        break;
    }
    displayLayout->addWidget(printItemPointer);
    displayLayout->setMargin(1);
    displayLayout->setSpacing(1);
    setLayout(displayLayout);
    connect(printItemPointer, SIGNAL(itemModified()),this, SIGNAL(itemModified()));
}

void PrintItem::closeWidgetSlot(){
    qDebug()<<"PrintItem::closeWidgetSlot called";
    emit closeWidget();
}

