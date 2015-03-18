#include "printitemlongtext.h"
#include "printitembase.h"
#include "myfield.h"
#include <QGridLayout>
#include <QHBoxLayout>


PrintItemLongText::PrintItemLongText()
{

}

PrintItemLongText::PrintItemLongText(MyField field)
    :PrintItemBase(field)
{
    valueSize->addItems(fontSize);
    buildWidget();
}

void PrintItemLongText::buildWidget()
{


    NewLine->setChecked(true);


    //Two Layout are created like for the valuelayout
    QHBoxLayout *fontLayout = new QHBoxLayout();
    QHBoxLayout *fontStyleLayout=new QHBoxLayout();







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



    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(valueFontChanged()));
    connect(valueSize,SIGNAL(currentIndexChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueBold,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(valueItalics,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(valueUnderlined,SIGNAL(clicked()),this,SLOT(valueFontChange()));
}

void PrintItemLongText::valueFontChanged(){
    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());
    valueFont.setBold(valueBold->isChecked());
    valueFont.setItalic(valueItalics->isChecked());
    valueFont.setUnderline(valueUnderlined->isChecked());
    valueNewLine=NewLine->isChecked();

}
