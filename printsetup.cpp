#include "printsetup.h"
#include <QScrollArea>
#include "printitem.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QVariant>
#include <QDebug>



PrintSetup::PrintSetup(QVector<MyField> avlbFields, QWidget *parent) : QWidget(parent)
{
    Fields=avlbFields;

    cmbFields=new QComboBox();
    for(int i=0;i<avlbFields.size();i++)
        cmbFields->addItem(Fields[i].getName(),QVariant::fromValue(Fields[i]));

    setLayout(mainLayout);
    mainLayout->addWidget(cmbFields,1,1,1,1);
    qDebug()<<"Print Setup";
    scrollLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    scrollLayout->setContentsMargins(1,1,1,1);
    scrollLayout->setSpacing(1);
    scrollArea->setLayout(scrollLayout);
    scrollArea->setWidgetResizable(false);
    scrollWidget->setLayout(scrollLayout);
    //scrollLayout->addStretch(0);
    scrollWidget->layout()->setAlignment( Qt::AlignTop );
    scrollWidget->setContentsMargins(1,1,1,1);
    //scrollWidget->setStyleSheet("background-color:black;");
    scrollWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(btnAddField,1,2,1,1);
    mainLayout->addWidget(scrollArea,2,1,5,4);
    //mainLayout->addWidget(mainTools,2,1,5,4);

    connect(btnAddField,SIGNAL(clicked()),this,SLOT(addField()));
    resize(600,400);



}

void PrintSetup::addField(){
    qDebug()<<"Step One";
    printFields.append(qvariant_cast<MyField>(cmbFields->currentData()));
    qDebug()<<"Step Two";
    printItems.append(new PrintItem(printFields.last()));
    qDebug()<<"Step Three";
    //mainTools->addItem((printItems.last())->getWidget(),"Prova");
    scrollLayout->addWidget((printItems.last())->getWidget());
    qDebug()<<"Step Four";
    //scrollArea->update();

}

PrintSetup::~PrintSetup()
{

}


Q_DECLARE_METATYPE(MyField);
Q_DECLARE_METATYPE(QVector<MyField>);
