#include "printitemimage.h"
#include "printitembase.h"
#include <QIcon>
#include <QDebug>
#include <QTextTable>

/******************************************************************/
//The PrintItemImage valueBox:
//  ____________________________________________________________
// |                                                            |
// | [] New Line                                                |
// |              _______________     ___     ___               |
// | Image Size: |_|_____________| W:|___| H:|___|              |
// |                                                            |
// | ____Caption Box___________________________________________ |
// ||             __________                                   ||
// ||            |       |  |       ______________        ___  ||
// ||Positioning:|       |  | Font:|_|____________| Size:|___| ||
// ||            |_______|__|                                  ||
// ||                ______________                            ||
// ||Initial String:|______________|  []Sequential Numbering   ||
// ||__________________________________________________________||
// |____________________________________________________________|

PrintItemImage::PrintItemImage()
{
    cmbImageSize->addItem("Full Page");
    cmbImageSize->addItem("Full Width");
    cmbImageSize->addItem("Fixed Height");
    cmbImageSize->addItem("Fixed Width");
    cmbImageSize->addItem("Set Width and Height");

}

PrintItemImage::PrintItemImage(MyField field)
    :PrintItemBase(field)
{
    captionSize->addItems(fontSize);
    //qDebug()<<"Before Connection";

    cmbImageSize->addItem("Full Page");
    cmbImageSize->addItem("Full Width");
    cmbImageSize->addItem("Fixed Height");
    cmbImageSize->addItem("Fixed Width");
    cmbImageSize->addItem("Set Width and Height");

    buildWidget();

}

void PrintItemImage::buildWidget(){


    captionBox->setTitle("Caption Settings");
            
    //The Combobox for determening the position of the caption is populated
    cmbCaptionPos->setIconSize(QSize(40,40));
    cmbCaptionPos->addItem(QIcon(":/Cpt_Bottom"),"");
    cmbCaptionPos->addItem(QIcon(":/Cpt_Top"),"");
    cmbCaptionPos->addItem(QIcon(":/Cpt_Left"),"");
    cmbCaptionPos->addItem(QIcon(":/Cpt_Right"),"");

    //cmbCaptionPos->resize(60,40);
    cmbCaptionPos->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    cmbCaptionPos->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    cmbCaptionPos->setMinimumHeight(60);

    QHBoxLayout *captionFontWeights = new QHBoxLayout();
    QFont btnFont=captionBold->font();
    captionBold->setText("B");
    captionBold->setCheckable(true);
    btnFont.setBold(true);
    captionBold->setFont(btnFont);
    btnFont.setBold(false);
    captionUnderlined->setText("U");
    captionUnderlined->setCheckable(true);
    btnFont.setUnderline(true);
    captionUnderlined->setFont(btnFont);
    btnFont.setUnderline(false);

    captionItalics->setText("I");
    captionItalics->setCheckable(true);
    btnFont.setItalic(true);
    captionItalics->setFont(btnFont);
    btnFont.setItalic(false);
    captionFontWeights->addWidget(captionBold);
    captionFontWeights->addWidget(captionItalics);
    captionFontWeights->addWidget(captionUnderlined);
    captionFontWeights->addStretch(0);

    
    QHBoxLayout *imageLayout =new QHBoxLayout();
    ledtImageHeight->setMaximumWidth(30);
    ledtImageWidth->setMaximumWidth(30);
    imageLayout->addWidget(lblImageSize);
    imageLayout->addWidget(cmbImageSize);
    imageLayout->addWidget(lblWidth);
    imageLayout->addWidget(ledtImageWidth);
    imageLayout->addWidget(lblHeight);
    imageLayout->addWidget(ledtImageHeight);
    imageLayout->addStretch(0);
    
    QVBoxLayout *captionLayout=new QVBoxLayout();
    QHBoxLayout *positionLayout=new QHBoxLayout();
    QHBoxLayout *captionLayout1=new QHBoxLayout();
    QHBoxLayout *captionLayout2=new QHBoxLayout();
    positionLayout->addWidget(lblCaptionPos);
    positionLayout->addWidget(cmbCaptionPos);
    positionLayout->addStretch(0);
    captionLayout1->addWidget(lblCaptionFont);
    captionLayout1->addWidget(captionFontStyle);
    captionLayout1->addWidget(lblCaptionSize);
    captionLayout1->addWidget(captionSize);
    captionLayout1->addStretch(0);
    captionLayout2->addWidget(lblCaptionPre);
    captionLayout2->addWidget(ledtCaptionPre);
    captionLayout2->addWidget(capNumbered);
    captionLayout2->addStretch(0);
    captionLayout->addLayout(positionLayout);
    captionLayout->addLayout(captionLayout1);
    captionLayout->addLayout(captionFontWeights);
    captionLayout->addLayout(captionLayout2);
    captionBox->setLayout(captionLayout);

    valueLayout->addWidget(NewLine);
    valueLayout->addLayout(imageLayout);
    valueLayout->addWidget(captionBox);

    //CONNECTIONS
    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(imagePrintChanged()));
    connect(cmbImageSize,SIGNAL(currentIndexChanged(int)),this,SLOT(imagePrintChanged()));
    connect(cmbCaptionPos,SIGNAL(currentIndexChanged(int)),this,SLOT(captionPrintChanged()));
    connect(ledtImageHeight,SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(ledtImageWidth,SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(captionSize,SIGNAL(currentIndexChanged(int)),this,SLOT(captionPrintChanged()));
    connect(captionFontStyle,SIGNAL(currentFontChanged(int)),this,SLOT(captionPrintChanged()));
    //connect(capNumbered,SIGNAL(stateChanged(int)),this,SLOT(captionPrintChanged()));


}

void PrintItemImage::captionPrintChanged(){
    captionFont=captionFontStyle->currentFont();
    captionFont.setPointSize(captionSize->currentText().toInt());
    captionFont.setBold(captionBold->isChecked());
    captionFont.setItalic(captionItalics->isChecked());
    captionFont.setUnderline(captionUnderlined->isChecked());
    captionPosition=cmbCaptionPos->currentIndex();

}

void PrintItemImage::imagePrintChanged(){
    switch(cmbImageSize->currentIndex()){
        case(0):
        //FullPage
        ledtImageHeight->setEnabled(false);
        ledtImageWidth->setEnabled(false);
        break;
        case(1):
        //FullWidth
        ledtImageHeight->setEnabled(false);
        ledtImageWidth->setEnabled(false);
        break;
        case(2):
        //FixedHeight
        ledtImageHeight->setEnabled(true);
        ledtImageWidth->setEnabled(false);
        break;
        case(3):
        //FixedWidth
        ledtImageHeight->setEnabled(false);
        ledtImageWidth->setEnabled(true);
        break;
        case(4):
        //SetWidthHeight
        ledtImageHeight->setEnabled(true);
        ledtImageWidth->setEnabled(true);
        break;
    }




}

void PrintItemImage::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    QTextFrame *frame=cursor->insertFrame(frameFormat);

    QString imageDir=record->value(field.getName()).toString();


    //QTextFrame *topFrame = cursor.currentFrame();
    QTextBlockFormat blockFormat = cursor->blockFormat();
    blockFormat.setAlignment(alignment);
    cursor->setBlockFormat(blockFormat);
    if(titleVisible->isChecked()){
        QTextCharFormat titleFormat;
        titleFormat.setFont(titleFont);
        cursor->insertText(QString("%1: ").arg(field.getName()),titleFormat);
        if(valueNewLine){
            cursor->insertBlock();
        }
    }

    QTextTable *imageTable;
    QTextTableFormat imageTableFormat;
    imageTableFormat.setAlignment(Qt::AlignCenter);
    QImage *Image=new QImage("/Users/Nick/Desktop/Unknown.jpeg");
    cursor->insertBlock();
    cursor->insertImage(*Image);
    switch(captionPosition){
    case(0):{
        imageTable = cursor->insertTable(2, 1, imageTableFormat);
        QTextFrameFormat imageFrameFormat = cursor->currentFrame()->frameFormat();
        imageFrameFormat.setBorder(0);
        QTextCursor tempCursor = imageTable->cellAt(0, 0).firstCursorPosition();
        //cursor.insertImage(Image);
        //cursor.insertImage(Image);

        break;}
    case(1):{
        imageTable = cursor->insertTable(2, 1, imageTableFormat);
         QTextCursor tempCursor = imageTable->cellAt(1, 0).firstCursorPosition();
        //cursor.insertImage(Image);
        break;}
    case(2):{
        imageTable = cursor->insertTable(1, 2, imageTableFormat);
        QTextCursor tempCursor = imageTable->cellAt(0, 1).firstCursorPosition();
        //cursor.insertImage(Image);
        break;}
    case(3):{
        imageTable = cursor->insertTable(1, 2, imageTableFormat);
        QTextCursor tempCursor = imageTable->cellAt(0, 0).firstCursorPosition();
        //cursor.insertImage(Image);
        break;}
    }


}

