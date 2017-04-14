#include "displaywidgetimage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFileDialog>
#include <QSqlField>

DisplayWidgetImage::DisplayWidgetImage(MyField newField, bool editable, QWidget *parent)
    :DisplayWidgetBase(newField,parent)
{
    editTitle->setText(field.getName()+':');
    nonEditTitle->setText(field.getName()+':');
    QVBoxLayout *editLayout = new QVBoxLayout();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QVBoxLayout *nonEditLayout = new QVBoxLayout();
    editLayout->addWidget(editTitle);
    editLayout->addWidget(lblEditImage);
    btnLayout->addWidget(btnSelectImg);
    btnLayout->addWidget(btnRemoveImg);
    editLayout->addLayout(btnLayout);

    nonEditLayout->addWidget(nonEditTitle);
    nonEditLayout->addWidget(lblNonEditImage);

    editWidget->setLayout(editLayout);
    nonEditWidget->setLayout(nonEditLayout);
    if(editable){
        setCurrentIndex(Editable);
    }else{
        setCurrentIndex(NonEditable);
    }
    value.append(QSqlField("File",QVariant::String));
    value.append(QSqlField("Status", QVariant::String));
    value.setNull("File");
    value.setNull("Status");
    noImage = QPixmap(":/Photo_Not_Avlb");
    connect(btnSelectImg,SIGNAL(clicked()),this,SLOT(selectImage()));
    connect(btnRemoveImg,SIGNAL(clicked()),this,SLOT(removeImage()));
}



DisplayWidgetImage::~DisplayWidgetImage()
{

}

void DisplayWidgetImage::setValue(QVariant newValue){
    value=qvariant_cast<QSqlRecord>(newValue);
    qDebug()<<"Value se to: "<<value;
    updateImage();

}

QVariant DisplayWidgetImage::getValue(){
    return QVariant::fromValue(value);
}

void DisplayWidgetImage::updateImage(){
    if(!value.value("File").isNull() && value.value("File")!=""){
        QString File=value.value("File").toString();
        //File.append(QDir::separator());
        //File.append(value.value('FileName').toString());
        lblEditImage->setPixmap(QPixmap(File));
        lblNonEditImage->setPixmap(QPixmap(File));
    }else{
        noImage = noImage.scaled(this->width(), 1000, Qt::KeepAspectRatio);
        lblEditImage->setPixmap(noImage);
        lblNonEditImage->setPixmap(noImage);
    }
}

void DisplayWidgetImage::selectImage(){
    QString filename = QFileDialog::getOpenFileName(this,"Set Image","/home","Images (*.png *.gif *.jpg *.jpeg *.tiff *.tif)");
    qDebug()<<"QFile Dialog for display widget image returned: "<<filename;
    if(filename != ""){
        QFile newFile(filename);
        QFileInfo *newFileInfo = new QFileInfo(newFile);
        if(value.value("File").toString()=="" ||value.value("File").isNull()){
            value.setValue("Status","N");
        }else{
            value.setValue("Status","M");
        }
        value.setValue("File",filename);
        //value.setValue('FileName',newFileInfo->fileName());

        qDebug()<<"display widget image changed the value to: "<<value;
        updateImage();

    }
}

void DisplayWidgetImage::removeImage(){
    value.setNull("FileName");
    value.setValue("Status","R");
    updateImage();
}

