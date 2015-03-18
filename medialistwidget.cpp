#include "medialistwidget.h"
#include <QListWidget>
#include <QIcon>
#include <QEvent>
#include <QWidget>
#include <QPushButton>
#include <QPalette>
#include <QRegion>
#include <QColor>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QSqlRecord>
#include <QSqlField>
//#include "imagewidgetdelegate.h"
#include <QModelIndex>
#include <QMouseEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QVariant>
#include "ImageWidgetDelegate.h"
#include <QVector>


MediaListWidget::MediaListWidget(QWidget *parent)
    :QListWidget(parent)
{
    setMouseTracking(true);
    //***********************************************************//
    //The button used to add items to the ListWidget is created,//
    //it is placed in a new Qwidget with a vertical box layout  //
    //and the widget is then palced in the created item.        //
    //The Button is created by using a custom stylesheet, using //
    //the option border radius, equal to half the side of the   //
    //QPushButton and by adding an inset border when the button //
    //is pressed. The plus sign is added as a big bold plus text//
    //The addBtnItem is a variable of the class, and can thus be//
    //removed and added to the QListWidget without problems.    //
    //When the button is clicked the signal is connected to the //
    //addNewItem slot which occupied itself to optain a list if //
    //files to add and emit the updated images
    //**********************************************************//
    addBtnItem = new QListWidgetItem();
    addItem(addBtnItem);

    QWidget *addBtnWidget = new QWidget();

    int pushButtonSide=60;
    int crossWidth=10;
    int crossLength=pushButtonSide-2*crossWidth;

    QPushButton *pushButton=new QPushButton(addBtnWidget);
    QIcon btnIcon(":/Add_Round_Btn.png");
    pushButton->setFixedHeight(pushButtonSide);
    pushButton->setFixedWidth(pushButtonSide);
    //pushButton->setText("+");
    pushButton->setIcon(btnIcon);
    pushButton->setIconSize(QSize(pushButtonSide,pushButtonSide));
    pushButton->setStyleSheet("QPushButton {border-radius: 30px; border-style: outset;"
                                  "border-width: 2px;border-color: white; font:bold 50px; text-align: center} "
                                  "QPushButton:pressed {border-style: inset; border-color: grey}");
        /*QRegion* complete=new QRegion(*(new QRect(0,0,pushButtonSide,pushButtonSide)));
    /*pushButton->setStyleSheet("QPushButton {background-color: qradialgradient(x: 0.2, y: 0.2,radius:5,fx:0.5,fy:0.5 "
                              "stop: 0 #008F00, stop: 0.2 #00FF00);border-radius: 30px; border-style: outset;"
                              "border-width: 2px;border-color: white; font:bold 50px; text-align: center} "
                              "QPushButton:pressed {border-style: inset; border-color: grey}");
    /*QRegion* complete=new QRegion(*(new QRect(0,0,pushButtonSide,pushButtonSide)));*/
    //QRegion* q1=new QRegion(*(new QRect(pushButtonSide/2-crossWidth/2,crossWidth,crossWidth,crossLength)));
    //QRegion* q2=new QRegion(*(new QRect(crossWidth,pushButtonSide/2-crossWidth/2,crossLength,crossWidth)));
    //complete-=*q1;
    //complete-=*q2;
    //pushButton->setMask(*complete);*/


    pushButton->update();
    pushButton->setCursor(Qt::PointingHandCursor);
    pushButton->show();

    QVBoxLayout *addLayout= new QVBoxLayout();
    addLayout->setAlignment(Qt::AlignCenter);
    addLayout->addStretch();
    addLayout->addWidget(pushButton);

    addBtnWidget->setLayout(addLayout);
    addBtnItem->setSizeHint(addBtnWidget->sizeHint());
    setItemWidget(addBtnItem, addBtnWidget);
    connect(pushButton, SIGNAL(clicked()), this, SLOT(addNewItem()));

}

/*void MediaListWidget::setItemDelegate(QAbstractItemDelegate *delegate)
{

    if (delegate == itemDelegate())
        return;

    if (itemDelegate()) {
            disconnect(itemDelegate(), SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),
                       this, SLOT(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)));
            disconnect(itemDelegate(), SIGNAL(commitData(QWidget*)), this, SLOT(commitData(QWidget*)));
            disconnect(itemDelegate(), SIGNAL(sizeHintChanged(QModelIndex)), this, SLOT(doItemsLayout()));
            if(typeid(itemDelegate()).name()==)
    }

    if (delegate) {
        if (d->delegateRefCount(delegate) == 0) {
            connect(delegate, SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),
                    this, SLOT(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)));
            connect(delegate, SIGNAL(commitData(QWidget*)), this, SLOT(commitData(QWidget*)));
            qRegisterMetaType<QModelIndex>("QModelIndex");
            connect(delegate, SIGNAL(sizeHintChanged(QModelIndex)), this, SLOT(doItemsLayout()), Qt::QueuedConnection);
        }
    }
    d->itemDelegate = delegate;
    viewport()->update();
}*/

void MediaListWidget::addItem(QListWidgetItem *newitem){
    int numItems=count();
    insertItem(numItems-1,newitem);

}

void MediaListWidget::insertItem(int row, QListWidgetItem *item){
    int numItems=count();
    if(row==numItems)
        --row;
    return QListWidget::insertItem(row,item);
}

void MediaListWidget::clear(){
    QListWidget::clear();
    addItem(addBtnItem);
}

void MediaListWidget::commitData(QWidget *e){
    qDebug()<<"Data Committed \n";
    ImageWidgetDelegate *editor = dynamic_cast<ImageWidgetDelegate*>(e);
    QListWidgetItem *currItem= currentItem();
    QSqlRecord modRecord=editor->getValue();
    qDebug()<<"Status is set to: "<<modRecord.value("Status");
    if(modRecord.value("Status").toString()!="O"){
        QSqlRecord prova=qvariant_cast<QSqlRecord>(currItem->data(Qt::UserRole));
        qDebug()<<"Olde Description was: "<<prova.value("Description");
        qDebug()<<"New Description is: "<<modRecord.value("Description");
        currItem->setData(Qt::UserRole,QVariant::fromValue(modRecord));
        if(modRecord.value("Status").toString()=="R")
            currItem->setHidden(true);
    }
    emit updatedImages();
}

QVector<QSqlRecord> MediaListWidget::getRecords(){
    QVector<QSqlRecord> outVect;
    int numbRecord=count()-1;
    for(int i=0;i<numbRecord;i++){
        if(item(i)->data(Qt::UserRole).canConvert<QSqlRecord>())
            outVect.append(qvariant_cast<QSqlRecord>(item(i)->data(Qt::UserRole)));

    }
    return outVect;
}


void MediaListWidget::addNewItem(){

    QFileDialog openDialog(this);
    openDialog.setNameFilter("Images (*.png *.gif *.jpg *jpeg)");

    //QStringList filenames=QFileDialog::getOpenFileNames(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");

    if(openDialog.exec()){
        QStringList filenames=openDialog.selectedFiles();
        QSqlRecord newImage = QSqlRecord();
        for (int i=0;i<filenames.length();i++){
            newImage.clear();
            newImage.append(QSqlField("File",QVariant::String));
            newImage.append(QSqlField("Description",QVariant::String));
            newImage.append(QSqlField("Status",QVariant::String));
            newImage.setValue("File",filenames[i]);
            newImage.setValue("Description","No Description");
            newImage.setValue("Status","N");
        }

        QListWidgetItem *newItem=new QListWidgetItem();
        newItem->setData(Qt::UserRole,QVariant::fromValue(newImage));
        //int numItems=count();
        addItem(newItem);
        emit updatedImages();
    }
}




Q_DECLARE_METATYPE(QSqlRecord);
