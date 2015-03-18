#include "printitembase.h"
#include "printitemtext.h"

#include "myfield.h"
#include <QObject>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

PrintItemText::PrintItemText()
    :PrintItemBase()
{
    buildWidget();
    valueSize->addItems(fontSize);

}


PrintItemText::PrintItemText(MyField field)
    :PrintItemBase(field)
{
    PrintItemText::buildWidget();
    valueSize->addItems(fontSize);
}

void PrintItemText::buildWidget()

{



    //Two Layout are created like for the valuelayout
    QHBoxLayout *fontLayout = new QHBoxLayout();
    QHBoxLayout *fontStyleLayout=new QHBoxLayout();


    //The widgets for setting the weight of the text B U I

    valueBold->setText("B");
    valueBold->setCheckable(true);
    valueItalics->setText("I");
    valueItalics->setCheckable(true);
    valueUnderlined->setText("U");
    valueUnderlined->setCheckable(true);

    QFont btnFont=valueBold->font();
    btnFont.setBold(true);
    valueBold->setFont(btnFont);
    btnFont.setBold(false);
    btnFont.setUnderline(true);
    valueUnderlined->setFont(btnFont);
    btnFont.setUnderline(false);
    btnFont.setItalic(true);
    valueItalics->setFont(btnFont);

    //The widgets are added to the layouts

    fontLayout->addWidget(lblValueFont);
    fontLayout->addWidget(valueFontStyle);
    fontLayout->addWidget(lblValueSize);
    fontLayout->addWidget(valueSize);
    fontLayout->addStretch(0);

    fontStyleLayout->addWidget(valueBold);
    fontStyleLayout->addWidget(valueItalics);
    fontStyleLayout->addWidget(valueUnderlined);
    fontStyleLayout->addStretch(0);

    valueLayout->addWidget(NewLine);
    valueLayout->addLayout(fontLayout);
    valueLayout->addLayout(fontStyleLayout);

    valueBox->setLayout(valueLayout);



    
    //editLayout->addLayout(valueLayout,2,3,3,2);
    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(valueFontChange()));
    connect(valueFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(valueFontChange()));
    connect(valueSize,SIGNAL(currentIndexChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueBold,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(valueItalics,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(valueUnderlined,SIGNAL(clicked()),this,SLOT(valueFontChange()));

}

void PrintItemText::valueFontChanged(){
    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());
    valueFont.setBold(valueBold->isChecked());
    valueFont.setItalic(valueItalics->isChecked());
    valueFont.setUnderline(valueUnderlined->isChecked());
    valueNewLine=NewLine->isChecked();
}




