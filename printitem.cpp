#include "printitem.h"
#include "datatype.h"
//#include "printitembase.h"
#include "printitemimage.h"
#include "printitemtext.h"
#include "printitemlongtext.h"
#include <QStackedWidget>
#include <QDebug>



PrintItem::PrintItem(MyField newField, QObject *parent) : QObject(parent)
{


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

        break;
    }

}


