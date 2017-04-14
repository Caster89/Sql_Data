#include "displaywidgetimages.h"
#include "imageitemdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSqlField>


DisplayWidgetImages::DisplayWidgetImages(MyField newField, bool editable, QWidget *parent)
    :DisplayWidgetBase(newField,parent)
{
    editTitle->setText(field.getName()+':');
    nonEditTitle->setText(field.getName()+':');

    QVBoxLayout *editLayout = new QVBoxLayout(editWidget);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QVBoxLayout *nonEditLayout = new QVBoxLayout(nonEditWidget);

    editWidget->setLayout(editLayout);
    nonEditWidget->setLayout(nonEditLayout);
    //ImagesDisplay = new QListWidget(NEditable);
    ImagesDisplay =new MediaListWidget(nonEditWidget, false);
    ImagesDisplay->setViewMode(QListView::IconMode);
    ImagesDisplay->setIconSize(QSize(200,200));
    ImagesDisplay->setWrapping(false);
    ImagesDisplay->setItemDelegate(new ImageItemDelegate(ImagesDisplay));

    //ImagesEdit = new QListWidget(Editable);
    ImagesEdit = new MediaListWidget(editWidget, true);
    ImagesEdit->setViewMode(QListView::IconMode);
    ImagesEdit->setIconSize(QSize(200,200));
    ImagesEdit->setWrapping(false);
    ImagesEdit->setItemDelegate(new ImageItemDelegate(ImagesEdit));
    ImagesEdit->setEditTriggers(QAbstractItemView::DoubleClicked);

    QPushButton *btnAddImage=new QPushButton();
    QIcon btnAddIcon(":/Add_Round_Btn.png");
    btnAddImage->setFixedHeight(30);
    btnAddImage->setFixedWidth(30);
    //pushButton->setText("+");
    btnAddImage->setIcon(btnAddIcon);
    btnAddImage->setIconSize(QSize(30,30));
    btnAddImage->setStyleSheet("QPushButton {border-radius: 15px; border-style: outset;"
                                  "border-width: 2px;border-color: white; font:bold 50px; text-align: center} "
                                  "QPushButton:pressed {border-style: inset; border-color: grey}");

    QPushButton *btnRemoveImage=new QPushButton();
    QIcon btnRmvIcon(":/Rmv_Round_Btn.png");
    btnRemoveImage->setFixedHeight(30);
    btnRemoveImage->setFixedWidth(30);
    //pushButton->setText("+");
    btnRemoveImage->setIcon(btnRmvIcon);
    btnRemoveImage->setIconSize(QSize(30,30));
    btnRemoveImage->setStyleSheet("QPushButton {border-radius: 15px; border-style: outset;"
                                  "border-width: 2px;border-color: white; font:bold 50px; text-align: center} "
                                  "QPushButton:pressed {border-style: inset; border-color: grey}");
    //AddImages= new QPushButton("Add Images");

    //AddImages= new QPushButton("Add Images");
    //RemoveImages= new QPushButton("Remove Images");
    QPushButton *btnIncreaseSize = new QPushButton();
    QIcon btnIconZoomIn(":/Zoom_in.png");
    btnIncreaseSize->setFixedHeight(30);
    btnIncreaseSize->setFixedWidth(30);
    btnIncreaseSize->setIcon(btnIconZoomIn);
    btnIncreaseSize->setIconSize(QSize(30,30));
    btnIncreaseSize->setStyleSheet("QPushButton {border-radius: 15px; border-style: outset;"
                                  "border-width: 0px;border-color: black; font:bold 50px; text-align: center} "
                                  "QPushButton:pressed {border-style: inset; border-color: grey}");

    QPushButton *btnDecreaseSize = new QPushButton();
    QIcon btnIconZoomOut(":/Zoom_out.png");
    btnDecreaseSize->setFixedHeight(30);
    btnDecreaseSize->setFixedWidth(30);
    btnDecreaseSize->setIcon(btnIconZoomOut);
    btnDecreaseSize->setIconSize(QSize(30,30));
    btnDecreaseSize->setStyleSheet("QPushButton {border-radius: 15px; border-style: outset;"
                                  "border-width: 0px;border-color: black; font:bold 50px; text-align: center} "
                                  "QPushButton:pressed {border-style: inset; border-color: grey}");


    btnLayout->addWidget(btnAddImage);
    btnLayout->addWidget(btnRemoveImage);
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnIncreaseSize);
    btnLayout->addWidget(btnDecreaseSize);
    //connect(AddImages,SIGNAL(clicked()),this,SLOT(addImages()));
    //connect(RemoveImages,SIGNAL(clicked()),this,SLOT(removeImages()));

    editLayout->addWidget(editTitle);
    editLayout->addWidget(ImagesEdit);


    nonEditLayout->addWidget(nonEditTitle);
    nonEditLayout->addWidget(ImagesDisplay);


    if(editable){
        setCurrentIndex(Editable);
        editLayout->addLayout(btnLayout);
        connect(btnIncreaseSize, SIGNAL(clicked()), ImagesEdit, SLOT(increaseItemScale()));
        connect(btnDecreaseSize, SIGNAL(clicked()), ImagesEdit, SLOT(decreaseItemScale()));
        connect(btnAddImage, SIGNAL(clicked()), ImagesEdit, SLOT(addNewItem()));
        connect(btnRemoveImage, SIGNAL(clicked()), ImagesEdit, SLOT(removeSelectedItem()));
    }else{
        setCurrentIndex(NonEditable);
        btnAddImage->setVisible(false);
        btnRemoveImage->setVisible(false);
        nonEditLayout->addLayout(btnLayout);
        connect(btnIncreaseSize, SIGNAL(clicked()), ImagesDisplay, SLOT(increaseItemScale()));
        connect(btnDecreaseSize, SIGNAL(clicked()), ImagesDisplay, SLOT(decreaseItemScale()));
    }

    connect(ImagesEdit,SIGNAL(updatedImages()),this,SLOT(imagesChanged()));


}

DisplayWidgetImages::~DisplayWidgetImages()
{

}

void DisplayWidgetImages::setValue(QVariant newValue){
    value=qvariant_cast<QVector<QSqlRecord> >(newValue);
    updateImages();
    //QVector<QSqlRecord> Data = qvariant_cast<QVector<QSqlRecord> >(m_Value);

}

QVariant DisplayWidgetImages::getValue(){
    return QVariant::fromValue(value);
}

///////////////////////////////////////////////////////////////////////////
/// \brief DisplayWidgetImages::addImages
///function called when the add image button is pressed. It opens a File
/// Dialog Window which allowes to to select different image types. All the 
/// selected images are then appended to the value variable and the widged
/// is updated.
///////////////////////////////////////////////////////////////////////////
 
void DisplayWidgetImages::addImages(){
    QStringList filenames=QFileDialog::getOpenFileNames(this,"Add Images","/home","Images (*.png *.gif *.jpg *jpeg)");


    //QVector<QSqlRecord > currentImages = value;
    QSqlRecord newImage = QSqlRecord();
    newImage.append(QSqlField("File",QVariant::String));
    newImage.append(QSqlField("Description",QVariant::String));
    newImage.append(QSqlField("Status",QVariant::String));
    for (int i=0;i<filenames.length();i++){
        newImage.clearValues();
        
        newImage.setValue("File",filenames[i]);
        newImage.setValue("Description","No Description");
        newImage.setValue("Status","N");
        value.append(newImage);
    }
    //m_Value.setValue(currentImages);
    updateImages();
}

////////////////////////////////////////////////////////////////////////////
/// \brief DisplayWidgetImages::updateImages
///Update images is called when the value variable has changed and the widget
///has to be update. The widget is thus cleared and the records in the value 
/// variable are then read one at a time and added to both the widgets
////////////////////////////////////////////////////////////////////////////
void DisplayWidgetImages::updateImages(){
    ImagesDisplay->clear();
    ImagesEdit->clear();

    for(int i=0;i<value.size();i++){
        QListWidgetItem *edit_item=new QListWidgetItem();

        edit_item->setData(Qt::UserRole,QVariant::fromValue(value[i]));
        QString imgFile = value[i].value("File").toString();
                //value[i].value("Directory").toString()+value[i].value("File").toString();
        edit_item->setData(Qt::DecorationRole, QVariant::fromValue(QPixmap(imgFile)));

        QListWidgetItem* disp_item =new QListWidgetItem(*edit_item);
        edit_item->setFlags (edit_item->flags () | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        ImagesDisplay->addItem(disp_item);
        ImagesEdit->addItem(edit_item);

    }
    ImagesDisplay->update();
    ImagesEdit->update();
}

/////////////////////////////////////////////////////////////////////////////////
/// \brief DisplayWidgetImages::removeImages
///This function is called when the remove images button is pressed. It stores
/// the selected images in a list, then reads the list one item at a time. if 
/// the item has hest been added then it removed the item from the widget 
/// directly. If the item already existed then it changes the status to R and
/// hides the item
////////////////////////////////////////////////////////////////////////////////
void DisplayWidgetImages::removeImages(){
    QList<QListWidgetItem *> selItems =ImagesEdit->selectedItems();
    for(int i=0;i<selItems.count();i++){
        if(selItems.at(i)->data(Qt::UserRole+1).toString()=="N"){
            ImagesEdit->removeItemWidget(selItems.at(i));
        }else if(selItems.at(i)->data(Qt::UserRole+1).toString()=="O"){
            selItems.at(i)->setData(Qt::UserRole+1,"R");
            selItems.at(i)->setHidden(true);
        }

    }
    retrieveValue();
}

void DisplayWidgetImages::retrieveValue(){
    //QVector<QSqlRecord> newImages;
    value.clear();
    QSqlRecord newImage = QSqlRecord();
    newImage.append(QSqlField("File",QVariant::String));
    newImage.append(QSqlField("Description",QVariant::String));
    newImage.append(QSqlField("Status",QVariant::String));

    for (int i=0;i<ImagesEdit->count();i++){
        newImage.setValue("File",ImagesEdit->item(i)->data(Qt::UserRole));
        newImage.setValue("Status",ImagesEdit->item(i)->data(Qt::UserRole+1));
        newImage.setValue("Description",ImagesEdit->item(i)->text());
        value.append(newImage);

        newImage.clearValues();
    }
}

void DisplayWidgetImages::imagesChanged(){
    value=ImagesEdit->getRecords();
}

//Q_DECLARE_METATYPE(QSqlRecord);

