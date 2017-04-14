#include "printitem.h"
#include "datatype.h"
//#include "printitembase.h"
#include "printitemimage.h"
#include "printitemimages.h"
#include "printitemtext.h"
#include "printitemlongtext.h"
#include "printitemstatic.h"
#include <QStackedWidget>
#include <QDebug>
#include <QWidget>
#include <QBoxLayout>



PrintItem::PrintItem(MyField newField, QWidget *parent) : QWidget(parent)
{

    QHBoxLayout *displayLayout = new QHBoxLayout;
    switch(newField.getType()){
    case(DataType::Text):
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
    }
    displayLayout->addWidget(printItemPointer);
    setLayout(displayLayout);
    connect(printItemPointer, SIGNAL(closeWidget()),this,SIGNAL(closeWidget()));
    connect(printItemPointer, SIGNAL(itemModified()),this, SIGNAL(itemModified()));
}


