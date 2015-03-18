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

PrintItemBase::PrintItemBase()
    :QFrame()
{
    /*lblTitle->setText("");
    fontSize<<"8"<<"9"<<"10"<<"11"<<"12"<<"14"<<"16"<<"18"<<"20"<<"22"<<"24"<<"26"<<"28"<<"32"<<"36"<<"38";
    titleSize->addItems(fontSize);*/
    buildWidget();

}

PrintItemBase::PrintItemBase(MyField field)
    :QFrame()
{
    fontSize<<"8"<<"9"<<"10"<<"11"<<"12"<<"14"<<"16"<<"18"<<"20"<<"22"<<"24"<<"26"<<"28"<<"32"<<"36"<<"38";

    titleSize->addItems(fontSize);

    lblTitle->setText(field.getName());

/*
    type=field.getType();
*/
    buildWidget();


}

void PrintItemBase::buildWidget(){

    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::StyledPanel);

    editLayout->setContentsMargins(1,1,1,1);
    editLayout->setMargin(1);
    editLayout->setSpacing(1);

    titleVisible->setChecked(true);


    //Two horizontal layouts are created, one for the fontstyle and one for the
    //weights B U I

    QHBoxLayout *hTitleLayoutFont = new QHBoxLayout();
    QHBoxLayout *fontStyleLayout=new QHBoxLayout();

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
    arrowLayout->setSpacing(1);
    arrowLayout->setContentsMargins(1,1,1,1);
    arrowLayout->setSizeConstraint(QLayout::SetMaximumSize);




    titleBox->setTitle("Field Name");
    valueBox->setTitle("Field Value");





    //The buttons for setting B U I are created

    titleBold->setText("B");
    titleBold->setCheckable(true);
    titleItalics->setText("I");
    titleItalics->setCheckable(true);
    titleUnderlined->setText("U");
    titleUnderlined->setCheckable(true);

    QFont btnFont=titleBold->font();
    btnFont.setBold(true);
    titleBold->setFont(btnFont);
    btnFont.setBold(false);
    btnFont.setUnderline(true);
    titleUnderlined->setFont(btnFont);
    btnFont.setUnderline(false);
    btnFont.setItalic(true);
    titleItalics->setFont(btnFont);


    //The Widgets are placed in the respetive widgets

    hTitleLayoutFont->addWidget(lblTitleFont);
    hTitleLayoutFont->addWidget(titleFontStyle);
    hTitleLayoutFont->addWidget(lblTitleSize);
    hTitleLayoutFont->addWidget(titleSize);
    hTitleLayoutFont->addStretch(0);

    fontStyleLayout->addWidget(titleBold);
    fontStyleLayout->addWidget(titleItalics);
    fontStyleLayout->addWidget(titleUnderlined);
    fontStyleLayout->addStretch(0);

    titleLayout->addWidget(titleVisible);
    titleLayout->addLayout(hTitleLayoutFont);
    titleLayout->addLayout(fontStyleLayout);


    //The layout is set for the titlebox
    titleBox->setLayout(titleLayout);


    //The title box and value box are added to the boxLayout


    QFrame *lineFrame=new QFrame();
    lineFrame->setFrameShape(QFrame::HLine);

    editLayout->addWidget(lineFrame);
    editLayout->addLayout(arrowLayout);
    setLayout(editLayout);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);


    titleBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    valueBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    connect(titleVisible,SIGNAL(stateChanged(int)),this,SLOT(titlePrintChanged()));
    connect(titleFontStyle,SIGNAL(currentFontChanged(QFont)),this,SLOT(titleFontChanged()));
    connect(titleSize,SIGNAL(currentIndexChanged(int)),this,SLOT(titleFontChanged()));
    connect(titleBold,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleItalics,SIGNAL(clicked()),this,SLOT(titleFontChanged()));
    connect(titleUnderlined,SIGNAL(clicked()),this,SLOT(titleFontChanged()));


    //The 2Layouts are added to the expanded widget


    boxLayout->addWidget(titleBox);
    boxLayout->addWidget(valueBox);
    editLayout->addLayout(boxLayout);

    //editWidget->installEventFilter(this);




}

PrintItemBase::PrintItemBase(const PrintItemBase &orgItem){
    /*editWidget=orgItem.getWidget();
    titleBox=orgItem.titleBox;
    field=orgItem.field;*/
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

    
}

void PrintItemBase::titleFontChanged(){

    titleFont=titleFontStyle->currentFont();
    titleFont.setPointSize(titleSize->currentText().toInt());
    titleFont.setBold(titleBold->isChecked());
    titleFont.setItalic(titleItalics->isChecked());
    titleFont.setUnderline(titleUnderlined->isChecked());

}


bool PrintItemBase::eventFilter(QObject *obj, QEvent *event){
    if(obj==lblArrow && event->type()==QEvent::MouseButtonRelease){
        if(expanded ){
            for(int i=0;i<boxLayout->count();i++){
                boxLayout->itemAt(i)->widget()->setVisible(false);
                lblArrow->setText("u");
                expanded=false;
            }

        }else if(!expanded){
            for(int i=0;i<boxLayout->count();i++){
                boxLayout->itemAt(i)->widget()->setVisible(true);
                lblArrow->setText("q");
                expanded=true;
            }
        }
        return true;
    }
    /*if(obj==editWidget && event->type()==QEvent::Enter){
        qDebug()<<"Enter";
    }else if (obj==editWidget && event->type()==QEvent::Leave){
        qDebug()<<"Leave";
    }*/
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

QSize PrintItemBase::sizeHint(){
    QSize retSize=QWidget::sizeHint();
    retSize.setWidth(valueBox->width()+titleBox->width()+10);
}

