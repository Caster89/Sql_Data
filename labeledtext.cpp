#include "labeledtext.h"
#include "imageitemdelegate.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QFileDialog>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QtDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <mysqltablemodel.h>
#include <datatype.h>

LabeledText::LabeledText(QWidget *parent,DataType::dataType text_type,QString label) :
    QWidget(parent)
{

    QVector<QString> Images;
   // QGridLayout *grdLayout = new QGridLayout;
    mainLayout =new QBoxLayout(QBoxLayout::TopToBottom);
    txtType=text_type;
    txtlabel=new QLabel(this);

    txtlabel->setText(QString("%1: ").arg(label));
    Field_name=label;

    txtNonEdit = new QLabel(this);
//    grdLayout->addWidget(txtlabel,0,0,1,1);
    switch(txtType){
    case DataType::Text:{
        lnEdit=new QLineEdit(this);

        mainLayout->setDirection(QBoxLayout::LeftToRight);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(lnEdit);
        mainLayout->addWidget(txtNonEdit);
        txtNonEdit->setVisible(false);
    }
        break;
    case DataType::LongText :{
        txtEdit=new QTextEdit(this);
        txtEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        mainLayout->setDirection(QBoxLayout::TopToBottom);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(txtEdit);
        mainLayout->addWidget(txtNonEdit);
        txtNonEdit->setVisible(false);
        txtNonEdit->setWordWrap(true);
        txtNonEdit->hide();
    }
        break;
    case DataType::Image :{
        lblImage = new QLabel(this);
        lblImage->setFrameStyle(QFrame::Box);
        lblImage->setScaledContents(true);
        lblImage->setFixedSize(200,200);

        mainLayout->setDirection(QBoxLayout::TopToBottom);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(lblImage);
        btnAddImage = new QPushButton("Change");
        btnRemoveImage = new QPushButton("-");
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(btnAddImage);
        btnLayout->addWidget(btnRemoveImage);
        btnLayout->addStretch(1);
        mainLayout->addLayout(btnLayout);
        connect(btnAddImage,SIGNAL(clicked()),SLOT(ChangeImage()));
    }
        break;
    case DataType::Images:{
        imgEdit =new QListWidget(this);
        imgEdit->setViewMode(QListView::IconMode);
        imgEdit->setIconSize(QSize(200,200));
        imgEdit->setWrapping(false);
        imgEdit->setItemDelegate(new ImageItemDelegate(imgEdit));

        mainLayout->setDirection(QBoxLayout::TopToBottom);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(imgEdit);
        btnAddImage=new QPushButton("+");
        btnRemoveImage = new QPushButton("-");
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addStretch(1);
        btnLayout->addWidget(btnAddImage);
        btnLayout->addWidget(btnRemoveImage);
        mainLayout->addLayout(btnLayout);
        connect(btnAddImage,SIGNAL(clicked()),SLOT(addImage()));
    }
        break;
    }
/*
    if (txtType=="TEXT"){
        lnEdit=new QLineEdit(this);

        mainLayout->setDirection(QBoxLayout::LeftToRight);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(lnEdit);
        mainLayout->addWidget(txtNonEdit);
        txtNonEdit->setVisible(false);
    }else if(txtType=="LONG TEXT"){
        txtEdit=new QTextEdit(this);
        txtEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        mainLayout->setDirection(QBoxLayout::TopToBottom);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(txtEdit);
        mainLayout->addWidget(txtNonEdit);
        txtNonEdit->setVisible(false);
        txtNonEdit->setWordWrap(true);
        txtNonEdit->hide();
    }else if (txtType=="IMAGES"){

        imgEdit =new QListWidget(this);
        imgEdit->setViewMode(QListView::IconMode);
        imgEdit->setIconSize(QSize(200,200));
        imgEdit->setWrapping(false);
        imgEdit->setItemDelegate(new ImageItemDelegate(imgEdit));

        //grdLayout->addWidget(imgEdit,1,0,3,4);
        mainLayout->setDirection(QBoxLayout::TopToBottom);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(imgEdit);
        btnAddImage=new QPushButton("+");
        btnRemoveImage = new QPushButton("-");
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addStretch(1);
        btnLayout->addWidget(btnAddImage);
        btnLayout->addWidget(btnRemoveImage);
        //grdLayout->addLayout(btnLayout,4,3,1,1);
        mainLayout->addLayout(btnLayout);
        connect(btnAddImage,SIGNAL(clicked()),SLOT(addImage()));

    }else if (txtType=="IMAGE"){
        lblImage = new QLabel(this);
        lblImage->setFrameStyle(QFrame::Box);
        lblImage->setScaledContents(true);
        lblImage->setFixedSize(200,200);

        //grdLayout->addWidget(lblImage,1,0,3,2);
        mainLayout->setDirection(QBoxLayout::TopToBottom);
        mainLayout->addWidget(txtlabel);
        mainLayout->addWidget(lblImage);
        btnAddImage = new QPushButton("Change");
        btnRemoveImage = new QPushButton("-");
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(btnAddImage);
        btnLayout->addWidget(btnRemoveImage);
        btnLayout->addStretch(1);
        //grdLayout->addLayout(btnLayout,4,0,1,1);
        mainLayout->addLayout(btnLayout);
        connect(btnAddImage,SIGNAL(clicked()),SLOT(ChangeImage()));
    }
    */

    setLayout(mainLayout);


}

void LabeledText::setValue(QString newValue){
    Value=newValue;
    update();
}


void LabeledText::setField(QString Field){
    Field_name=Field;
}

void LabeledText::setTable(QString Table){
    Table_name=Table;
}

void LabeledText::setPrimary(int newPrimary){
    Primary=newPrimary;
}

QString LabeledText::getLabel(){
    return txtlabel->text();
}

QVariant LabeledText::getValue(){
    return Value;
}

QVector<QVector< QVector<QString> > > LabeledText::getImage(){
    QListWidgetItem *temp_item = new QListWidgetItem();
    QVector<QVector<QString> > temp_img(3);
    int numb_items = imgEdit->count();
    for(int i=0; i<imgEdit->count();i++){
        temp_item=imgEdit->item(i);
        temp_img[0].append("File");
        temp_img[0].append(temp_item->data(Qt::UserRole).toString());
        temp_img[1].append("Description");
        temp_img[1].append(temp_item->text());
        temp_img[2].append("Status");
        temp_img[2].append(temp_item->data(Qt::UserRole+1).toString());
        Images.append(temp_img);
    }
    return Images;
}

DataType::dataType LabeledText::getType(){
    return txtType;
}

QString LabeledText::getTable(){
    return Table_name;
}

int LabeledText::getPrimary(){
    return Primary;
}

QString LabeledText::getField(){
    return Field_name;
}

void LabeledText::addImage(){
    QStringList filenames=QFileDialog::getOpenFileNames(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");
    QMessageBox msgbx;
    msgbx.setText(QString("%1").arg(filenames.length()));
    msgbx.exec();

    for (int i=0;i<filenames.length();i++){
        QListWidgetItem *temp_item=new QListWidgetItem();
        QVector<QVector<QString> > temp_img(3);
        temp_item->setText("No description");
        temp_item->setIcon(QIcon(filenames[i]));
        temp_item->setFlags (temp_item->flags () | Qt::ItemIsEditable);
        temp_item->setData(Qt::UserRole,filenames[i]);
        temp_item->setData(Qt::UserRole+1,"N");
        imgEdit->addItem(temp_item);
//        temp_img[0].append("File");
//        temp_img[0].append(filenames[i]);
//        temp_img[1].append("Description");
//        temp_img[1].append("No description");
//        temp_img[2].append("Status");
//        temp_img[2].append("N");
//        Images.append(temp_img);
    }
}

void LabeledText::ChangeImage(){
    QString filename=QFileDialog::getOpenFileName(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");
    QMessageBox msgbx;

    if(!filename.isNull()){
        QListWidgetItem *temp_item=new QListWidgetItem();
        QPixmap new_image = QPixmap(filename);
        new_image.scaled(200,200);
        lblImage->setPixmap(QPixmap(filename));
        lblImage->setWhatsThis(filename);
        setValue(filename);
    }
}

void LabeledText::setEditable(bool editable)
{
    isEditable=editable;
    if(editable)
    {
        switch(txtType){
        case DataType::Text :
            txtNonEdit->setVisible(false);
            lnEdit->setVisible(true);
            break;
        case DataType::LongText:
            txtNonEdit->setVisible(false);
            txtEdit->setVisible(true);
            break;
        case DataType::Image:
            btnAddImage->show();
            btnRemoveImage->show();
            break;
        }
        /*
        if (txtType=="TEXT"){
            txtNonEdit->setVisible(false);
            lnEdit->setVisible(true);
        }else if(txtType=="LONG TEXT"){
            txtNonEdit->setVisible(false);
            txtEdit->setVisible(true);
        }else if (txtType=="IMAGE"){
            btnAddImage->show();
            btnRemoveImage->show();
        }*/
     }else{
        switch(txtType){
        case DataType::Text:
            txtNonEdit->setVisible(true);
            lnEdit->setVisible(false);
            mainLayout->addWidget(txtNonEdit);
            break;
        case DataType::LongText:
            txtNonEdit->setVisible(true);
            txtEdit->setVisible(false);
            break;
        case DataType::Image:
            btnAddImage->hide();
            btnRemoveImage->hide();
            break;
        }
/*
        if (txtType=="TEXT"){
            txtNonEdit->setVisible(true);
            lnEdit->setVisible(false);
            mainLayout->addWidget(txtNonEdit);
        }else if(txtType=="LONG TEXT"){
            txtNonEdit->setVisible(true);
            txtEdit->setVisible(false);
        }else if (txtType=="IMAGE" || txtType=="IMAGES"){
            btnAddImage->hide();
            btnRemoveImage->hide();
        }*/
    }
    update();
}

void LabeledText::update()
{
    if(isEditable)
    {
        switch(txtType){
        case DataType::Text:
            lnEdit->setText(Value);
            break;
        case DataType::LongText:
            txtEdit->setText(Value);
        }
/*
        if(txtType=="TEXT"){
            lnEdit->setText(Value);
        }else if(txtType=="LONG TEXT"){
            txtEdit->setText(Value);
        }*/

    }
    else{
        if(txtType==DataType::Text || txtType==DataType::LongText){

        txtNonEdit->setText(Value);
        }

    }
}

bool LabeledText::eventFilter(QObject *o, QEvent *e)
{
    //event->accept();
    //qDebug()<<"doubleclick";
    //setEditable(!isEditable);
    if(e->type()==QEvent::MouseButtonDblClick)
    {
        QMessageBox *msgBox=new QMessageBox();
        msgBox->setText("double click");
        msgBox->show();
        return true;
    }
    return false;

}
