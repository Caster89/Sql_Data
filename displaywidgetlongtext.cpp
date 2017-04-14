#include <QVBoxLayout>
#include "displaywidgetlongtext.h"

DisplayWidgetLongText::DisplayWidgetLongText(MyField newField, bool editable, QWidget *parent)
    :DisplayWidgetBase(newField,parent)
{
    editTitle->setText(field.getName()+':');
    nonEditTitle->setText(field.getName()+':');
    QVBoxLayout *editLayout = new QVBoxLayout();
    QVBoxLayout *nonEditLayout = new QVBoxLayout();
    lblNonEditText->setWordWrap(true);
    editLayout->addWidget(editTitle);
    editLayout->addWidget(longText);
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

QVariant DisplayWidgetLongText::getValue()
{
    return longText->toPlainText();
}

void DisplayWidgetLongText::setValue(QVariant newValue)
{
    longText->setText(newValue.toString());
    lblNonEditText->setText(newValue.toString());
}

DisplayWidgetLongText::~DisplayWidgetLongText()
{

}

