#include "displaywidget.h"
#include <QVariant>
#include <QLabel>
#include <QStackedWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPixmap>
#include <QListWidgetItem>
#include "medialistwidget.h"
#include <QFileDialog>
#include <imageitemdelegate.h>
#include <QMetaType>
#include <QDebug>
#include <QSpacerItem>
#include <QSqlField>
#include <QSqlRecord>
#include <mysqltablemodel.h>
#include <datatype.h>


DisplayWidget::DisplayWidget(QString newFieldName, DataType::dataType FieldType, bool Edit,  QWidget *parent) :
    QStackedWidget(parent)
{
    Field=newFieldName;
    Type=FieldType;
    isEdit=Edit;
    construct();

}

DisplayWidget::DisplayWidget(QString newFieldName, DataType::dataType FieldType, QVariant Value, bool Edit,  QWidget *parent)
    :QStackedWidget(parent)
{
    m_Value = Value;
    isEdit=Edit;
    Field=newFieldName;
    Type=FieldType;
    construct();
    switch(Type)
    {
    case DataType::Text:{
        ShortText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }
        break;
    case DataType::LongText:{
        LongText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }
        break;
    case DataType::Image:{
        QPixmap image = QPixmap(m_Value.toString());
        image= image.scaledToHeight(200);
        ImageDisplay->setPixmap(image);
        ImageDisplay->setFixedSize(QSize(image.width(),image.height()));
        ImageEdit->setPixmap(image);
        ImageEdit->setFixedSize(QSize(image.width(),image.height()));
    }
        break;
    case DataType::Images:{
        setImages();
    }
        break;
    }

}

void DisplayWidget::construct(){
    EFieldName = new QLabel(QString("%1: ").arg(Field),Editable);
    NFieldName = new QLabel(QString("%1: ").arg(Field),NEditable);

    switch(Type){
    case DataType::Text:
        EditLayout->setDirection(QBoxLayout::LeftToRight);
        NEditLayout->setDirection(QBoxLayout::LeftToRight);
        ShortText = new QLineEdit(Editable);
        connect(ShortText,SIGNAL(textChanged(QString)),this,SLOT(ValueChange()));
        TextDisplay = new QLabel(NEditable);

        EditLayout->addWidget(EFieldName);
        EditLayout->addWidget(ShortText);

        NEditLayout->addWidget(NFieldName);
        NEditLayout->addWidget(TextDisplay);
        NEditLayout->addStretch(2);
        break;
    case DataType::LongText:
        EditLayout->setDirection(QBoxLayout::TopToBottom);
        NEditLayout->setDirection(QBoxLayout::TopToBottom);
        LongText = new QTextEdit(Editable);
        TextDisplay = new QLabel(NEditable);
        TextDisplay->setWordWrap(true);

        EditLayout->addWidget(EFieldName);
        EditLayout->addWidget(LongText);

        NEditLayout->addWidget(NFieldName);
        NEditLayout->addWidget(TextDisplay);
        break;
    case DataType::Image:
        EditLayout->setDirection(QBoxLayout::TopToBottom);
        NEditLayout->setDirection(QBoxLayout::TopToBottom);

        ImageEdit = new QLabel(Editable);
        ImageDisplay = new QLabel(NEditable);

        ChangeImage = new QPushButton("Change Image");
        RemoveImage = new QPushButton("Remove Image");

        EditLayout->addWidget(EFieldName);
        EditLayout->addWidget(ImageEdit);
        EditLayout->addWidget(ChangeImage);
        EditLayout->addWidget(RemoveImage);

        NEditLayout->addWidget(NFieldName);
        NEditLayout->addWidget(ImageDisplay);
        break;
    case DataType::Images:
        EditLayout->setDirection(QBoxLayout::TopToBottom);
        NEditLayout->setDirection(QBoxLayout::TopToBottom);

        //ImagesDisplay = new QListWidget(NEditable);
        ImagesDisplay =new MediaListWidget(NEditable);
        ImagesDisplay->setViewMode(QListView::IconMode);
        ImagesDisplay->setIconSize(QSize(200,200));
        ImagesDisplay->setWrapping(false);
        ImagesDisplay->setItemDelegate(new ImageItemDelegate(ImagesDisplay));

        //ImagesEdit = new QListWidget(Editable);
        ImagesEdit = new MediaListWidget(Editable);
        ImagesEdit->setViewMode(QListView::IconMode);
        ImagesEdit->setIconSize(QSize(200,200));
        ImagesEdit->setWrapping(false);
        ImagesEdit->setItemDelegate(new ImageItemDelegate(ImagesEdit));
        ImagesEdit->setEditTriggers(QAbstractItemView::DoubleClicked);

        //AddImages= new QPushButton("Add Images");
        //RemoveImages= new QPushButton("Remove Images");

        //connect(AddImages,SIGNAL(clicked()),this,SLOT(addImages()));
        //connect(RemoveImages,SIGNAL(clicked()),this,SLOT(removeImages()));

        connect(ImagesEdit,SIGNAL(updatedImages()),this,SLOT(imagesChanged()));

        EditLayout->addWidget(EFieldName);
        EditLayout->addWidget(ImagesEdit);
        //EditLayout->addWidget(AddImages);
        //EditLayout->addWidget(RemoveImages);

        NEditLayout->addWidget(NFieldName);
        NEditLayout->addWidget(ImagesDisplay);
        break;
    }




    Editable->setLayout(EditLayout);
    NEditable->setLayout(NEditLayout);

    addWidget(NEditable);
    addWidget(Editable);
    setCurrentIndex(isEdit);
}

void DisplayWidget::imagesChanged(){
    /*QStringList filenames=QFileDialog::getOpenFileNames(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");


    QVector<QSqlRecord > currentImages = qvariant_cast<QVector<QSqlRecord> >(m_Value);
    QSqlRecord newImage = QSqlRecord();
    for (int i=0;i<filenames.length();i++){
        newImage.clear();
        newImage.append(QSqlField("File",QVariant::String));
        newImage.append(QSqlField("Description",QVariant::String));
        newImage.append(QSqlField("Status",QVariant::String));
        newImage.setValue("File",filenames[i]);
        newImage.setValue("Description","No Description");
        newImage.setValue("Status","N");
        currentImages.append(newImage);
    }
    m_Value.setValue(currentImages);*/
    //qDebug()<<"You changed the values, they were:\n"<<qvariant_cast<QVector<QSqlRecord> >(m_Value)<< "\n they now are:\n"<<ImagesEdit->getRecords();
    m_Value.setValue(ImagesEdit->getRecords());

}



void DisplayWidget::reload(){
    switch(Type){
    case DataType::Text:{
        ShortText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }
        break;
    case DataType::LongText:{
        LongText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }
        break;
    case DataType::Image:{
        QPixmap image = QPixmap(m_Value.toString());
        image= image.scaledToHeight(200);

        ImageDisplay->setPixmap(image);
        ImageDisplay->setFixedSize(QSize(image.width(),image.height()));
        ImageEdit->setPixmap(image);
        ImageEdit->setFixedSize(QSize(image.width(),image.height()));
    }
        break;
    case DataType::Images:{
        setImages();
    }
        break;
    }

    update();
}

void DisplayWidget::ValueChange(){
    switch(Type){
    case DataType::Text:
        setValue(ShortText->text());
        break;
    case DataType::LongText:
        setValue(LongText->toPlainText());
        break;
    }

}


void DisplayWidget::setImages(){
    QVector<QSqlRecord> Data = qvariant_cast<QVector<QSqlRecord> >(m_Value);
    //ImagesDisplay->clear();
    //ImagesEdit->clear();
    for(int i=0;i<Data.size();i++){
        //QString fileDir=Data[i].value("Directory").toString();
        //fileDir.append(Data[i].value("File").toString());
        QListWidgetItem *edit_item=new QListWidgetItem();
        /*edit_item->setText(Data[i].value("Description").toString());
        edit_item->setIcon(QIcon(fileDir));
        edit_item->setData(Qt::UserRole,fileDir);
        edit_item->setData(Qt::UserRole+1,Data[i].value("Status").toString());*/
        edit_item->setData(Qt::UserRole,QVariant::fromValue(Data[i]));


        QListWidgetItem* disp_item =new QListWidgetItem(*edit_item);
        edit_item->setFlags (edit_item->flags () | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        ImagesDisplay->addItem(disp_item);
        ImagesEdit->addItem(edit_item);

    }
    ImagesDisplay->update();
    ImagesEdit->update();
}



void DisplayWidget::addImages(){
    QStringList filenames=QFileDialog::getOpenFileNames(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");


    QVector<QSqlRecord > currentImages = qvariant_cast<QVector<QSqlRecord> >(m_Value);
    QSqlRecord newImage = QSqlRecord();
    for (int i=0;i<filenames.length();i++){
        newImage.clear();
        newImage.append(QSqlField("File",QVariant::String));
        newImage.append(QSqlField("Description",QVariant::String));
        newImage.append(QSqlField("Status",QVariant::String));
        newImage.setValue("File",filenames[i]);
        newImage.setValue("Description","No Description");
        newImage.setValue("Status","N");
        currentImages.append(newImage);
    }
    m_Value.setValue(currentImages);
    setImages();

}

void DisplayWidget::removeImages(){
    QList<QListWidgetItem *> selItems =ImagesEdit->selectedItems();
    for(int i=0;i<selItems.count();i++){
        if(selItems.at(i)->data(Qt::UserRole+1).toString()=="N"){
            ImagesEdit->removeItemWidget(selItems.at(i));
        }else if(selItems.at(i)->data(Qt::UserRole+1).toString()=="O"){
            selItems.at(i)->setData(Qt::UserRole+1,"R");
            selItems.at(i)->setHidden(true);
        }

    }
    updateValue();
}

void DisplayWidget::updateValue(){
    QVector<QSqlRecord> newImages;
    QSqlRecord newImage = QSqlRecord();
    newImage.append(QSqlField("File",QVariant::String));
    newImage.append(QSqlField("Description",QVariant::String));
    newImage.append(QSqlField("Status",QVariant::String));

    for (int i=0;i<ImagesEdit->count();i++){
        newImage.setValue("File",ImagesEdit->item(i)->data(Qt::UserRole));
        newImage.setValue("Status",ImagesEdit->item(i)->data(Qt::UserRole+1));
        newImage.setValue("Description",ImagesEdit->item(i)->text());
        newImages.append(newImage);
        qDebug()<<"new value: "<<newImage;
        newImage.clearValues();
    }
    m_Value.setValue(newImages);
}

/*void DisplayWidget::removeImages(){
    QVector <QVector <QString> > Images = qvariant_cast<QVector<QVector<QString> > >(m_Value);
    for(int i =0;i<ImagesEdit->count();i++){
        QListWidgetItem *currItem = ImagesEdit->item(i);
        if(currItem->isSelected()){
            if(currItem->data(Qt::UserRole+1)=="O"){
                currItem->setData(Qt::UserRole+1,"R");
                currItem->setHidden(true);
                Images[i][2]="R";
            }else{
                currItem->setData(Qt::UserRole+1,"NA");
                currItem->setHidden(true);
                Images[i][2]="NA";
            }
        }
    }
    m_Value.setValue(Images);
}*/

void DisplayWidget::changeImage(){
    QString filename = QFileDialog::getOpenFileName(this,"Set Image","/home","Images (*.png *.gif *.jpg *jpeg)");
    if(filename != ""){
        m_Value = filename;
        ImageDisplay->setPixmap(QPixmap(filename));
        ImageEdit->setPixmap(QPixmap(filename));

    }
}

void DisplayWidget::removeImage(){
    m_Value="";
    ImageDisplay->clear();
    ImageEdit->clear();
}


Q_DECLARE_METATYPE(QVector<QVector<QString> >);
Q_DECLARE_METATYPE(QVector<QSqlRecord>);
