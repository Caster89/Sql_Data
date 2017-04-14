#include "displaywidgetbase.h"


DisplayWidgetBase::DisplayWidgetBase(MyField newField, bool editable, QWidget *parent)
    : QStackedWidget(parent)
{
    field=newField;
    addWidget(editWidget);
    addWidget(nonEditWidget);
    if(editable){
        setCurrentIndex(Editable);
    }else{
        setCurrentIndex(NonEditable);
    }
}

DisplayWidgetBase::~DisplayWidgetBase()
{

}
