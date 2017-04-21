#include "printitemcombined.h"
#include <QDebug>
#include <QPalette>

PrintItemCombined::PrintItemCombined()
{

}

PrintItemCombined::PrintItemCombined(MyField field, QWidget *parent)
{
    lblTitle->setText("Combined");
    avlbFields = QStringList(field.getTable().split(";"));
    baseCMB->addItems(avlbFields);
    buildWidget();
}

void PrintItemCombined::buildWidget(){
    fontSize<<"8"<<"9"<<"10"<<"11"<<"12"<<"14"<<"16"<<"18"<<"20"<<"22"<<"24"<<"26"<<"28"<<"32"<<"36"<<"38";
    titleSize->addItems(fontSize);
    valueSize->addItems(fontSize);
    alignMap->insert(0,Qt::AlignLeft);
    alignMap->insert(1,Qt::AlignRight);
    alignMap->insert(2,Qt::AlignCenter);
    alignMap->insert(3,Qt::AlignJustify);

    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::StyledPanel);


    editLayout->setContentsMargins(1,1,1,1);
    editLayout->setMargin(1);
    editLayout->setSpacing(1);

    titleVisible->setChecked(true);


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


    //The static text uses the title widgets to set the text details
    //The buttons for setting B U I are created
    //titleBox->setTitle("Static Text");
    QHBoxLayout *TitleNameLayout = new QHBoxLayout();
    QHBoxLayout *hTitleLayoutFont = new QHBoxLayout();
    QHBoxLayout *fontStyleLayout=new QHBoxLayout();
    TitleNameLayout->addWidget(lblTitleName);
    TitleNameLayout->addWidget(ledtTitleName);

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

    //titleLayout->addWidget(txtedtStaticText);
    titleLayout->addWidget(titleVisible);
    titleLayout->addLayout(TitleNameLayout);
    titleLayout->addLayout(hTitleLayoutFont);
    titleLayout->addLayout(fontStyleLayout);

    //The layout is set for the titlebox
    titleBox->setLayout(titleLayout);

    //NewLine->setChecked(true);

    //Two Layout are created like for the valuelayout
    QHBoxLayout *valueFontLayout = new QHBoxLayout();
    QHBoxLayout *valueFontStyleLayout=new QHBoxLayout();

    valueBold->setText("B");
    valueBold->setCheckable(true);
    btnFont=valueBold->font();
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
    /*
    valueFontLayout->addWidget(lblValueFont);
    valueFontLayout->addWidget(valueFontStyle);
    valueFontLayout->addWidget(lblValueSize);
    valueFontLayout->addWidget(valueSize);
    valueFontLayout->addStretch(0);
    */
    //valueFontLayout->addWidget(lblValueFont);
    //valueFontLayout->addWidget(lblValueSize);
    //QLabel *label = new QLabel("Testlabel");
    valueFontLayout->addWidget(lblValueFont);
    valueFontLayout->addWidget(valueFontStyle);
    valueFontLayout->addWidget(lblValueSize);
    valueFontLayout->addWidget(valueSize);
    //valueFontLayout->addStretch(0);

    valueFontStyleLayout->addWidget(valueBold);
    valueFontStyleLayout->addWidget(valueUnderlined);
    valueFontStyleLayout->addWidget(valueItalics);
    valueFontStyleLayout->addSpacing(40);
    valueFontStyleLayout->addWidget(valueLeft);
    valueFontStyleLayout->addWidget(valueCenter);
    valueFontStyleLayout->addWidget(valueRight);
    valueFontStyleLayout->addWidget(valueJustify);
    valueFontStyleLayout->addStretch(0);

    QHBoxLayout *addRemoveLayout = new QHBoxLayout();
    //addRemoveLayout->addStretch(0);
    addRemoveLayout->addWidget(btnAddField);
    addRemoveLayout->addWidget(btnRemoveField);
    addRemoveLayout->addStretch(0);


    scrollWidget->setBackgroundRole(QPalette::Dark);
    scrollWidget->setFrameShadow(QFrame::Sunken);
    listWidget->setLayout(scrollLayout);
    listWidget->layout()->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    listWidget->setContentsMargins(0,0,0,0);
    listWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    //scrollLayout->addWidget(listWidget);

    scrollWidget->setWidget(listWidget);
    scrollWidget->setWidgetResizable(true);
    scrollWidget->setMinimumHeight(100);

    valueLayout->addWidget(scrollWidget);
    valueLayout->addLayout(addRemoveLayout);
    valueLayout->addLayout(valueFontLayout);
    valueLayout->addLayout(valueFontStyleLayout);

    valueBox->setLayout(valueLayout);
    //mainWidget->addWidget(titleBox);
    //mainWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    //mainWidget->setFixedHeight(mainWidget->currentWidget()->sizeHint().height());

/*
    connect(titleVisible,SIGNAL(stateChanged(int)),this,SLOT(titlePrintChanged()));
    connect(titleFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(titleFontChanged()));
    connect(titleSize,SIGNAL(currentIndexChanged(int)),this,SLOT(titleFontChanged()));
    connect(titleBold,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleItalics,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleUnderlined,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleAlignGroup, SIGNAL(buttonClicked(int)),this,SLOT(titleAlignmentChanged(int)));
    //connect(cmbStaticElmt, SIGNAL(currentIndexChanged(int)), this, SLOT(staticElementChange(int)));
*/
    //The Stacked widget and combobox are added to the boxLayout
    boxLayout->addWidget(previousLine);
    boxLayout->addWidget(titleBox);
    boxLayout->addWidget(valueBox);
    //boxLayout->addWidget(cmbStaticElmt);
    //boxLayout->addWidget(mainWidget);


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

    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());


    setLayout(editLayout);
    setFixedWidth(320);

    connect(btnAddField, SIGNAL(clicked(bool)), this, SLOT(addField()));
    connect(btnRemoveField, SIGNAL(clicked(bool)), this, SLOT(removeField()));
    connect(NewLine,SIGNAL(stateChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(valueFontChanged()));
    connect(valueSize,SIGNAL(currentIndexChanged(int)),this,SLOT(valueFontChanged()));
    connect(valueBold,SIGNAL(clicked()),this,SLOT(valueFontChanged()));
    connect(valueItalics,SIGNAL(clicked()),this,SLOT(valueFontChanged()));
    connect(valueUnderlined,SIGNAL(clicked()),this,SLOT(valueFontChanged()));
    connect(alignGroup, SIGNAL(buttonClicked(int)),this,SLOT(alignmentChanged(int)));
    connect(ledtTitleName, SIGNAL(textChanged(QString)), this, SIGNAL(itemModified()));
    connect(titleVisible,SIGNAL(stateChanged(int)),this,SLOT(titlePrintChanged()));
    connect(titleFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(titleFontChanged()));
    connect(titleSize,SIGNAL(currentIndexChanged(int)),this,SLOT(titleFontChanged()));
    connect(titleBold,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleItalics,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleUnderlined,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleAlignGroup, SIGNAL(buttonClicked(int)),this,SLOT(titleAlignmentChanged(int)));
    connect(previousLine, SIGNAL(stateChanged(int)), this, SLOT(positioningChanged()));
}

void PrintItemCombined::valueFontChanged(){
    valueFont=valueFontStyle->currentFont();
    valueFont.setPointSize(valueSize->currentText().toInt());
    valueFont.setBold(valueBold->isChecked());
    valueFont.setItalic(valueItalics->isChecked());
    valueFont.setUnderline(valueUnderlined->isChecked());
    valueNewLine=NewLine->isChecked();
    emit itemModified();
}

void PrintItemCombined::alignmentChanged(int align){

    alignment = alignMap->value(align);
    emit itemModified();
}

void PrintItemCombined::addField(){

    selectedFields.append(new QComboBox());
    selectedFields.last()->addItems(avlbFields);
    selectedFields.last()->setFixedWidth(100);

    if (selectedFields.count()>1){
        separators.append(new QLineEdit());
        scrollLayout->addWidget(separators.last());
        separators.last()->setFixedWidth(100);
        connect(separators.last(), SIGNAL(textChanged(QString)), this, SIGNAL(itemModified()));
    }
    scrollLayout->addWidget(selectedFields.last());
    connect(selectedFields.last(), SIGNAL(currentIndexChanged(int)), this, SIGNAL(itemModified()));
    emit itemModified();
}

void PrintItemCombined::removeField(){
    if (selectedFields.count()>1){
        separators.last()->setParent(NULL);
        separators.removeLast();
    }
    selectedFields.last()->setParent(NULL);
    selectedFields.removeLast();
    emit itemModified();
}

void PrintItemCombined::paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){
    //QTextCursor cursor(doc);
    QTextFrame *topFrame =cursor->currentFrame();
    QTextFrameFormat frameFormat=topFrame->frameFormat();
    cursor->movePosition(QTextCursor::End);
    if (previousLine->isChecked()){
        cursor->movePosition(QTextCursor::PreviousBlock);
        cursor->movePosition(QTextCursor::EndOfBlock);
        cursor->insertText(" ");
    }else{
        QTextFrame *frame=cursor->insertFrame(frameFormat);
    }

    QTextBlockFormat blockFormat = cursor->blockFormat();

    if(titleVisible->isChecked()){
        blockFormat.setAlignment(titleAlignment);
        cursor->setBlockFormat(blockFormat);
        QTextCharFormat titleFormat;
        titleFormat.setFont(titleFont);
        qDebug()<<titleFormat;
        cursor->insertText(QString("%1: ").arg(ledtTitleName->text()),titleFormat);
        //cursor->insertText("Tomorrow ",titleFormat);
        if(valueNewLine){
            cursor->insertBlock();
            blockFormat.setAlignment(alignment);
            cursor->setBlockFormat(blockFormat);
        }
    }

    QTextCharFormat valueFormat;
    valueFormat.setFont(valueFont);

    for (int i =0; i <selectedFields.count(); i++){
        if (i>0){
            cursor->insertText(separators[(i-1)]->text(),valueFormat);
        }
        cursor->insertText(record->value(selectedFields[i]->currentText()).toString(),valueFormat);

    }


}
