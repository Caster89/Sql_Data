#include "previewpopup.h"
#include "labeledtext.h"
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QModelIndex>
#include <QTextEdit>
#include <QDebug>
#include <QList>
#include <QSize>
#include <QScrollBar>
#include <QEvent>
#include <QMessageBox>
#include <QRect>
#include <QPoint>
#include <QApplication>
#include <QDesktopWidget>
#include <QSqlRecord>

PreviewPopup::PreviewPopup(QWidget *parent, QSqlRecord sel_record ,QVector<QLabel*> prwFields, QVector<QVector<QString> > Fields)
    : QWidget(parent)
{
    QVBoxLayout *scrLayout = new QVBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QScrollArea *scrArea = new QScrollArea(this);
    QWidget *mainWidget = new QWidget(scrArea);
    QVector<LabeledText*> prwValues;

        for (int i=0;i<Fields.size();i++){
            //If the field has to be previewd in the preview frame then the field name label is added
            if (Fields[i][3].toInt())
            {

            //If the field is marked as text then the lable position is increaed by one and the text is set on the
            //same row of the label
                if(Fields[i][2].toInt()==DataType::Text){
                    prwValues.append(new LabeledText(scrArea,DataType::Text,Fields[i][0]));
                    prwValues.last()->setValue(sel_record.value(i).toString());
                    prwValues.last()->setEditable(false);
                    scrLayout->addWidget(prwValues[i]);
                    //If the field is marked as long text or image the label position is increasd by 4 and the label is
            //set one row below the label
                }else if(Fields[i][2].toInt()==DataType::LongText ){
                    prwValues.append(new LabeledText(scrArea,DataType::LongText,Fields[i][0]));
                    prwValues.last()->setValue(sel_record.value(i).toString());
                    prwValues.last()->setEditable(false);
                    scrLayout->addWidget(prwValues[i]);

                }else if(Fields[i][2].toInt()==DataType::Image){
                    prwValues.append(new LabeledText(scrArea,DataType::Image,Fields[i][0]));
                    prwValues.last()->setEditable(false);
                    scrLayout->addWidget(prwValues[i]);
                }
            }


    }
        mainWidget->setLayout(scrLayout);
        mainWidget->setMinimumWidth(200);
        scrArea->setWidget(mainWidget);
        mainLayout->addWidget(scrArea);
        mainLayout->setContentsMargins(0,0,0,0);
        setLayout(mainLayout);
        setContentsMargins(0,0,0,0);
        QRect deskRect = QApplication::desktop()->screenGeometry();
        QPoint curPosition = QCursor::pos();
        int xpos = curPosition.x();
        int ypos = curPosition.y();

        if(xpos>deskRect.width()-300)
            xpos=xpos-300;
        if(ypos>deskRect.height()-400)
            ypos=deskRect.height()-420;

        move(xpos,ypos);


}

void PreviewPopup::closeEvent(QCloseEvent *closeEv)
{
    emit popupClosed();

    QWidget::closeEvent(closeEv);
}

QSize PreviewPopup::sizeHint() const
{
    return QSize(300,400);
}

bool PreviewPopup::eventFilter(QObject *o, QEvent *e)
{

    if(e->type()==QEvent::MouseButtonDblClick)
    {
        QMessageBox *msgBox=new QMessageBox();
        msgBox->setText("double click");
        msgBox->show();
        return true;
    }
    return false;

}
