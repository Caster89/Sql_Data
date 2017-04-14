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


PrintItemText::PrintItemText(MyField newfield)
    :PrintItemBase(newfield)
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
    QFont btnFont=valueBold->font();
    btnFont.setBold(true);
    valueBold->setFont(btnFont);

    valueUnderlined->setText("U");
    valueUnderlined->setCheckable(true);
    btnFont.setBold(false);
    btnFont.setUnderline(true);
    valueUnderlined->setFont(btnFont);

    valueItalics->setText("I");
    valueItalics->setCheckable(true);
    btnFont.setUnderline(false);
    btnFont.setItalic(true);
    valueItalics->setFont(btnFont);


    valueLeft->setIcon(QIcon(":/Align_Left"));
    valueLeft->setCheckable(true);
    valueLeft->setChecked(true);
    alignGroup->addButton(valueLeft,alignMap->key(Qt::AlignLeft));

    valueCenter->setIcon(QIcon(":/Align_Center"));
    valueCenter->setCheckable(true);
    alignGroup->addButton(valueCenter,alignMap->key(Qt::AlignCenter));

    valueRight->setIcon(QIcon(":/Align_Right"));
    valueRight->setCheckable(true);
    alignGroup->addButton(valueRight, alignMap->key(Qt::AlignRight));

    valueJustify->setIcon(QIcon(":/Align_Justify"));
    valueJustify->setCheckable(true);
    alignGroup->addButton(valueJustify, alignMap->key(Qt::AlignJustify));

    //The widgets are added to the layouts

    fontLayout->addWidget(lblValueFont);
    fontLayout->addWidget(valueFontStyle);
    fontLayout->addWidget(lblValueSize);
    fontLayout->addWidget(valueSize);
    fontLayout->addStretch(0);

    fontStyleLayout->addWidget(valueBold);
    fontStyleLayout->addWidget(valueItalics);
    fontStyleLayout->addWidget(valueUnderlined);
    fontStyleLayout->addSpacing(40);
    fontStyleLayout->addWidget(valueLeft);
    fontStyleLayout->addWidget(valueCenter);
    fontStyleLayout->addWidget(valueRight);
    fontStyleLayout->addWidget(valueJustify);
    fontStyleLayout->addStretch(0);

    valueLayout->addWidget(NewLine);
    valueLayout->addLayout(fontLayout);
    valueLayout->addLayout(fontStyleLayout);

    valueBox->setLayout(valueLayout);



    
    //editLayout->addLayout(valueLayout,2,3,3,2);
    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(valueFontChanged()));
    connect(valueSize,SIGNAL(currentIndexChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueBold,SIGNAL(clicked()),this,SLOT(valueFontChanged()));
    connect(valueItalics,SIGNAL(clicked()),this,SLOT(valueFontChanged()));
    connect(valueUnderlined,SIGNAL(clicked()),this,SLOT(valueFontChanged()));
    connect(alignGroup, SIGNAL(buttonClicked(int)),this,SLOT(alignmentChanged(int)));
    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());


}

void PrintItemText::valueFontChanged(){
    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());
    valueFont.setBold(valueBold->isChecked());
    valueFont.setItalic(valueItalics->isChecked());
    valueFont.setUnderline(valueUnderlined->isChecked());
    valueNewLine=NewLine->isChecked();
    emit itemModified();
}

void PrintItemText::alignmentChanged(int align){
    qDebug()<<"Alignement Table"<<"\n"<<"=================";
    qDebug()<<"Left==>"<<Qt::AlignLeft;
    qDebug()<<"Right==>"<<Qt::AlignRight;
    qDebug()<<"Center==>"<<Qt::AlignCenter;
    qDebug()<<"Justify==>"<<Qt::AlignJustify;
    qDebug()<<"Alignment int"<<align;
    qDebug()<<"Alignment"<<alignMap->value(align);
    alignment = alignMap->value(align);
    emit itemModified();
}

//void PrintItemText::paintItem(QTextFrame *frame, QSqlRecord *record){
void PrintItemText::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    cursor->movePosition(QTextCursor::End);

    QTextFrame *frame=cursor->insertFrame(frameFormat);

    qDebug()<<"Printing Record"<<*record;
    //QTextFrame *topFrame = cursor.currentFrame();
    QTextBlockFormat blockFormat = cursor->blockFormat();

    QTextCharFormat valueFormat;
    valueFormat.setFont(valueFont);

    if(titleVisible->isChecked()){
        qDebug()<<"Title alignment is now:"<<titleAlignment;
        blockFormat.setAlignment(titleAlignment);
        cursor->setBlockFormat(blockFormat);
        QTextCharFormat titleFormat;
        titleFormat.setFont(titleFont);
        cursor->insertText(QString("%1:").arg(field.getName()),titleFormat);
        cursor->insertText(" ",valueFormat);
        if(valueNewLine){
            cursor->insertBlock();
        }
    }
    blockFormat.setAlignment(alignment);
    cursor->setBlockFormat(blockFormat);

    cursor->insertText(record->value(field.getName()).toString(),valueFormat);

}




