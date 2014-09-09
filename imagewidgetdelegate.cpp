#include "imagewidgetdelegate.h"
#include <QWidget>
#include <QObject>
#include <QString>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QModelIndex>
#include <QIcon>
#include <QSize>
#include <QPixmap>
#include <QDebug>

ImageWidgetDelegate::ImageWidgetDelegate(QWidget *parent, const QModelIndex & index, const QStyleOptionViewItem &option)
    :QWidget(parent)
    ,txtLabelEdit(new QTextEdit(this))
    ,lblImage(new QLabel(this))
{
    //The base layout of the widget is created, and margins are set in order to center it with the underlying
    //item
    //this->setWindowOpacity(50);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5,2,0,0);

    //The text is set in the QTextEdit from the index
    txtLabelEdit->setText(index.data().toString());
    //The image is extracted from the index data, first casted to QIcon, then converted to pixmap and set in
    //the QLabel (step not necessary, widget could only cover the text probably)
    QIcon icnImage=qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    //lblImage->setPixmap(icnImage.pixmap(200));
    //the QLabel is added to the layout and subsequentelly the QTextEdit
    //mainLayout->addWidget(lblImage);
    mainLayout->addSpacing(0.9*option.decorationSize.height());
    mainLayout->addWidget(txtLabelEdit);
}

//Create a method to retrieve the text fromt the txtLabelEdit in order to update the index data
QString ImageWidgetDelegate::getText() const
{

    return txtLabelEdit->toPlainText();
}

//Create a method to set the QTextEdit text
void ImageWidgetDelegate::setText(const QString & text)
{
    txtLabelEdit->setText(text);
}

void ImageWidgetDelegate::setStatus(QString newStatus)
{
    strStatus=newStatus;
}

QString ImageWidgetDelegate::getStatus()
{
    return strStatus;
}
