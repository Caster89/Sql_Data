#include "dragscrollarea.h"
#include <QDebug>

DragScrollArea::DragScrollArea(QWidget *parent) : QScrollArea(parent)
{
    //this->addWidget(mainWidget);
    mainWidget->setLayout(mainLayout);
    //scrollWidget->setLayout(scrollLayout);

    mainWidget->layout()->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    mainWidget->setContentsMargins(0,0,0,0);
    mainWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    this->setWidget(mainWidget);
    this->setWidgetResizable(true);


    setMinimumWidth(400);
}

void DragScrollArea::addWidget(QWidget *newWidget){
    widgetList.append(newWidget);
    mainLayout->addWidget(newWidget);
    emit itemAdded();
}

void DragScrollArea::clearArea(){
    for(int i = mainLayout->count()-1; i>=0;--i){
        mainLayout->removeItem(mainLayout->itemAt(i));
    }
}

void DragScrollArea::repopulateArea(){
    for (int i = 0; i<widgetList.count();++i){
        mainLayout->addWidget(widgetList[0]);
    }
}

void DragScrollArea::removeWidget(QWidget* removeWidget){
    qDebug()<<"Removing item";
    int posDelete = widgetList.lastIndexOf(removeWidget);

    removeWidget->setParent(NULL);

    widgetList.removeAt(posDelete);
    emit itemRemoved();

}


