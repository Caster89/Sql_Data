#include "printitemlongtext.h"
#include "printitembase.h"
#include "myfield.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>


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
    QFont btnFont=valueBold->font();
    btnFont.setBold(true);
    valueBold->setCheckable(true);

    valueUnderlined->setText("U");
    valueBold->setFont(btnFont);
    btnFont.setBold(false);
    btnFont.setUnderline(true);
    valueUnderlined->setFont(btnFont);
    valueUnderlined->setCheckable(true);

    valueItalics->setText("I");
    btnFont.setUnderline(false);
    btnFont.setItalic(true);
    valueItalics->setFont(btnFont);
    valueItalics->setCheckable(true);


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



    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(valueFontChanged()));
    connect(valueSize,SIGNAL(currentIndexChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueBold,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(valueItalics,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(valueUnderlined,SIGNAL(clicked()),this,SLOT(valueFontChange()));
    connect(alignGroup, SIGNAL(buttonClicked(int)),this,SLOT(alignmentChanged(int)));

    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());
}

void PrintItemLongText::valueFontChanged(){
    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());
    valueFont.setBold(valueBold->isChecked());
    valueFont.setItalic(valueItalics->isChecked());
    valueFont.setUnderline(valueUnderlined->isChecked());
    valueNewLine=NewLine->isChecked();
    emit itemModified();

}

void PrintItemLongText::alignmentChanged(int align){
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

void PrintItemLongText::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord* record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    QTextFrame *frame=cursor->insertFrame(frameFormat);

    //QTextFrame *topFrame = cursor.currentFrame();
    QTextBlockFormat blockFormat = cursor->blockFormat();

    if(titleVisible->isChecked()){
        blockFormat.setAlignment(titleAlignment);
        cursor->setBlockFormat(blockFormat);
        QTextCharFormat titleFormat;
        titleFormat.setFont(titleFont);
        cursor->insertText(QString("%1: ").arg(field.getName()),titleFormat);
        if(valueNewLine){
            cursor->insertBlock();
            cursor->setBlockFormat(blockFormat);
        }
    }
    blockFormat.setAlignment(alignment);
    cursor->setBlockFormat(blockFormat);
    QTextCharFormat valueFormat;
    valueFormat.setFont(valueFont);
    cursor->insertText(record->value(field.getName()).toString(),valueFormat);
}
