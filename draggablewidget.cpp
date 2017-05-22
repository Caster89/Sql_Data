#include "draggablewidget.h"
#include <QHBoxLayout>
#include <QDebug>


DraggableWidget::DraggableWidget(QWidget *parent) : QFrame(parent)
{
    qDebug()<<"DraggableWidget::DraggableWidget()";
    buildWidget();

}

void DraggableWidget::buildWidget(){
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::StyledPanel);

    //A basic layout is created to for the arrow and title used
    //to expand and contract the widget
    // _______________________________________
    //|_|>_________________________________X__|
    QHBoxLayout *arrowLayout=new QHBoxLayout();
    QFont arrowFont("Wingdings 3");
    lblArrow->setFont(arrowFont);
    lblArrow->installEventFilter(this);

    lblArrow->setMargin(1);
    arrowLayout->addWidget(lblArrow);
    arrowLayout->addWidget(lblTitle);
    arrowLayout->addStretch(0);
    arrowLayout->addWidget((lblClose));
    lblClose->setVisible(false);
    lblClose->installEventFilter(this);
    arrowLayout->setSpacing(1);
    arrowLayout->setContentsMargins(1,1,1,1);
    arrowLayout->setSizeConstraint(QLayout::SetMaximumSize);

    QFrame *lineFrame=new QFrame();
    lineFrame->setFrameShape(QFrame::HLine);
    //The editLayout is populated
    mainLayout->setMargin(1);
    mainLayout->setSpacing(1);
    mainLayout->addWidget(lineFrame);
    mainLayout->addLayout(arrowLayout);
    mainLayout->addWidget(disappearingWidget);
    setLayout(mainLayout);
}

bool DraggableWidget::eventFilter(QObject *obj, QEvent *event){

    if(obj==lblArrow && event->type()==QEvent::MouseButtonRelease){
        //qDebug()<<"There are "<<disappearingLayout->count()<<"objects in layout";
        if(expanded ){
            disappearingWidget->setVisible(false);
            lblArrow->setText(rightTri);
            expanded=false;
            /*for(int i=0;i<disappearingLayout->count();i++){
                disappearingLayout->itemAt(i)->widget()->setVisible(false);
                lblArrow->setText(rightTri);
                expanded=false;
            }*/

        }else if(!expanded){
            disappearingWidget->setVisible(true);
            lblArrow->setText(downTri);
            expanded=true;
            /*for(int i=0;i<disappearingLayout->count();i++){
                disappearingLayout->itemAt(i)->widget()->setVisible(true);
                lblArrow->setText(downTri);
                expanded=true;
            }*/
        }

        return true;
    }else if (obj==lblClose && event->type()==QEvent::MouseButtonRelease){
        qDebug()<<"DraggableWidget:closeWidget()";
        emit closeWidget();
    }

    return QWidget::eventFilter(obj,event);
}

void DraggableWidget::enterEvent(QEvent *event){
    lblClose->setVisible(true);
    QWidget::enterEvent(event);
}

void DraggableWidget::leaveEvent(QEvent *event){
    lblClose->setVisible(false);
    QWidget::leaveEvent(event);
}
