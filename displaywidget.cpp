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
#include <QFileDialog>
#include <imageitemdelegate.h>
#include <QMetaType>
#include <QDebug>
#include <QSpacerItem>
#include <QSqlField>
#include <QSqlRecord>


DisplayWidget::DisplayWidget(QString newFieldName, QString FieldType, bool Edit,  QWidget *parent) :
    QStackedWidget(parent)
{
    Field=newFieldName;
    Type=FieldType;
    isEdit=Edit;
    construct();

}

DisplayWidget::DisplayWidget(QString newFieldName, QString FieldType, QVariant Value, bool Edit,  QWidget *parent)
    :QStackedWidget(parent)
{
    m_Value = Value;
    isEdit=Edit;
    Field=newFieldName;
    Type=FieldType;
    construct();

    if (Type=="TEXT"){
        ShortText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }else if (Type=="LONGTEXT"){
        LongText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }else if (Type=="IMAGE"){
        QPixmap image = QPixmap(m_Value.toString());
        image= image.scaledToHeight(200);
        ImageDisplay->setPixmap(image);
        ImageDisplay->setFixedSize(QSize(image.width(),image.height()));
        ImageEdit->setPixmap(image);
        ImageEdit->setFixedSize(QSize(image.width(),image.height()));
    }else if (Type=="IMAGES"){
        setImages();
    }
}

void DisplayWidget::construct(){
    EFieldName = new QLabel(QString("%1: ").arg(Field),Editable);
    NFieldName = new QLabel(QString("%1: ").arg(Field),NEditable);

    if (Type=="TEXT"){
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

    }else if (Type=="LONGTEXT"){
        EditLayout->setDirection(QBoxLayout::TopToBottom);
        NEditLayout->setDirection(QBoxLayout::TopToBottom);
        LongText = new QTextEdit(Editable);
        TextDisplay = new QLabel(NEditable);
        TextDisplay->setWordWrap(true);

        EditLayout->addWidget(EFieldName);
        EditLayout->addWidget(LongText);

        NEditLayout->addWidget(NFieldName);
        NEditLayout->addWidget(TextDisplay);



    }else if (Type=="IMAGE") {
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
    }else if (Type=="IMAGES"){
        EditLayout->setDirection(QBoxLayout::TopToBottom);
        NEditLayout->setDirection(QBoxLayout::TopToBottom);

        ImagesDisplay = new QListWidget(Editable);
        ImagesDisplay->setViewMode(QListView::IconMode);
        ImagesDisplay->setIconSize(QSize(200,200));
        ImagesDisplay->setWrapping(false);
        ImagesDisplay->setItemDelegate(new ImageItemDelegate(ImagesDisplay));

        ImagesEdit = new QListWidget(NEditable);
        ImagesEdit->setViewMode(QListView::IconMode);
        ImagesEdit->setIconSize(QSize(200,200));
        ImagesEdit->setWrapping(false);
        ImagesEdit->setItemDelegate(new ImageItemDelegate(ImagesDisplay));

        AddImages= new QPushButton("Add Images");
        //RemoveImages= new QPushButton("Remove Images");

        connect(AddImages,SIGNAL(clicked()),this,SLOT(addImages()));

        EditLayout->addWidget(EFieldName);
        EditLayout->addWidget(ImagesEdit);
        EditLayout->addWidget(AddImages);
        //EditLayout->addWidget(RemoveImages);

        NEditLayout->addWidget(NFieldName);
        NEditLayout->addWidget(ImagesDisplay);
    }

    Editable->setLayout(EditLayout);
    NEditable->setLayout(NEditLayout);

    addWidget(NEditable);
    addWidget(Editable);
    setCurrentIndex(isEdit);
}



void DisplayWidget::reload(){
    if (Type=="TEXT"){
        ShortText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }else if(Type=="LONGTEXT"){
        LongText->setText(m_Value.toString());
        TextDisplay->setText(m_Value.toString());
    }else if (Type=="IMAGE"){
        QPixmap image = QPixmap(m_Value.toString());
        image= image.scaledToHeight(200);

        ImageDisplay->setPixmap(image);
        ImageDisplay->setFixedSize(QSize(image.width(),image.height()));
        ImageEdit->setPixmap(image);
        ImageEdit->setFixedSize(QSize(image.width(),image.height()));
    }else if(Type=="IMAGES"){
        setImages();
    }
}

void DisplayWidget::ValueChange(){
    if (Type=="TEXT"){
        setValue(ShortText->text());
    }else if (Type=="TEXT"){
        setValue(LongText->toPlainText());
    }
}


void DisplayWidget::setImages(){
    QVector<QSqlRecord> Data = qvariant_cast<QVector<QSqlRecord> >(m_Value);
    qDebug()<<Data;
    ImagesDisplay->clear();
    ImagesEdit->clear();
    for(int i=0;i<Data.size();i++){

        QListWidgetItem *edit_item=new QListWidgetItem();
        edit_item->setText(Data[i].value("Description").toString());
        edit_item->setIcon(QIcon(Data[i].value("File").toString()));
        edit_item->setData(Qt::UserRole,Data[i].value("File").toString());
        edit_item->setData(Qt::UserRole+1,Data[i].value("Status").toString());

        QListWidgetItem* disp_item =new QListWidgetItem(*edit_item);
        edit_item->setFlags (edit_item->flags () | Qt::ItemIsEnabled);
        ImagesDisplay->addItem(disp_item);
        ImagesEdit->addItem(edit_item);


    }
    ImagesDisplay->update();
    ImagesEdit->update();
}



void DisplayWidget::addImages(){
    QStringList filenames=QFileDialog::getOpenFileNames(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");
    //msgbx.setText(QString("%1").arg(filenames.length()));
    //msgbx.exec();
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
    //qDebug()<<currentImages;
    m_Value.setValue(currentImages);
    setImages();

}

void DisplayWidget::removeImages(){
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
}

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
