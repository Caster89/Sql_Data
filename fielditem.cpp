#include "fielditem.h"
#include "querygenerator.h"
#include <QLabel>


FieldItem::FieldItem(QWidget *parent):DraggableWidget(parent)
{
    fieldEdit = FieldEdit("New", MyField(),MyField());
    ledtFieldName->setText(fieldEdit.getOldField().getName());

    ckbFieldVisPreview->setChecked(fieldEdit.getOldField().getVisPreview());

    ckbFieldVisTable->setChecked(fieldEdit.getOldField().getVisTable());

    ckbFieldPrimary->setChecked(fieldEdit.getOldField().getPrimary());

    ckbFieldUnique->setChecked(fieldEdit.getOldField().getUnique());
    buildWidget();
    cmbFieldType->setCurrentText(dataTypeMap.key(fieldEdit.getOldField().getType()));
}

FieldItem::FieldItem(MyField field, QWidget *parent):DraggableWidget(parent)
{
    fieldEdit = FieldEdit(QString(""),field,field);
    action = "";
    origField = MyField(field);
    editedField = MyField(field);


    ledtFieldName->setText(fieldEdit.getOldField().getName());

    ckbFieldVisPreview->setChecked(fieldEdit.getOldField().getVisPreview());

    ckbFieldVisTable->setChecked(fieldEdit.getOldField().getVisTable());

    ckbFieldPrimary->setChecked(fieldEdit.getOldField().getPrimary());

    ckbFieldUnique->setChecked(fieldEdit.getOldField().getUnique());
    buildWidget();
    cmbFieldType->setCurrentText(dataTypeMap.key(fieldEdit.getOldField().getType()));

}

void FieldItem::buildWidget(){

    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::StyledPanel);

    QLabel *lblFieldName = new QLabel("Field Name:");
    QLabel *lblFieldType = new QLabel("Field Type:");
    fieldLayout->addWidget(lblFieldName,0,0,1,1,Qt::AlignRight);
    fieldLayout->addWidget(ledtFieldName,0,1,1,1,Qt::AlignRight);

    foreach(QString key, dataTypeMap.keys()){
        cmbFieldType->addItem(key, dataTypeMap.value(key));
    }

    fieldLayout->addWidget(lblFieldType,0,2,1,1,Qt::AlignRight);
    fieldLayout->addWidget(cmbFieldType,0,3,1,1,Qt::AlignRight);

    fieldLayout->addWidget(ckbFieldPrimary,0,4,1,1, Qt::AlignLeft);

    fieldLayout->addWidget(ckbFieldVisTable,1,0,1,1);
    fieldLayout->addWidget(ckbFieldVisPreview,1,1,1,1);
    fieldLayout->addWidget(ckbFieldUnique,1,2,1,1);


    setMainLayout(fieldLayout);
    //setLayout(mainLayout);
    connect(ledtFieldName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged()));
    connect(cmbFieldType, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged()));
    connect(ckbFieldPrimary, SIGNAL(stateChanged(int)), this, SLOT(principalChanged()));
    connect(ckbFieldVisTable, SIGNAL(stateChanged(int)), this, SLOT(visTableChanged()));
    connect(ckbFieldVisPreview, SIGNAL(stateChanged(int)), this, SLOT(visPreviewChanged()));
    connect(ckbFieldUnique, SIGNAL(stateChanged(int)), this, SLOT(uniqueChanged()));
}

FieldEdit FieldItem::changes(){
    return fieldEdit;
    //return FieldEdit(action,editedField,origField);
}



void FieldItem::nameChanged(){
    fieldEdit.setName(ledtFieldName->text());
    editedField.setName(ledtFieldName->text());
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action = "Edit";
    } else {
        action = "";
    }
}

void FieldItem::positionChanged(){
    //editedField.setPos(-1);
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::setPosition(int i){
    qDebug()<<"Setting position of "<<fieldEdit.getOldField().getName()<<" to "<<i;
    fieldEdit.setPos(i);
    editedField.setPos(i);
    qDebug()<<"Getting action of "<<fieldEdit.getOldField().getName()<<" "<<fieldEdit.getAction();
    if (getAction()=="Remove" || getAction()=="New"){
        qDebug()<<"Action was remove or New no change of action";
        return;
    }
    if (origField!=editedField){
        qDebug()<<"Fields changed";
        fieldEdit.setAction("Edit");
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::typeChanged(){
    fieldEdit.setType(qvariant_cast<DataType::dataType>(cmbFieldType->currentData()));
    editedField.setType(qvariant_cast<DataType::dataType>(cmbFieldType->currentData()));
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::principalChanged(){
    fieldEdit.setPrimary(ckbFieldPrimary->isChecked());
    editedField.setPrimary(ckbFieldPrimary->isChecked());
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::visTableChanged(){
    fieldEdit.setVisTable(ckbFieldVisTable->isChecked());
    editedField.setVisTable(ckbFieldVisTable->isChecked());
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::visPreviewChanged(){
    fieldEdit.setPreview(ckbFieldVisPreview->isChecked());
    editedField.setVisPreview(ckbFieldVisPreview->isChecked());
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::uniqueChanged(){
    fieldEdit.setUnique(ckbFieldUnique->isChecked());
    editedField.setUnique(ckbFieldUnique->isChecked());
    if (getAction()=="Remove" || getAction()=="New"){
        return;
    }
    if (origField!=editedField){
        action="Edit";
    } else {
        action = "";
    }
}

void FieldItem::commentsChanged(){

}
