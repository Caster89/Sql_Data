#include "dragscrollarea.h"
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include "draggablewidget.h"
#include <QPushButton>

DragScrollArea::DragScrollArea(QWidget *parent) : QScrollArea(parent)
{
    //this->addWidget(mainWidget);
    mainLayout->addSpacing(2);
    mainWidget->setLayout(mainLayout);
    //scrollWidget->setLayout(scrollLayout);

    mainWidget->layout()->setAlignment( Qt::AlignTop | Qt::AlignCenter );
    mainWidget->layout()->setSpacing(0);
    mainWidget->setContentsMargins(0,0,0,0);
    mainWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    this->setWidget(mainWidget);
    this->setWidgetResizable(true);

    setAcceptDrops(true);

    setMinimumWidth(350);
}

void DragScrollArea::addWidget(QWidget *newWidget){
    widgetList.append(newWidget);
    mainLayout->addWidget(newWidget,1);
    mainLayout->addSpacing(2);
    emit itemAdded();
}

void DragScrollArea::clearArea(){
    for(int i = mainLayout->count()-1; i>=0;--i){
        mainLayout->removeItem(mainLayout->itemAt(i));
    }
    for (int i =0; i<widgetList.count();i++){
        widgetList[i]->setParent(NULL);
    }
}

void DragScrollArea::repopulateArea(){
    mainLayout->addSpacing(2);
    for (int i = 0; i<widgetList.count();++i){
        mainLayout->addWidget(widgetList[0],1);
        mainLayout->addSpacing(2);
    }
}

void DragScrollArea::removeWidget(QWidget* removeWidget){
    qDebug()<<"Removing item";
    int posDelete = widgetList.lastIndexOf(removeWidget);
    QSpacerItem *spacerDelete = (QSpacerItem*)(mainLayout->itemAt(posDelete-1));
    mainLayout->removeItem(spacerDelete);
    delete spacerDelete;

    removeWidget->setParent(NULL);

    widgetList.removeAt(posDelete);
    emit itemRemoved();
}

void DragScrollArea::reorderWidget(QWidget *widget, int newPos){
    int origPos = widgetList.lastIndexOf(widget);
    /*if (newPos >= widgetList.size()){
        newPos = widgetList.size()-1;
    }

    widgetList.move(origPos,newPos);
    clearArea();
    repopulateArea();*/
    reorderWidget(origPos, newPos);
}

void DragScrollArea::reorderWidget(int orig, int dest){
    if (dest>= widgetList.size()){
        dest = widgetList.size()-1;
    }
    qDebug()<<"Widget List before:"<<widgetList;
    widgetList.move(orig,dest);
    qDebug()<<"Widget List after:"<<widgetList;
    mainLayout->removeItem(mainLayout->itemAt(2*orig));
    mainLayout->removeItem(mainLayout->itemAt(2*orig));

    mainLayout->insertWidget(2*dest,widgetList[dest]);
    mainLayout->insertSpacing(2*dest, 2);
    QSpacerItem* currSpacer;
    for (int i = 0; i<mainLayout->count(); i=i+2){
        currSpacer = (QSpacerItem*)(mainLayout->itemAt(i));
        currSpacer->changeSize(1,2);
    }

    emit itemMoved();
    //clearArea();
    //repopulateArea();
}

void DragScrollArea::mousePressEvent(QMouseEvent *event)
{

    QWidget *child = childAt(event->pos());
    if (child->objectName() == "qt_scrollarea_viewport" || child == mainWidget){
        return;
    }
    while (child->parent() != mainWidget){
        child = dynamic_cast<QWidget*>(child->parent());
    }

    if (!child){
        return;
    }

    if (event->button() == Qt::LeftButton)
            dragStartPosition = event->pos();
}

void DragScrollArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)){
        return;
    }
    if ((event->pos() - dragStartPosition).manhattanLength()< minDragMove){
        return;
    }
    //qDebug()<<"Manhattan Length"<<(event->pos() - dragStartPosition).manhattanLength();
    //qDebug()<<"Start Position"<<dragStartPosition;
    QWidget *child = childAt(dragStartPosition);
    while (child->parent() != mainWidget){
        child = dynamic_cast<QWidget*>(child->parent());
    }
    moveFromPosition = widgetList.lastIndexOf(child);
    moveToPosition = moveFromPosition;
    QPixmap *pixmap = new QPixmap(child->size());
    child->render(pixmap);
    QMimeData *mimeData = new QMimeData;

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*pixmap);
    drag->setHotSpot(QPoint(event->pos().x(),0));
    drag->exec(Qt::MoveAction);


}

void DragScrollArea::dragMoveEvent(QDragMoveEvent *event){
    qDebug()<<"Dragging";
    int positioning = destinationPosition(event->pos());
    qDebug()<<"Going to place at "<<positioning;
    if (moveToPosition!=positioning){
        moveToPosition = positioning;
        QSpacerItem *currSpacer;
        for (int i = 0; i<mainLayout->count(); i=i+2){
            currSpacer = (QSpacerItem*)(mainLayout->itemAt(i));
            if (i == 2*positioning){
                currSpacer->changeSize(1,20);
            }else{
                currSpacer->changeSize(1,2);
            }
        }
        mainLayout->invalidate();
    }
/*
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
     } else {
        child->show();
     }*/
}

void DragScrollArea::dropEvent(QDropEvent *event)
{
    if (event->source() != this)
        return;
    if (event->proposedAction() == Qt::MoveAction) {
        if(moveFromPosition!=moveToPosition){
            reorderWidget(moveFromPosition,moveToPosition);
            emit itemMoved();
        }
        event->setDropAction(Qt::MoveAction);
        event->accept();

        // Process the data from the event.
    }else {
        // Ignore the drop.
        return;
    }

}

void DragScrollArea::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

int DragScrollArea::destinationPosition(QPoint pos){
    if (pos.y()==0)
        return 0;
    QWidget *child = childAt(pos);
    if (child->objectName() == "qt_scrollarea_viewport" || child == mainWidget){
        return destinationPosition(QPoint(pos.x(),pos.y()-1));
    }
    while (child->parent() != mainWidget){
        child = dynamic_cast<QWidget*>(child->parent());
    }
    int widgetPos = widgetList.lastIndexOf(child);
    if (child->mapFrom(this,pos).y()<child->height()/2){
        return widgetPos;
    }else{
        return widgetPos+1;
    }
}


