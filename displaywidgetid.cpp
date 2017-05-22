#include "displaywidgetid.h"
#include <QHBoxLayout>


DisplayWidgetID::DisplayWidgetID(MyField newField, bool editable, QWidget *parent)
    :DisplayWidgetBase(newField,parent)
{
    editTitle->setText(field.getName()+':');
    nonEditTitle->setText(field.getName()+':');
    QHBoxLayout *editLayout = new QHBoxLayout();
    QHBoxLayout *nonEditLayout = new QHBoxLayout();
    editLayout->addWidget(editTitle);
    editLayout->addWidget(lblID);
    editLayout->addStretch(10);
    nonEditLayout->addWidget(nonEditTitle);
    nonEditLayout->addWidget(lblNonEditText);
    nonEditLayout->addStretch(10);
    editWidget->setLayout(editLayout);
    nonEditWidget->setLayout(nonEditLayout);
    setCurrentIndex(NonEditable);



}

DisplayWidgetID::~DisplayWidgetID()
{

}

void DisplayWidgetID::setValue(QVariant newValue){
    lblID->setText(newValue.toString());
    lblNonEditText->setText(newValue.toString());
}

QVariant DisplayWidgetID::getValue(){
    return lblID->text();
}
