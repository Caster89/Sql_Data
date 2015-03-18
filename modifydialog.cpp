#include "modifydialog.h"
#include "displaywidget.h"
#include "querygenerator.h"
#include <QObject>
#include <QDialog>
#include <QVector>
#include <QString>
#include <QSqlTableModel>
#include "mysqltablemodel.h"
#include <QSqlDatabase>
#include <QModelIndex>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlRecord>
#include <QSqlField>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QDataWidgetMapper>
#include <QStringList>
#include <QScrollBar>
#include "myfield.h"



//The first construvtor allowe a parent and a vector for the field, this is for the creation of new records
ModifyDialog::ModifyDialog(QWidget *parent, MySqlTableModel *DBModel,int index) :
    QDialog(parent)
{
    mapper->setModel(DBModel);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    construct(parent,DBModel);

    if (index <0){
        //DBModel->insertRow(0,QModelIndex());
        //QModelIndex currIndex=DBModel->index(DBModel->rowCount(QModelIndex())-1,0);
        //mapper->setCurrentModelIndex(currIndex);
        mapper->toLast();
    }else{
        QModelIndex currIndex=DBModel->index(index,0);
        mapper->setCurrentModelIndex(currIndex);
    }



}

ModifyDialog::ModifyDialog(QWidget *parent,QSqlDatabase *db, QVector<QVector<QString> > Fields, QVector<QVector<QString> > Primary) :
    QDialog(parent)
{

}

void ModifyDialog::construct(QWidget *parent, MySqlTableModel *DBModel){
    //This Widget creates the basic structure on which to add/modify records
    //The layout type is rid in order to add multiple fields in the same row
    //The layout could be changed to verical- a while loop - horizontal layout
    //if this field and the following are short fields
    //QGridLayout *mainLayout=new QGridLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QWidget *mainWidget = new QWidget(this);
    QScrollArea *scrArea = new QScrollArea(this);

    BaseModel=DBModel;
    QVector<MyField> Fields = DBModel->getFields();



    //The fields are read one by one and added in the correct position along the DispFields array
    for (int i=0;i<Fields.size();i++){
        DispFields.append(new DisplayWidget(Fields[i].getName(),Fields[i].getType(),true));
        mapper->addMapping(DispFields.last(),i,"Value");
    }

    //To insert the various fields in the correct position DispFields is iterated. A first check is done to make
    //sure there are 2 more fields available, if this is not true then the remaining field is simply added
    //by itself, with a horizontal box spacer if the field is of type TEXT.If not, if the two following fields
    //are both of type TEXT then the two are put in one horizontal box and the box is added to the main layout
    //if only th efirst field is of type TEXT it is added to a horizontal box with a spacer, which is added to
    //the main layout, while the following field is added by itself. If at last the first field is something
    //other than TEXT then it is inserted as is in the main layout and the following widget is added with a
    //horizontal spacer
    //THREE POSSIBLE COMBINATIONS:
    //1. TEXT && TEXT
    //2. TEXT && OTHER
    //3. OTHER && TEXT

    int pos=0;
    while (pos<DispFields.size()){
        if(pos<DispFields.size()-1){
            QHBoxLayout *temp_hLayout = new QHBoxLayout;
            if (Fields[pos].getType()==DataType::Text && Fields[pos+1].getType()==DataType::Text){
                temp_hLayout->addWidget(DispFields[pos]);
                temp_hLayout->addWidget(DispFields[pos+1]);
                mainLayout->addLayout(temp_hLayout);
            }else if(Fields[pos].getType()==DataType::Text){
                temp_hLayout->addWidget(DispFields[pos]);
                temp_hLayout->addStretch((this->width()/2));
                mainLayout->addLayout(temp_hLayout);
                mainLayout->addWidget(DispFields[pos+1]);
            }else{
                mainLayout->addWidget(DispFields[pos]);
                temp_hLayout->addWidget(DispFields[pos+1]);
                temp_hLayout->addSpacing((this->width()/2));
                mainLayout->addLayout(temp_hLayout);

            }
            pos+=2;
        }else{
                mainLayout->addWidget(DispFields[pos]);
                pos++;
        }
    }
    //The Button layout is created with two buttons
    QHBoxLayout *btnLayout= new QHBoxLayout;
    //btnLayout->setDirection(QBoxLayout::RightToLeft);
    QPushButton *btnAccept= new QPushButton("Accept",this);
    QPushButton *btnReject=new QPushButton("Cancel",this);
    btnLayout->addWidget(btnAccept);
    btnLayout->addWidget(btnReject);
    //The main layout is created with no margin, the scrollarea is created
    mainLayout->setMargin(0);
    mainWidget->setLayout(mainLayout);
    scrArea->setWidget(mainWidget);
    scrArea->adjustSize();
    scrArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *dialog_layout=new QVBoxLayout;
    dialog_layout->addWidget(scrArea);
    dialog_layout->addLayout(btnLayout);
    dialog_layout->setContentsMargins(0,0,0,0);
    setLayout(dialog_layout);
    setContentsMargins(0,0,0,0);

    //The two buttons are linked to the respective signals
    //connect(btnAccept,SIGNAL(clicked()),SLOT(accept()));
    //connect(btnReject,SIGNAL(clicked()),this,SLOT(reject()));
    connect(btnAccept,SIGNAL(clicked()),SLOT(AcceptDialog()));
    connect(btnReject,SIGNAL(clicked()),this,SLOT(RejectDialog()));
    scrArea->setMinimumWidth(scrArea->contentsRect().width()+scrArea->verticalScrollBar()->width());

}

QVector<DisplayWidget *> ModifyDialog::getValues(){
        return DispFields;
}

void ModifyDialog::AcceptDialog(){

    mapper->submit();
    BaseModel->submitAll();
    accept();
}

void ModifyDialog::RejectDialog(){
    mapper->model()->revert();
    reject();
}

