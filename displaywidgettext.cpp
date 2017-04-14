#include "displaywidgettext.h"
#include "displaywidgetbase.h"
#include <QHBoxLayout>


DisplayWidgetText::DisplayWidgetText(MyField newField, bool editable, QWidget *parent)
    :DisplayWidgetBase(newField,parent)
{
    editTitle->setText(field.getName()+':');
    nonEditTitle->setText(field.getName()+':');
    QHBoxLayout *editLayout = new QHBoxLayout();
    QHBoxLayout *nonEditLayout = new QHBoxLayout();
    editLayout->addWidget(editTitle);
    editLayout->addWidget(shortText);
    nonEditLayout->addWidget(nonEditTitle);
    nonEditLayout->addWidget(lblNonEditText);
    editWidget->setLayout(editLayout);
    nonEditWidget->setLayout(nonEditLayout);
    if(editable){
        setCurrentIndex(Editable);
    }else{
        setCurrentIndex(NonEditable);
    }


}

DisplayWidgetText::~DisplayWidgetText()
{

}

void DisplayWidgetText::setValue(QVariant newValue){
    shortText->setText(newValue.toString());
    lblNonEditText->setText(newValue.toString());
}

QVariant DisplayWidgetText::getValue(){
    return shortText->text();
}
