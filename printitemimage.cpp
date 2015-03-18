#include "printitemimage.h"
#include "printitembase.h"
#include <QIcon>

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

}

PrintItemImage::PrintItemImage(MyField field)
    :PrintItemBase(field)
{
    captionSize->addItems(fontSize);
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
    captionLayout->addLayout(captionLayout2);
    captionBox->setLayout(captionLayout);

    valueLayout->addWidget(NewLine);
    valueLayout->addLayout(imageLayout);
    valueLayout->addWidget(captionBox);
    /*
    
    captionBox->setTitle("Caption");

    //The main layout for the value GroupBox is created. It is a QGridLayout
    QGridLayout valueLayout(valueBox);

    //The layout for the GroupBox containing the widgets for setting the
    //caption properties is created. It is a QGridLayout
    QGridLayout captionLayout(captionBox);

    QHBoxLayout captionStyleLayout(titleBox);

    

    //The buttons for determening the font style of the caption are created
    //and placed in a specific layout
    QFont btnFont=captionBold->font();
    btnFont.setBold(true);
    captionBold->setFont(btnFont);
    btnFont.setBold(false);
    btnFont.setUnderline(true);
    captionUnderlined->setFont(btnFont);
    btnFont.setUnderline(false);
    btnFont.setItalic(true);
    captionItalics->setFont(btnFont);
    captionBold->setText("B");
    captionBold->setCheckable(true);
    captionItalics->setText("I");
    captionItalics->setCheckable(true);
    captionUnderlined->setText("U");
    captionUnderlined->setCheckable(true);

    captionStyleLayout.addWidget(captionBold);
    captionStyleLayout.addWidget(captionUnderlined);
    captionStyleLayout.addWidget(captionItalics);



    //The whole layout is created by adding widgets and
    //layouts

    valueLayout.addWidget(NewLine,1,1);
    valueLayout.addWidget(lblImageSize,2,1);
    valueLayout.addWidget(imageSize,2,2);
    valueLayout.addWidget(lblWidth,2,3);
    valueLayout.addWidget(ledtImageWidth,2,4);
    valueLayout.addWidget(lblHeight,2,5);
    valueLayout.addWidget(ledtImageHeight,2,6);



    //The caption layout contains the widgets to set the values for the
    //caption font and position
    captionLayout.addWidget(lblCaptionPos,1,1);
    captionLayout.addWidget(cmbCaptionPos,1,2);
    captionLayout.addWidget(lblCaptionFont,1,3);
    captionLayout.addWidget(captionFontStyle,1,4);
    captionLayout.addWidget(lblCaptionSize,1,5);
    captionLayout.addWidget(captionSize,1,6);
    captionLayout.addLayout(&captionStyleLayout,2,1,1,2);
    captionLayout.addWidget(lblCaptionPre,2,3);
    captionLayout.addWidget(ledtCaptionPre,2,4);
    captionLayout.addWidget(capNumbered);

    valueLayout.addLayout(&captionLayout,3,1,1,6);

    valueBox->setLayout(&valueLayout);

    //CONNECTIONS
    connect(NewLine,SIGNAL(stateChanged()),this,SLOT(imagePrintChanged()));
    connect(imageSize,SIGNAL(currentIndexChanged()),this,SLOT(imagePrintChanged()));
    connect(ledtImageHeight,SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(ledtImageWidth,SIGNAL(editingFinished()),this,SLOT(imagePrintChanged()));
    connect(captionSize,SIGNAL(currentIndexChanged()),this,SLOT(captionPrintChanged()));
    connect(captionFontStyle,SIGNAL(currentFontChanged()),this,SLOT(captionPrintChanged()));
    connect(capNumbered,SIGNAL(stateChanged()),this,SLOT(captionPrintChanged()));

*/

}

void PrintItemImage::captionPrintChanged(){


}

void PrintItemImage::imagePrintChanged(){

}

