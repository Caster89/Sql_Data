#include "editdatabasedialog.h"
#include <QPushButton>
#include "fielditem.h"
#include <QDebug>
#include <QMessageBox>

EditDatabaseDialog::EditDatabaseDialog(QWidget *parent):QDialog(parent)
{
    buildWidget();
}

EditDatabaseDialog::EditDatabaseDialog(QVector<MyField> fields, QWidget* parent) :QDialog(parent)
{
    buildWidget();
    foreach(MyField currField, fields){
        dragWidget->addWidget(new FieldItem(currField));
        connect(dragWidget->lastWidget(), SIGNAL(closeWidget()),this, SLOT(removeField()));
    }
}

void EditDatabaseDialog::buildWidget(){

    //setLayout(mainLayout);
    //dragWidget->setWidget(mainWidget);
    QPushButton *btnAddField = new QPushButton("Add Field");
    QPushButton *btnRemoveField = new QPushButton("Remove Field");
    QPushButton *btnAcceptChanges = new QPushButton("Accept");

    setLayout(mainLayout);
    mainLayout->addWidget(dragWidget,0,0,3,3);
    mainLayout->addWidget(btnAddField,3,0,1,1);
    mainLayout->addWidget(btnRemoveField,3,1,1,1);
    mainLayout->addWidget(btnAcceptChanges,3,2,1,1);

    connect(btnAddField, SIGNAL(clicked(bool)), this, SLOT(addNewField()));
    connect(btnRemoveField, SIGNAL(clicked(bool)), this, SLOT(removeField()));
    connect(btnAcceptChanges, SIGNAL(clicked(bool)), this, SLOT(acceptChanges()));
    connect(dragWidget, SIGNAL(itemMoved()), this , SLOT(resetPositions()));
}


void EditDatabaseDialog::addNewField(){
    //dragWidget->addWidget(new QPushButton("Button"));
    dragWidget->addWidget(new FieldItem());

    connect(dragWidget->lastWidget(),SIGNAL(closeWidget()),this, SLOT(removeField()));
    //connect(widgetList.last(),SIGNAL(itemModified()), this, SIGNAL(itemModified()));
}

void EditDatabaseDialog::removeField(){

    FieldItem* senderItm = qobject_cast<FieldItem*>(QObject::sender());
    if (senderItm->getAction()=="New"){
        dragWidget->removeWidget(senderItm);
        return;
    }
    senderItm->setAction("Remove");
    dragWidget->reorderWidget(senderItm,dragWidget->count());

    resetPositions();
    //if (senderItm->get)
   // removeWidget(senderItm);
}

QList<FieldEdit*> EditDatabaseDialog::editDatabase(QVector<MyField> fieldsList, QWidget *parent){
    EditDatabaseDialog dialog(fieldsList, parent);
    if (dialog.exec() == QDialog::Accepted){
        return dialog.getChanges();
    }
    return QList<FieldEdit*>();
}

QList<FieldEdit*> EditDatabaseDialog::getChanges(){
    QList<FieldEdit*> actions = QList<FieldEdit*>();
    for (int i = 0; i<dragWidget->count(); i++){
        FieldItem *currWidget = (FieldItem*)(dragWidget->widgetAt(i));
        actions.append(new FieldEdit(currWidget->changes()));
    }
    return actions;
}

void EditDatabaseDialog::resetPositions(){
    for (int i = 0; i < dragWidget->count(); i++){

        dynamic_cast<FieldItem*>(dragWidget->widgetAt(i))->setPosition(i);

    }
}

bool EditDatabaseDialog::valid(){
    for (int i = 0; i < dragWidget->count(); i++){
        FieldItem* currCheck = dynamic_cast<FieldItem*>(dragWidget->widgetAt(i));
        qDebug()<<"Field's name: "<<currCheck->getField().getName();
        if (currCheck->getField().getName()=="")
            return false;
    }
     return true;
}

void EditDatabaseDialog::acceptChanges(){
    qDebug()<<"Accepting changes";
    qDebug()<<"Everything valid: "<<valid();
    if (valid()){
        accept();
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("Data Error.");
    msgBox.setInformativeText("One of the fields is missing required information (Possibly the field's name)");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
    return;
}
