#include "imagewidgetdelegate.h"
#include <QWidget>
#include <QDialog>
#include <QObject>
#include <QSqlRecord>
#include <QString>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QModelIndex>
#include <QIcon>
#include <QSize>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QRect>
#include <QImage>
#include <QPaintEvent>
#include <QDebug>
#include <QTimer>
#include <QPoint>
#include <QTime>
#include <QColor>
#include <QMessageBox>
#include <QVariant>
#include <QGuiApplication>
#include <QScreen>


ImageWidgetDelegate::ImageWidgetDelegate(QWidget *parent, const QModelIndex & index, const QStyleOptionViewItem &option, bool readOnly)
    :QDialog(parent)
    ,txtLabelEdit(new QTextEdit(this))
    ,lblImage(new QLabel(this))
{
    currIndex=index;
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::ClickFocus);
    int width=300;
    currRecord=qvariant_cast<QSqlRecord>(index.data(Qt::UserRole));
    description=currRecord.value("Description").toString();
    //The base layout of the widget is created, and margins are set in order to center it with the underlying
    //item
    //this->setWindowOpacity(50);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *imageLayout =new QHBoxLayout(this);
    imageLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setAlignment(Qt::AlignCenter);

    //The text is set in the QTextEdit from the index
    //txtLabelEdit->setText(index.data().toString());
    //The image is extracted from the index data, first casted to QIcon, then converted to pixmap and set in
    //the QLabel (step not necessary, widget could only cover the text probably)
    //QIcon icnImage=QIcon(currRecord.value("Directory").toString()+currRecord.value("File").toString());
    QPixmap icnImage = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int scrnHeight = screenGeometry.height();
    int scrnWidth = screenGeometry.width();
    icnImage = icnImage.scaled(qMin(icnImage.width(),scrnWidth*2/3),qMin(icnImage.height(),scrnHeight*2/3), Qt::KeepAspectRatio);
    lblImage->setPixmap(icnImage);

    //the QLabel is added to the layout and subsequentelly the QTextEdit
    txtLabelEdit->setText(currRecord.value("Description").toString());
    txtLabelEdit->setReadOnly(readOnly);
    strStatus=currRecord.value("Status").toString();
    imageLayout->addWidget(lblImage);
    mainLayout->addLayout(imageLayout);
    resize(width,3*2/width);
    //mainLayout->addSpacing(0.9*option.decorationSize.height());
    mainLayout->addWidget(txtLabelEdit);
    connect(txtLabelEdit,SIGNAL(textChanged()),this,SLOT(changeDescription()));




}

QSqlRecord ImageWidgetDelegate::getValue(){
    //return QVariant::fromValue(currRecord);
    return currRecord;
}

void ImageWidgetDelegate::changeDescription(){
    currRecord.setValue("Description",txtLabelEdit->toPlainText());
    if (currRecord.value("Status").toString()!="N")
        currRecord.setValue("Status","M");

}

void ImageWidgetDelegate::closeEvent(QCloseEvent *event){
    emit editingFinished(this);
}



//Create a method to retrieve the text fromt the txtLabelEdit in order to update the index data
QString ImageWidgetDelegate::getText() const
{
    return currRecord.value("Description").toString();
    //return txtLabelEdit->toPlainText();
}

//Create a method to set the QTextEdit text
void ImageWidgetDelegate::setText(const QString & text)
{
    txtLabelEdit->setText(currRecord.value("Description").toString());
}

void ImageWidgetDelegate::setStatus(QString newStatus)
{
    currRecord.setValue("Status",newStatus);
}

QString ImageWidgetDelegate::getStatus()
{
    return currRecord.value("Status").toString();
}


//Q_DECLARE_METATYPE(QSqlRecord);
