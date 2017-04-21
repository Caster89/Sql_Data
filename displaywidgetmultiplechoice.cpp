#include "displaywidgetmultiplechoice.h"
#include <QHBoxLayout>

DisplayWidgetMultipleChoice::DisplayWidgetMultipleChoice(MyField newField, bool editable, QWidget *parent)
    :DisplayWidgetBase(newField,parent)
{
    editTitle->setText(field.getName()+':');
    nonEditTitle->setText(field.getName()+':');
    QHBoxLayout *editLayout = new QHBoxLayout();
    QHBoxLayout *nonEditLayout = new QHBoxLayout();
    qDebug()<<"Comments: "<<newField.getComments();
    *avlbChoices = newField.getComments().split(";");
    avlbChoices->prepend("");
    cmbValues->addItem("Select value...", "");
    for(int i = 1; i<avlbChoices->count();i++){

        cmbValues->addItem(avlbChoices->at(i),QVariant::fromValue(avlbChoices->at(i)));
    }
    cmbValues->setCurrentIndex(0);

    //lblNonEditText->setWordWrap(true);
    editLayout->addWidget(editTitle);
    editLayout->addWidget(cmbValues);
    nonEditLayout->addWidget(nonEditTitle);
    nonEditLayout->addWidget(lblValue);

    editWidget->setLayout(editLayout);
    nonEditWidget->setLayout(nonEditLayout);
    if(editable){
        setCurrentIndex(Editable);
    }else{
        setCurrentIndex(NonEditable);
    }
    connect(cmbValues, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged(int)));
}

QVariant DisplayWidgetMultipleChoice::getValue()
{
    return lblValue->text();
}

void DisplayWidgetMultipleChoice::selectionChanged(int newSelection){
    lblValue->setText(cmbValues->currentData().toString());
}

void DisplayWidgetMultipleChoice::setValue(QVariant newValue)
{
    lblValue->setText(newValue.toString());
    cmbValues->setCurrentIndex(avlbChoices->lastIndexOf(newValue.toString()));
}

DisplayWidgetMultipleChoice::~DisplayWidgetMultipleChoice()
{

}
