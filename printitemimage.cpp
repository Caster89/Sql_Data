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
    //captionSize->addItems(fontSize);
    //qDebug()<<"Before Connection";

    cmbImageSize->addItem("Full Page");
    cmbImageSize->addItem("Full Width");
    cmbImageSize->addItem("Fixed Height");
    cmbImageSize->addItem("Fixed Width");
    cmbImageSize->addItem("Set Width and Height");

    buildWidget();

}

void PrintItemImage::buildWidget(){


    //captionBox->setTitle("Caption Settings");
            
    //The Combobox for determening the position of the caption is populated
    //cmbCaptionPos->setIconSize(QSize(40,40));
    //cmbCaptionPos->addItem(QIcon(":/Cpt_Bottom"),"");
    //cmbCaptionPos->addItem(QIcon(":/Cpt_Top"),"");
    //cmbCaptionPos->addItem(QIcon(":/Cpt_Left"),"");
    //cmbCaptionPos->addItem(QIcon(":/Cpt_Right"),"");

    //cmbCaptionPos->resize(60,40);
    //cmbCaptionPos->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    //cmbCaptionPos->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    //cmbCaptionPos->setMinimumHeight(60);

    /*QHBoxLayout *captionFontWeights = new QHBoxLayout();
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
    */
    
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
    QHBoxLayout *imagePositionLayout = new QHBoxLayout();
    imagePositionLayout->addWidget(valueLeft);
    imagePositionLayout->addWidget(valueCenter);
    imagePositionLayout->addWidget(valueRight);
    imagePositionLayout->addWidget(valueJustify);
    imagePositionLayout->addStretch(0);
    
    //QVBoxLayout *captionLayout=new QVBoxLayout();
    //QHBoxLayout *positionLayout=new QHBoxLayout();
    //QHBoxLayout *captionLayout1=new QHBoxLayout();
    //QHBoxLayout *captionLayout2=new QHBoxLayout();
    //positionLayout->addWidget(lblCaptionPos);
    //positionLayout->addWidget(cmbCaptionPos);
    //positionLayout->addStretch(0);
    /*captionLayout1->addWidget(lblCaptionFont);
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
    captionBox->setLayout(captionLayout);*/

    valueLayout->addWidget(NewLine);
    valueLayout->addLayout(imageLayout);
    valueLayout->addLayout(imagePositionLayout);

    //CONNECTIONS
    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(imagePrintChanged()));
    connect(cmbImageSize,SIGNAL(currentIndexChanged(int)),this,SLOT(imagePrintChanged()));
    //connect(cmbCaptionPos,SIGNAL(currentIndexChanged(int)),this,SLOT(captionPrintChanged()));
    connect(ledtImageHeight,SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(ledtImageWidth,SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(alignGroup, SIGNAL(buttonClicked(int)),this,SLOT(alignmentChanged(int)));

    //connect(captionSize,SIGNAL(currentIndexChanged(int)),this,SLOT(captionPrintChanged()));
    //connect(captionFontStyle,SIGNAL(currentFontChanged(int)),this,SLOT(captionPrintChanged()));
    //connect(capNumbered,SIGNAL(stateChanged(int)),this,SLOT(captionPrintChanged()));


}
/*
void PrintItemImage::captionPrintChanged(){
    captionFont=captionFontStyle->currentFont();
    captionFont.setPointSize(captionSize->currentText().toInt());
    captionFont.setBold(captionBold->isChecked());
    captionFont.setItalic(captionItalics->isChecked());
    captionFont.setUnderline(captionUnderlined->isChecked());
    captionPosition=cmbCaptionPos->currentIndex();

}
*/
void PrintItemImage::alignmentChanged(int align){
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

void PrintItemImage::imagePrintChanged(){
    switch(cmbImageSize->currentIndex()){
        case(0):
        //FullPage
        ledtImageHeight->setEnabled(false);
        lblHeight->setEnabled(false);
        ledtImageWidth->setEnabled(false);
        lblWidth->setEnabled(false);
        break;
        case(1):
        //FullWidth
        ledtImageHeight->setEnabled(false);
        lblHeight->setEnabled(false);
        ledtImageWidth->setEnabled(false);
        lblWidth->setEnabled(false);
        break;
        case(2):
        //FixedHeight
        ledtImageHeight->setEnabled(true);
        lblHeight->setEnabled(true);
        ledtImageWidth->setEnabled(false);
        lblWidth->setEnabled(false);
        break;
        case(3):
        //FixedWidth
        ledtImageHeight->setEnabled(false);
        lblHeight->setEnabled(false);
        ledtImageWidth->setEnabled(true);
        lblWidth->setEnabled(true);
        break;
        case(4):
        //SetWidthHeight
        ledtImageHeight->setEnabled(true);
        lblHeight->setEnabled(true);
        ledtImageWidth->setEnabled(true);
        lblWidth->setEnabled(true);
        break;
    }
    imageScaling = cmbImageSize->currentIndex();
    emit itemModified();




}

void PrintItemImage::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    QTextFrame *frame=cursor->insertFrame(frameFormat);

    QSizeF pageSize = doc->pageSize();
    qDebug()<<"The size of the page "<<pageSize;

    float w = 0;
    float h = 0;


    //QString imageDir=record->value(field.getName()).toString();


    //QTextFrame *topFrame = cursor.currentFrame();
    QTextBlockFormat blockFormat = cursor->blockFormat();
    blockFormat.setAlignment(titleAlignment);
    cursor->setBlockFormat(blockFormat);
    if(titleVisible->isChecked()){
        QTextCharFormat titleFormat;
        titleFormat.setFont(titleFont);
        cursor->insertText(QString("%1: ").arg(field.getName()),titleFormat);
        if(valueNewLine){
            blockFormat.setAlignment(alignment);
            cursor->insertBlock();
        }
    }



    QSqlRecord imageData = qvariant_cast<QSqlRecord>(record->value(field.getName()));
    QImage Image(imageData.value("File").toString());
    qDebug()<<"Fiel Name: "<<imageData.value("File").toString();

    switch(imageScaling){
    //Full Page
        case(0):{
            w = pageSize.width();
            h = pageSize.height();

            Image = Image.scaled(w,h,Qt::KeepAspectRatio);
                //qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
        }
        //Full Width
        case(1):{
            w = pageSize.width();
            Image = Image.scaledToWidth(w);
            //qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
        }
            //Fixed Height
        case(2):{
            h = ledtImageHeight->text().toFloat();
            Image = Image.scaledToHeight(h);
            //qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
        }
            //Fixed Width
        case(3):{
            w = ledtImageWidth->text().toFloat();
            Image = Image.scaledToWidth(w);
            //qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
        }
        //Fixed Width & Height
        case(4):{
            w = ledtImageWidth->text().toFloat();
            h = ledtImageHeight->text().toFloat();
            Image = Image.scaled(w,h,Qt::IgnoreAspectRatio);
            //qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
        }
    }
    cursor->insertImage(Image);

}

