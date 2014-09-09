#include "imageitemdelegate.h"
#include "imagewidgetdelegate.h"
#include <QStyledItemDelegate>
#include <QStyleOptionViewItemV4>
#include <QWidget>
#include <QIcon>
#include <QPixmap>
#include <QVariant>
#include <QPainter>
#include <QRectF>
#include <QRect>
#include <QPointF>
#include <QFontMetrics>
#include <QFont>
#include <QTextLayout>
#include <QTextLine>
#include <QDebug>

ImageItemDelegate::ImageItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ImageItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //The painter status is saved befor painting in order to restore it at the end of the method
    painter->save();

    //the option parameter is set to the correct version
    QStyleOptionViewItemV4 editedOpt = option;


    //The canvas rectangle is taken fromt the option parameter and it's width and height are saved
    QRect avlRect = editedOpt.rect;
    QPoint topLeft = avlRect.topLeft();
    int width=avlRect.width();
    int height = avlRect.height();

    //The text is saved to writeText
    QString writeText=index.data().toString();

    //The image is prepared for painting. It is first casted as a QIcon, and then converted to QPixmap. Finally
    //it is scaled keeping the aspect ration to either the full width or 70% of the height
    //Margins are then calculated using the available width and the image's width
    QIcon icnImage = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QPixmap Image = icnImage.pixmap(200);
    //Image=Image.scaled(width,0.7*height,Qt::KeepAspectRatio);
    Image=Image.scaledToHeight(0.7*height);
    int margin = (width-Image.width())/2;
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
    //The painter object is restored
    painter->restore();

}

//SizeHint is called to determin the size of the item and returns a QSize varaible
QSize  ImageItemDelegate::sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    //The size of the decoration is retrieved this value is taken from the declaration
    //of hte QListWidget setIconSize.
    QSize decSize=option.decorationSize;
    int height = decSize.height();
    //The image is retrieved and type casted to a QIcon
    QIcon icnImage = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    //The Qicon is transformed to a QPixmap
    QPixmap Image = icnImage.pixmap(height);
    //The QPixmap'height is scaled to be equal to 70% of the available height
    //All images are thus the same height and vary only in width
    Image = Image.scaledToHeight(0.7*height);
    //The returned QSize thus has a width equal to the one of the image plus a 20px
    //margin (10px per side) and a heigh equal to th eone set for the decoration
    QSize sizeH=QSize(Image.width()+20,height);
    return sizeH;
}

//This method is called each time an item is being modified. The called item is the created
//ImageWidgetDelegate, the widget is then returned
QWidget * ImageItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{


    //object should probably be used to send the dimension of the widget
    //ImageWidgetDelegate *editWidget = new ImageWidgetDelegate(parent, index, option);
    return new ImageWidgetDelegate(parent, index, option);
}


//This method is called to set the the data within the editor
void ImageItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    static_cast<ImageWidgetDelegate*>(editor)->setText(index.data(Qt::EditRole).toString());
}

//This method is called to get the data from the editor and update it in the index
void ImageItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    QString value=static_cast<ImageWidgetDelegate*>(editor)->getText();
    model->setData(index, value, Qt::EditRole);
}

//This method is called to update the geometry of the edito widget
void ImageItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static_cast<ImageWidgetDelegate*>(editor)->setGeometry(option.rect);
}



