#include "printitemimages.h"
#include "printitembase.h"
#include <QIcon>
#include <QDebug>
#include <QTextTable>
#include <QAbstractTextDocumentLayout>

/******************************************************************/
//The PrintItemImages valueBox:
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

PrintItemImages::PrintItemImages()
{
    cmbImageSize->addItem("Full Page");
    cmbImageSize->addItem("Full Width");
    cmbImageSize->addItem("Fixed Height");
    cmbImageSize->addItem("Fixed Width");
    cmbImageSize->addItem("Set Width and Height");

}

PrintItemImages::PrintItemImages(MyField field)
    :PrintItemBase(field)
{
    captionSize->addItems(fontSize);
    //qDebug()<<"Before Connection";



    buildWidget();

}

void PrintItemImages::buildWidget(){

    cmbImageSize->addItem("Full Page");
    cmbImageSize->addItem("Full Width");
    cmbImageSize->addItem("# Images per line");
    cmbImageSize->addItem("Fixed Width");
    cmbImageSize->addItem("Set Width and Height");

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
    ledtImageHeight->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
    lblHeight->setVisible(false);
    ledtImageHeight->setVisible(false);

    ledtImageWidth->setMaximumWidth(30);
    ledtImageWidth->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
    lblWidth->setVisible(false);
    ledtImageWidth->setVisible(false);

    ledtNumImages->setMaximumWidth(30);
    ledtNumImages->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
    lblNumImages->setVisible(false);
    ledtNumImages->setVisible(false);

    imageLayout->addWidget(lblImageSize);
    imageLayout->addWidget(cmbImageSize);
    imageLayout->addWidget(lblWidth);
    imageLayout->addWidget(ledtImageWidth);
    imageLayout->addWidget(lblHeight);
    imageLayout->addWidget(ledtImageHeight);
    imageLayout->addWidget(lblNumImages);
    imageLayout->addWidget(ledtNumImages);
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
    connect(ledtNumImages, SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(captionSize,SIGNAL(currentIndexChanged(int)),this,SLOT(captionPrintChanged()));
    connect(captionFontStyle,SIGNAL(currentFontChanged(int)),this,SLOT(captionPrintChanged()));
    //connect(capNumbered,SIGNAL(stateChanged(int)),this,SLOT(captionPrintChanged()));


}

void PrintItemImages::captionPrintChanged(){
    captionFont=captionFontStyle->currentFont();
    captionFont.setPointSize(captionSize->currentText().toInt());
    captionFont.setBold(captionBold->isChecked());
    captionFont.setItalic(captionItalics->isChecked());
    captionFont.setUnderline(captionUnderlined->isChecked());
    captionPosition=cmbCaptionPos->currentIndex();
    emit itemModified();

}

void PrintItemImages::imagePrintChanged(){
    switch(cmbImageSize->currentIndex()){
        case(0):
        //FullPage
        lblHeight->setVisible(false);
        ledtImageHeight->setVisible(false);
        lblWidth->setVisible(false);
        ledtImageWidth->setVisible(false);
        lblNumImages->setVisible(false);
        ledtNumImages->setVisible(false);
        break;
        case(1):
        //FullWidth
        lblHeight->setVisible(false);
        ledtImageHeight->setVisible(false);
        lblWidth->setVisible(false);
        ledtImageWidth->setVisible(false);
        lblNumImages->setVisible(false);
        ledtNumImages->setVisible(false);
        break;
        case(2):
        //Num Images
        lblHeight->setVisible(false);
        ledtImageHeight->setVisible(false);
        lblWidth->setVisible(false);
        ledtImageWidth->setVisible(false);
        lblNumImages->setVisible(true);
        ledtNumImages->setVisible(true);
        break;
        case(3):
        //FixedWidth
        lblHeight->setVisible(false);
        ledtImageHeight->setVisible(false);
        lblWidth->setVisible(true);
        ledtImageWidth->setVisible(true);
        lblNumImages->setVisible(false);
        ledtNumImages->setVisible(false);
        break;
        case(4):
        //SetWidthHeight
        lblHeight->setVisible(true);
        ledtImageHeight->setVisible(true);
        lblHeight->setVisible(true);
        ledtImageWidth->setVisible(true);
        lblNumImages->setVisible(false);
        ledtNumImages->setVisible(false);
        break;
    }
    imageScaling = cmbImageSize->currentIndex();
    emit itemModified();



}

void PrintItemImages::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    QTextFrame *frame=cursor->insertFrame(frameFormat);

    QPaintDevice *printer=doc->documentLayout()->paintDevice();
    //QPageSize pageType = QPageSize(doc->pageSize());
    //QSizeF pageSize = pageType.size();
    QSizeF pageSize = doc->pageSize();
    qDebug()<<"The size of the page "<<pageSize;
    qDebug()<<"The size of the page in px "<<printer->width()<<" x "<<printer->height();
    qDebug()<<"The size of the page in mm "<<printer->widthMM()<<" x "<<printer->heightMM();
    qDebug()<<"The size of the page in inch "<<printer->widthMM()/2.5<<" x "<<printer->heightMM()/2.5;
    qDebug()<<"Pixels Per inch: "<<printer->width()/printer->widthMM()*25;
    //qDebug()<<"Margins"<<printer->margins().left<<printer->margins().right;
    //qDebug()<<"Margins"<<printer->margins().top<<printer->margins().bottom;

    float w = 0;
    float h = 0;


    //QString imageDir=record->value(field.getName()).toString();


    //QTextFrame *topFrame = cursor.currentFrame();
    //QTextBlockFormat blockFormat = cursor->blockFormat();
    //blockFormat.setAlignment(alignment);
    //cursor->setBlockFormat(blockFormat);
    if(titleVisible->isChecked()){
        QTextCharFormat titleFormat;
        titleFormat.setFont(titleFont);
        cursor->insertText(QString("%1: ").arg(field.getName()),titleFormat);
        //if(valueNewLine){
            //cursor->insertBlock();
        //}
    }


    QVector<QSqlRecord> images = qvariant_cast<QVector<QSqlRecord> >(record->value(field.getName()));
    int currRow = 0;
    int currCol = 0;
    float avlbSpace = pageSize.width();


    QTextTable *imageTable;
    QTextTableFormat imageTableFormat;
    imageTableFormat.setAlignment(Qt::AlignCenter);
    imageTableFormat.setCellPadding(0);
    imageTableFormat.setCellSpacing(0);
    imageTableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
    imageTable = cursor->insertTable(1, 1, imageTableFormat);

    for(int i =0; i <images.count(); i++){

        QImage Image(images[i].value("File").toString());

        switch(imageScaling){
            //Full Page
            case(0):{
                w = pageSize.width();
                h = pageSize.height();

                Image = Image.scaled(w,h,Qt::KeepAspectRatio);
                qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
            }
            //Full Width
            case(1):{
                w = pageSize.width();

                Image = Image.scaledToWidth(w);
                qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
            }
            //Num Images
            case(2):{
                w = (pageSize.width()/ledtNumImages->text().toFloat());
                Image = Image.scaledToWidth(w);
                qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
            }
            //Fixed Width
            case(3):{
                w = ledtImageWidth->text().toFloat();
                Image = Image.scaledToWidth(w);
                qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
            }
            //Fixed Width & Height
            case(4):{
                w = ledtImageWidth->text().toFloat();
                h = ledtImageHeight->text().toFloat();
                Image = Image.scaled(w,h,Qt::IgnoreAspectRatio);
                qDebug()<<"Width: "<<Image.width()<<"Height: "<<Image.height();
            break;
            }
        }
        //qDebug()<<"Available space before image insertion: "<<avlbSpace;
        //qDebug()<<"Width of the image: "<<w;
        //qDebug()<<"Plannin on inserting the image in: "<<currRow<<" "<<currCol;
        //qDebug()<<"The talbe has size: "<<imageTable->rows()<<" "<<imageTable->columns();
        if(avlbSpace<w){
            qDebug()<<"New rows are needed";
            imageTable->appendRows(1);
            currCol = 0;
            currRow++;
            avlbSpace=pageSize.width()-w;

        }else{
            avlbSpace-=w;
            if(currCol==imageTable->columns()){
                qDebug()<<"New Columns are needed";
                imageTable->appendColumns(1);
            }

        }
        qDebug()<<"Will insert the image in: "<<currRow<<" "<<currCol;
        QTextTable *singImage;
        QString caption = images[i].value("Description").toString();
        if (caption == "No Description")
            caption = "";
        switch(captionPosition){
            case(0):{
                *cursor = imageTable->cellAt(currRow,currCol).firstCursorPosition();
                singImage = cursor->insertTable(2, 1, imageTableFormat);
                //QTextFrameFormat imageFrameFormat = cursor->currentFrame()->frameFormat();
                //imageFrameFormat.setBorder(0);
                //Image = Image.scaled(frame->pageSize(),)
                QTextCursor tableCursor = singImage->cellAt(0, 0).firstCursorPosition();
                tableCursor.insertImage(Image);
                tableCursor = singImage->cellAt(1, 0).firstCursorPosition();
                tableCursor.insertText(caption);
                //cursor.insertImage(Image);

                break;
            }
            case(1):{
                *cursor = imageTable->cellAt(currRow,currCol).firstCursorPosition();
                singImage = cursor->insertTable(2, 1, imageTableFormat);
                QTextCursor tableCursor = singImage->cellAt(0, 0).firstCursorPosition();

                tableCursor.insertText(caption);
                tableCursor = singImage->cellAt(1, 0).firstCursorPosition();
                tableCursor.insertImage(Image);

                //cursor.insertImage(Image);
                break;
            }
            case(2):{
                *cursor = imageTable->cellAt(currRow,currCol).firstCursorPosition();
                singImage = cursor->insertTable(1, 2, imageTableFormat);
                QTextCursor tableCursor = singImage->cellAt(0, 0).firstCursorPosition();
                tableCursor.insertText(caption);
                tableCursor = singImage->cellAt(0, 1).firstCursorPosition();
                tableCursor.insertImage(Image);
                break;}
            case(3):{
                *cursor = imageTable->cellAt(currRow,currCol).firstCursorPosition();
                singImage = cursor->insertTable(1, 2, imageTableFormat);
                QTextCursor tableCursor = singImage->cellAt(0, 0).firstCursorPosition();
                tableCursor.insertImage(Image);
                tableCursor = singImage->cellAt(0, 1).firstCursorPosition();
                tableCursor.insertText(caption);

                break;
            }
        }
        *cursor = frame->lastCursorPosition();
        currCol++;
    }


}

int PrintItemImages::ImagesPerRow(float avlbSpace, float width){
    float realWidth = 0;
    if(captionPosition==0 || captionPosition ==1){
        realWidth = width*1.1;
    }else if(captionPosition==2 || captionPosition==3){
        realWidth = width*1.55;
    }
    return (int)(qMax(avlbSpace/realWidth, (float)1.0));
}

