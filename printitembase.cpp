#include "printitembase.h"
#include "myfield.h"
#include <QFontComboBox>
#include <QComboBox>
#include <QCheckBox>
#include <QToolButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QEvent>
#include <QFont>

PrintItemBase::PrintItemBase(QWidget *parent)
    :DraggableWidget(parent)
{
    //buildWidget();
}

PrintItemBase::PrintItemBase(MyField newField, QWidget *parent)
    :DraggableWidget(parent)
{
    field = newField;
    lblTitle->setText(field.getName());
    buildWidget();
}

void PrintItemBase::buildWidget(){

    //field=newField;
    //Set the available font sizes
    fontSize<<"8"<<"9"<<"10"<<"11"<<"12"<<"14"<<"16"<<"18"<<"20"<<"22"<<"24"<<"26"<<"28"<<"32"<<"36"<<"38";
    titleSize->addItems(fontSize);

    //Populate the alignment map
    alignMap->insert(0,Qt::AlignLeft);
    alignMap->insert(1,Qt::AlignRight);
    alignMap->insert(2,Qt::AlignCenter);
    alignMap->insert(3,Qt::AlignJustify);

    //Set the style of the frame
//setFrameShadow(QFrame::Raised);
//setFrameShape(QFrame::StyledPanel);

    //edit layout is the main layout where everything else is incorporated

    editLayout->setContentsMargins(1,1,1,1);
    editLayout->setMargin(1);
    editLayout->setSpacing(1);

    //By defult the field name is visible
    titleVisible->setChecked(true);


    //Two horizontal layouts are created, one for the fontstyle and one for the
    //weights B U I
    QHBoxLayout *hTitleLayoutFont = new QHBoxLayout();
    QHBoxLayout *fontStyleLayout=new QHBoxLayout();

    //A basic layout is created to for the arrow and title used
    //to expand and contract the widget
    // _______________________________________
    //|_|>_________________________________X__|
/*
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
*/
    //The titleBox and Value box are the two groupboxed where the configuration
    //for the setup of the Field name and it's value are placed
    titleBox->setTitle("Field Name");
    valueBox->setTitle("Field Value");




    //Creating the Title Box
    //The buttons for setting B U I are created
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

    titleLayout->addWidget(titleVisible);
    titleLayout->addLayout(hTitleLayoutFont);
    titleLayout->addLayout(fontStyleLayout);


    //The layout is set for the titlebox
    titleBox->setLayout(titleLayout);


    titleBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    valueBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    connect(titleVisible,SIGNAL(stateChanged(int)),this,SLOT(titlePrintChanged()));
    connect(titleFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(titleFontChanged()));
    connect(titleSize,SIGNAL(currentIndexChanged(int)),this,SLOT(titleFontChanged()));
    connect(titleBold,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleItalics,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleUnderlined,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleAlignGroup, SIGNAL(buttonClicked(int)),this,SLOT(titleAlignmentChanged(int)));
    connect(previousLine, SIGNAL(stateChanged(int)), this, SLOT(positioningChanged()));

    //The title box and value box are added to the boxLayout

    boxLayout->addWidget(previousLine);
    boxLayout->addSpacing(10);
    boxLayout->addWidget(titleBox);
    boxLayout->addSpacing(10);
    boxLayout->addWidget(valueBox);

    QFrame *lineFrame=new QFrame();
    lineFrame->setFrameShape(QFrame::HLine);
    //The editLayout is populated
//editLayout->addWidget(lineFrame);
//editLayout->addLayout(arrowLayout);
//editLayout->addLayout(boxLayout);
//setLayout(editLayout);
    setMainLayout(boxLayout);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);



    //editWidget->installEventFilter(this);
    titleFont=titleFontStyle->currentFont();
    titleFont.setPointSize(titleSize->currentText().toInt());

    setFixedWidth(320);
}

PrintItemBase::PrintItemBase(const PrintItemBase &orgItem){
    /*editWidget=orgItem.getWidget();
    titleBox=orgItem.titleBox;
    field=orgItem.field;*/
}

void PrintItemBase::positioningChanged(){
    emit itemModified();
}

void PrintItemBase::titleAlignmentChanged(int align){
    qDebug()<<"Changing Title Alignment to:"<<align<<"==>"<<alignMap->value(align);
    titleAlignment = alignMap->value(align);
    emit itemModified();
}

PrintItemBase& PrintItemBase::operator =(const PrintItemBase &orgItem){
    PrintItemBase retItem(orgItem);
    return retItem;

}

void PrintItemBase::titlePrintChanged(){

    if(titleVisible->isChecked()){
        showTitle=true;
        titleFontStyle->setDisabled(false);
        lblTitleFont->setDisabled(false);
        titleSize->setDisabled(false);
        lblTitleSize->setDisabled(false);
        titleBold->setDisabled(false);
        titleItalics->setDisabled(false);
        titleUnderlined->setDisabled(false);
    }else{
        showTitle=false;
        titleFontStyle->setDisabled(true);
        lblTitleFont->setDisabled(true);
        titleSize->setDisabled(true);
        lblTitleSize->setDisabled(true);
        titleBold->setDisabled(true);
        titleItalics->setDisabled(true);
        titleUnderlined->setDisabled(true);
    }
    emit itemModified();
    
}

void PrintItemBase::titleFontChanged(){


    titleFont=titleFontStyle->currentFont();
    titleFont.setPointSize(titleSize->currentText().toInt());
    titleFont.setBold(titleBold->isChecked());
    titleFont.setItalic(titleItalics->isChecked());
    titleFont.setUnderline(titleUnderlined->isChecked());
    emit itemModified();

}

/*
bool PrintItemBase::eventFilter(QObject *obj, QEvent *event){

    if(obj==lblArrow && event->type()==QEvent::MouseButtonRelease){
        if(expanded ){
            for(int i=0;i<boxLayout->count();i++){
                boxLayout->itemAt(i)->widget()->setVisible(false);
                lblArrow->setText(rightTri);
                expanded=false;
            }

        }else if(!expanded){
            for(int i=0;i<boxLayout->count();i++){
                boxLayout->itemAt(i)->widget()->setVisible(true);
                lblArrow->setText(downTri);
                expanded=true;
            }
        }

        return true;
    }else if (obj==lblClose && event->type()==QEvent::MouseButtonRelease){
        emit closeWidget();
    }

    return QWidget::eventFilter(obj,event);
}

void PrintItemBase::enterEvent(QEvent *event){
    lblClose->setVisible(true);
    QWidget::enterEvent(event);
}

void PrintItemBase::leaveEvent(QEvent *event){
    lblClose->setVisible(false);
    QWidget::leaveEvent(event);
}
*/
QSize PrintItemBase::sizeHint(){
    QSize retSize=QWidget::sizeHint();
    retSize.setWidth(qMax(valueBox->width()+titleBox->width()+10, this->parentWidget()->width()));
    qDebug()<<"Calling printitem sinze hint, returning: "<<retSize;
    return retSize;
}

