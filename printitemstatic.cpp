#include "printitemstatic.h"
#include <QLineEdit>
#include <QDebug>

PrintItemStatic::PrintItemStatic(MyField newField, QWidget *parent)
    :PrintItemBase(parent)
{
    lblTitle->setText("Static Element");
    buildWidget();
    delete valueBox;
}

void PrintItemStatic::buildWidget(){

    fontSize<<"8"<<"9"<<"10"<<"11"<<"12"<<"14"<<"16"<<"18"<<"20"<<"22"<<"24"<<"26"<<"28"<<"32"<<"36"<<"38";
    titleSize->addItems(fontSize);

    alignMap->insert(0,Qt::AlignLeft);
    alignMap->insert(1,Qt::AlignRight);
    alignMap->insert(2,Qt::AlignCenter);
    alignMap->insert(3,Qt::AlignJustify);

    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::StyledPanel);

    cmbStaticElmt->addItem("Page Break");
    cmbStaticElmt->addItem("New Line");
    cmbStaticElmt->addItem("Static Text");

    editLayout->setContentsMargins(1,1,1,1);
    editLayout->setMargin(1);
    editLayout->setSpacing(1);

    //titleVisible->setChecked(true);


    //A basic layout is created to for the arrow and title used
    //to expand and contract the widget
    QHBoxLayout *arrowLayout=new QHBoxLayout();
    QFont arrowFont("Wingdings 3");
    lblArrow->setFont(arrowFont);
    lblArrow->installEventFilter(this);

    lblArrow->setMargin(1);
    arrowLayout->addWidget(lblArrow);
    arrowLayout->addWidget(lblTitle);
    arrowLayout->addStretch(0);
    arrowLayout->addWidget((lblClose));
    lblClose->setVisible(false);
    lblClose->installEventFilter(this);
    arrowLayout->setSpacing(1);
    arrowLayout->setContentsMargins(1,1,1,1);
    arrowLayout->setSizeConstraint(QLayout::SetMaximumSize);

    //The new page widget is empty
    mainWidget->addWidget(wdgNewPage);

    //The new line widget asks how many lines to enter
    QHBoxLayout *newLineLayout = new QHBoxLayout();
    QLabel *lblNewLine = new QLabel("Number of empty lines:");

    newLineLayout->addWidget(lblNewLine);
    newLineLayout->addWidget(ledtNewLine);
    newLineLayout->addStretch(0);
    wdgNewLine->setLayout(newLineLayout);
    mainWidget->addWidget(wdgNewLine);

    //The static text uses the title widgets to set the text details
    //The buttons for setting B U I are created
    //titleBox->setTitle("Static Text");

    QHBoxLayout *hTitleLayoutFont = new QHBoxLayout();
    QHBoxLayout *fontStyleLayout=new QHBoxLayout();
    titleVisible->setVisible(false);
    titleBold->setText("B");
    QFont btnFont=titleBold->font();
    btnFont.setBold(true);
    titleBold->setFont(btnFont);
    titleBold->setCheckable(true);

    titleUnderlined->setText("U");
    btnFont.setBold(false);
    btnFont.setUnderline(true);
    titleUnderlined->setFont(btnFont);
    titleUnderlined->setCheckable(true);

    titleItalics->setText("I");
    titleItalics->setCheckable(true);
    btnFont.setUnderline(false);
    btnFont.setItalic(true);
    titleItalics->setFont(btnFont);

    titleLeft->setIcon(QIcon(":/Align_Left"));
    titleLeft->setCheckable(true);
    titleLeft->setChecked(true);
    titleAlignGroup->addButton(titleLeft,alignMap->key(Qt::AlignLeft));

    titleCenter->setIcon(QIcon(":/Align_Center"));
    titleCenter->setCheckable(true);
    titleAlignGroup->addButton(titleCenter,alignMap->key(Qt::AlignCenter));

    titleRight->setIcon(QIcon(":/Align_Right"));
    titleRight->setCheckable(true);
    titleAlignGroup->addButton(titleRight, alignMap->key(Qt::AlignRight));

    titleJustify->setIcon(QIcon(":/Align_Justify"));
    titleJustify->setCheckable(true);
    titleAlignGroup->addButton(titleJustify, alignMap->key(Qt::AlignJustify));

    //The Widgets are placed in the respetive widgets

    hTitleLayoutFont->addWidget(lblTitleFont);
    hTitleLayoutFont->addWidget(titleFontStyle);
    hTitleLayoutFont->addWidget(lblTitleSize);
    hTitleLayoutFont->addWidget(titleSize);
    hTitleLayoutFont->addStretch(0);

    fontStyleLayout->addWidget(titleBold);
    fontStyleLayout->addWidget(titleItalics);
    fontStyleLayout->addWidget(titleUnderlined);
    fontStyleLayout->addSpacing(40);
    fontStyleLayout->addWidget(titleLeft);
    fontStyleLayout->addWidget(titleCenter);
    fontStyleLayout->addWidget(titleRight);
    fontStyleLayout->addWidget(titleJustify);
    fontStyleLayout->addStretch(0);

    titleLayout->addWidget(txtedtStaticText);
    titleLayout->addLayout(hTitleLayoutFont);
    titleLayout->addLayout(fontStyleLayout);


    //The layout is set for the titlebox
    titleBox->setLayout(titleLayout);
    mainWidget->addWidget(titleBox);
    mainWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    mainWidget->setFixedHeight(mainWidget->currentWidget()->sizeHint().height());


    connect(titleVisible,SIGNAL(stateChanged(int)),this,SLOT(titlePrintChanged()));
    connect(titleFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(titleFontChanged()));
    connect(titleSize,SIGNAL(currentIndexChanged(int)),this,SLOT(titleFontChanged()));
    connect(titleBold,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleItalics,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleUnderlined,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleAlignGroup, SIGNAL(buttonClicked(int)),this,SLOT(titleAlignmentChanged(int)));
    connect(cmbStaticElmt, SIGNAL(currentIndexChanged(int)), this, SLOT(staticElementChange(int)));

    //The Stacked widget and combobox are added to the boxLayout
    boxLayout->addWidget(cmbStaticElmt);
    boxLayout->addWidget(mainWidget);


    QFrame *lineFrame=new QFrame();
    lineFrame->setFrameShape(QFrame::HLine);
    //The editLayout is populated
    editLayout->addWidget(lineFrame);
    editLayout->addLayout(arrowLayout);
    editLayout->addLayout(boxLayout);
    setLayout(editLayout);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);


    //editWidget->installEventFilter(this);
    titleFont=titleFontStyle->currentFont();
    titleFont.setPointSize(titleSize->currentText().toInt());

    setFixedWidth(320);
}

void PrintItemStatic::staticElementChange(int currElmt){
    mainWidget->setCurrentIndex(currElmt);
    //mainWidget->resize(mainWidget->widget(currElmt)->size());
    mainWidget->setFixedHeight(mainWidget->currentWidget()->sizeHint().height());

}

void PrintItemStatic::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    cursor->movePosition(QTextCursor::End);

    switch (cmbStaticElmt->currentIndex()) {
    case 0:
        frameFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
        cursor->insertFrame(frameFormat);
        break;
    case 1:
        for(int i=0;i<ledtNewLine->text().toInt();i++){
            //QTextBlockFormat blockFormat = cursor->blockFormat();
            cursor->insertBlock();
            cursor->movePosition(QTextCursor::End);
        }
        break;
    case 2:
    {
        QTextCharFormat textFormat;
        textFormat.setFont(titleFont);
        cursor->insertText(txtedtStaticText->toPlainText(),textFormat);
    }
    default:
        break;
    }
}





