#include "displaywidget.h"
#include <QVariant>
#include <QLabel>
#include <QStackedWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPixmap>
#include <QListWidgetItem>
#include "medialistwidget.h"
#include <QFileDialog>
#include <imageitemdelegate.h>
#include <QMetaType>
#include <QDebug>
#include <QSpacerItem>
#include <QSqlField>
#include <QSqlRecord>
#include <mysqltablemodel.h>
#include <datatype.h>
#include "displaywidgetimage.h"
#include "displaywidgetimages.h"
#include "displaywidgettext.h"
#include "displaywidgetlongtext.h"
#include "displaywidgetmultiplechoice.h"


DisplayWidget::DisplayWidget(MyField newField, bool Edit,  QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *displayLayout = new QHBoxLayout;
    switch(newField.getType()){
        case(DataType::Text):
            displayWidgetPointer= new DisplayWidgetText(newField, Edit);
            break;
        case(DataType::LongText):
            displayWidgetPointer= new DisplayWidgetLongText(newField, Edit);
            break;
        case(DataType::Image):
            displayWidgetPointer=new DisplayWidgetImage(newField, Edit);
            break;
        case(DataType::Images):
            displayWidgetPointer=new DisplayWidgetImages(newField, Edit);
            break;
        case(DataType::MultipleChoice):
            qDebug()<<newField.getComments();
            displayWidgetPointer=new DisplayWidgetMultipleChoice(newField, Edit);
            break;
    }
    displayLayout->addWidget(displayWidgetPointer);
    this->setLayout(displayLayout);
    this->show();
}



//Q_DECLARE_METATYPE(QVector<QVector<QString> >);
//Q_DECLARE_METATYPE(QVector<QSqlRecord>);
