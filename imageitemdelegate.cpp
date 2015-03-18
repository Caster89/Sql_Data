#include "imageitemdelegate.h"
#include "imagewidgetdelegate.h"
#include <QStyledItemDelegate>
#include <QSqlRecord>
#include <QStyleOptionViewItemV4>
#include <QWidget>
#include <QIcon>
#include <QPixmap>
#include <QImage>
#include <QVariant>
#include <QPainter>
#include <QRectF>
#include <QRect>
#include <QPointF>
#include <QPoint>
#include <QFontMetrics>
#include <QFont>
#include <QTextLayout>
#include <QTextLine>
#include <QDebug>
#include <QTime>
#include <QPainterPath>
#include <QRadialGradient>
#include <QApplication>
#include <QPushButton>

ImageItemDelegate::ImageItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    origParent=parent;
}

void ImageItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //The painter status is saved befor painting in order to restore it at the end of the method
    if(index.data(Qt::UserRole).canConvert<QSqlRecord>()){
        QSqlRecord currValue=qvariant_cast<QSqlRecord>(index.data(Qt::UserRole));
        painter->save();

        //the option parameter is set to the correct version
        QSize avlbSpace=origParent->size();

        //The canvas rectangle is taken fromt the option parameter and it's width and height are saved
        //QRect avlRect = QRect();
        //QPoint topLeft = avlRect.topLeft();
        int width=avlbSpace.width()/3;
        int height = avlbSpace.height();

        //the option parameter is set to the correct version
        QStyleOptionViewItemV4 editedOpt = option;


        //The canvas rectangle is taken fromt the option parameter and it's width and height are saved
        QRect avlRect = editedOpt.rect;
        QPoint topLeft = avlRect.topLeft();


        //The text is saved to writeText
        //QString writeText=index.data().toString();
        QString writeText=currValue.value("Description").toString();

    //The image is prepared for painting. It is first casted as a QIcon, and then converted to QPixmap. Finally
    //it is scaled keeping the aspect ration to either the full width or 70% of the height
    //Margins are then calculated using the available width and the image's width
        QString fileDir=currValue.value("Directory").toString();
        fileDir.append(currValue.value("File").toString());
        QIcon icnImage(fileDir);
        QPixmap Image = icnImage.pixmap(200);

    //Image=Image.scaled(width,0.7*height,Qt::KeepAspectRatio);

        Image=Image.scaledToHeight(0.7*height);

        //int margin = (width-Image.width())/2;
        QPoint imgTopLeft = QPoint(topLeft.x()+margin,topLeft.y()+2);
        QPoint txtTopLeft = QPoint(topLeft.x()+1,topLeft.y()+Image.height());

        //A QFontMetrics item is initalized in order to determin how the text is going to be split when drawn
        //the number of lines are calculated using the total width of the text and the total width of the window
        QFontMetrics fm= QFontMetrics(editedOpt.font);
        int numLines=fm.width(writeText)/width +1;

        //The following objects are created to determin where the text should be elided. usedWidth will keep the
        //current width of the text up to the read line. QTextLayout is used to store the text line by line
        //and availableLines is calculated by dividing the height available to the text (30%) by the height of the
        //font
        qreal usedWidth=0;
        QTextLayout textLayout(writeText);
        textLayout.setFont(editedOpt.font);
        textLayout.beginLayout();
        int i=0;
        int availableLines = 0.3*height/fm.height();

        //Two different items can be painted, one when the item is selected and the other when the item is not
        //selected.
        //We first paint this second option
        if (option.state & QStyle::State_Selected)
        {

            //When the item is selected we want to paint as many lines as possible, so the function is iterated
            //from 0 to availableLines
            while(i<availableLines){
                //each line is created and the width of the line is set, the natural length the line would occupy
                //is then added to the usedWidth
                QTextLine line = textLayout.createLine();
                if (!line.isValid())
                        break;
                line.setLineWidth(width);
                usedWidth += line.naturalTextWidth();
                i++;
            }
            textLayout.endLayout();

            //The whole available rectangle is painted with the styles highlighted color
            painter->fillRect(avlRect,editedOpt.palette.color(QPalette::Highlight));
            //The text is drawn, starting from 70% of the height down + 5pt with 2pt margins per side.
            //The text is wordWrapped, and it is elided after a length of usedWidth, which will corrisponf to
            //a number of lines equal to availableLines
            painter->drawText(QRect(txtTopLeft,QPoint(txtTopLeft.x()-2+width,height)), Qt::TextWordWrap,fm.elidedText(writeText,Qt::ElideRight,usedWidth));
            //The image is drawn with the top-right corner 2pt from the top and "margin" points from the left.
            //painter->drawPixmap(width*index.row()+margin,2,Image);
            painter->drawPixmap(imgTopLeft,Image);

        } else {
            //If the item is not selected we want to draw less text lines, because of this we choos the minor
            //between half of the avalableLines, or 2
            while(i<std::min(availableLines/2,2)){
                //each line is created and the width of the line is set, the natural length the line would occupy
                //is then added to the usedWidth
                QTextLine line = textLayout.createLine();
                if (!line.isValid())
                        break;
                line.setLineWidth(width);
                usedWidth += line.naturalTextWidth();
                i++;
            }

        //The text is drawn, starting from 70% of the height down + 5pt with 2pt margins per side.
        //The text is wordWrapped, and it is elided after a length of usedWidth, which will corrispond to
        //a number of lines equal to either availableLines/2 or 2
        painter->drawText(QRect(txtTopLeft,QPoint(txtTopLeft.x()-4+width,height)), Qt::TextWordWrap,fm.elidedText(writeText,Qt::ElideRight,usedWidth));
        //The image is drawn with the top-right corner 2pt from the top and "margin" points from the left.
        //painter->drawPixmap(width*index.row()+margin,2,Image);
        painter->drawPixmap(imgTopLeft,Image);
        }
        if(option.state & QStyle::State_MouseOver ){
             //&& option.state & QStyle::State_Selected
            //*******************************************************************//
            //Create the remove button image by creating a cirlce. The placement //
            //is obtained using the posDelete function. The top left corner is   //
            //set by taking the right margin and moving left of a value equal to //
            //20 px plus the margin. The minus sign is made by creating a        //
            //rectangular path, the left x is equal to the topL.x() plus margin, //
            // the top is equal to topL.y() plus 10. The width is equal to the   //
            //complete width minus the margin, and the height is 5.
            QPoint topL=posDelete(option);
            QPushButton *btnDelete = new QPushButton();

            QRect drawRec(topL,QSize(30,30));
            //QPainterPath circlePath;
            //QPainterPath minusSign;
            QIcon rmvIcn(":/Rmv_Round_Btn");

            //minusSign.addRect(topL.x()+margin,topL.y()+10,25-2*margin,5);
            //circlePath.addEllipse(topL.x(),topL.y(),25,25);
            //QRadialGradient delGradient =QRadialGradient(QPointF(topL.x(),topL.y()),30);
            //delGradient.setFocalPoint(topL.x()+option.rect.height()/12,topL.y()+option.rect.height()/12);
            //delGradient.setColorAt(0.0, "#B20000");
            //delGradient.setColorAt(1.0, "#FF1919");
            //painter->setBrush(delGradient);
            //painter->drawPath(circlePath);
            //painter->setBrush(QColor("#FFFFFF"));
            //painter->drawPath(minusSign);
            //painter->drawPixmap(topL.x(),topL.y(),30,30,rmvIcn);
            //painter->drawImage(30,30,rmvIcn);
            rmvIcn.paint(painter,drawRec);


        }
        //The painter object is restored
        painter->restore();
    }else {
        QStyledItemDelegate::paint(painter, option, index);
    }

}
QPoint ImageItemDelegate::posDelete(const QStyleOptionViewItem &option) const{
    return QPoint(option.rect.right() - 30 - margin,
                          option.rect.top() +margin);
}

void ImageItemDelegate::buttonClicked(){
    qDebug()<<"Del buton clicked";
}



bool ImageItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index){
    QRect closeButtonRect = QRect(0,0,25,25).translated(posDelete(option));

    // Emit a signal when the icon is clicked

    if(!index.parent().isValid() && event->type()==QEvent::MouseMove){
        if(index.data(Qt::UserRole).canConvert<QSqlRecord>()){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(closeButtonRect.contains(mouseEvent->pos())){
                QApplication::setOverrideCursor(Qt::PointingHandCursor);
            }else{

                QApplication::restoreOverrideCursor();

            }
            QApplication::processEvents();
    }
    }


    if(!index.parent().isValid() &&
            event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);



        if(closeButtonRect.contains(mouseEvent->pos()))
        {

            if(index.data(Qt::UserRole).canConvert<QSqlRecord>()){
                ImageWidgetDelegate *editor=new ImageWidgetDelegate(0,index,option);
                editor->setStatus("R");
                updateObject(editor);
                return true;

            }
            return QStyledItemDelegate::editorEvent(event,model,option,index);
        }
        //return QStyledItemDelegate::editorEvent(event,model,option,index);

    }
    if(event->type()==QEvent::MouseButtonDblClick){
        ImageWidgetDelegate *editor=new ImageWidgetDelegate(0,index,option);
        connect(editor,SIGNAL(editingFinished(ImageWidgetDelegate*)),this,SLOT(updateObject(ImageWidgetDelegate*)));
        //connect(editor,SIGNAL(editingFinished()),this,SLOT(updateObject(editor)));
        editor->setWindowModality(Qt::WindowModal);
        editor->exec();
        return true;
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

//SizeHint is called to determin the size of the item and returns a QSize varaible
QSize  ImageItemDelegate::sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const
{

    //the option parameter is set to the correct version
    if(index.data(Qt::UserRole).canConvert<QSqlRecord>()){
        QSize avlbSpace=origParent->size();

        QSqlRecord currValue=qvariant_cast<QSqlRecord>(index.data(Qt::UserRole));
        QString fileDir=currValue.value("Directory").toString();
        fileDir.append(currValue.value("File").toString());
        QIcon icnImage(fileDir);
        QPixmap Image = icnImage.pixmap(200);

        //The canvas rectangle is taken fromt the option parameter and it's width and height are saved
        //QRect avlRect = QRect();
        //QPoint topLeft = avlRect.topLeft();
        int height = avlbSpace.height();
        Image=Image.scaledToHeight(height*0.7);
        int width=Image.width();

        return QSize(width+2*margin,height);
    }else{
        return QStyledItemDelegate::sizeHint(option,index);
    }
}

void ImageItemDelegate::updateObject(ImageWidgetDelegate *editor){
    //qDebug()<<"Updating Object";
    emit commitData(editor);
    emit closeEditor(editor,NoHint);
}

//This method is called each time an item is being modified. The called item is the created
//ImageWidgetDelegate, the widget is then returned
/*QWidget * ImageItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    //return QStyledItemDelegate::createEditor(parent, option, index);
    //object should probably be used to send the dimension of the widget
    //ImageWidgetDelegate *editWidget = new ImageWidgetDelegate(parent, index, option);
    /*ImageWidgetDelegate *imgDel=new ImageWidgetDelegate(parent,index,option);
    imgDel->setWindowModality(Qt::WindowModal);
    imgDel->show();
    QWidget *blankWidg=new QWidget();
    //imgDel->show();
    return imgDel;
}*/


//This method is called to set the the data within the editor
void ImageItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSqlRecord currRecord = qvariant_cast<QSqlRecord>(index.data(Qt::UserRole));
    static_cast<ImageWidgetDelegate*>(editor)->setText(currRecord.value("Description").toString());
}

//This method is called to get the data from the editor and update it in the index
void ImageItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    qDebug()<<"setting Model Data";
    //QString value=static_cast<ImageWidgetDelegate*>(editor)->getText();
    QSqlRecord values=dynamic_cast<ImageWidgetDelegate*>(editor)->getValue();
    //values.setValue("Description",value);
    model->setData(index, QVariant::fromValue(values), Qt::UserRole);
}

//This method is called to update the geometry of the edito widget
void ImageItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static_cast<ImageWidgetDelegate*>(editor)->setGeometry(option.rect);
}


Q_DECLARE_METATYPE(QSqlRecord);
