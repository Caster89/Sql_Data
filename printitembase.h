#pragma once
#ifndef PRINTITEMBASE_H
#define PRINTITEMBASE_H

#include <QObject>
#include "myfield.h"
#include <QSqlRecord>
#include <datatype.h>
#include <QFrame>
#include <QWidget>
#include <QPainter>
#include <QGroupBox>
#include <QFont>
#include <QCheckBox>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QStringList>
#include <QLabel>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLayout>
#include "mystackedwidget.h"
#include "metatypedeclaration.h"
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#include <QTextFrame>
#include <QTextCursor>
#include <QTextDocument>
#include <QMap>
#include "draggablewidget.h"
#endif


class PrintItemBase: public DraggableWidget
{
    Q_OBJECT
public:
    PrintItemBase(QWidget *parent = 0);

    PrintItemBase(MyField newField, QWidget *parent = 0);

    PrintItemBase(const PrintItemBase &orgItem);

    PrintItemBase &operator=(const PrintItemBase &orgItem);

    virtual void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record){}


    virtual ~PrintItemBase(){ }

    inline DataType::dataType getType(){
        return type;
    }

    inline QFrame* getWidget() const{
        return editWidget;
    }

    bool eventFilter(QObject *obj, QEvent *event);

    void setTitleFont(QFont f);


protected:
    /*QWidget *basicWidget = new QWidget();
    QVBoxLayout *baseLayout = new QVBoxLayout();
    QGroupBox *baseGroup = new QGroupBox();
    QGroupBox *derivedGroup = new QGroupBox();*/
    //col 25B row 6
    QChar rightTri = QChar(0xB6,0x25);
    //col 25B row C
    QChar downTri = QChar(0xBC,0x25);
    enum imageSize{
        FullPage=0,
        FullWidth,
        FixedHeight,
        FixedWidth,
        WidthHeight
    };

    //The data varaibles are created as protected
    MyField field;
    DataType::dataType type;
    QFont titleFont;
    bool showTitle;
    bool itemOnNewLine;
    bool expanded=true;

    QStringList fontSize;
    Qt::AlignmentFlag alignment=Qt::AlignLeft;
    Qt::AlignmentFlag titleAlignment=Qt::AlignLeft;

    QMap<int, Qt::AlignmentFlag> *alignMap = new QMap<int, Qt::AlignmentFlag>;
    //The labels for the expanded and closed widget are created
    //and arrow (Up of right) and a label with the name of the field
    QLabel *lblTitle = new QLabel(this);
    //QLabel *lblArrow=new QLabel(QChar(9,207));
    //QLabel *lblArrow=new QLabel("q");
    QLabel *lblArrow = new QLabel(downTri);
    QLabel *lblClose = new QLabel("X");

    //The three basic widgets ar created, a stacked widget
    //to switch between the expanded and the closed widget

    QFrame *editWidget=new QFrame();

    //Two groupboxes are created, one for editing
    //the font of the title and on the value
    QGroupBox *titleBox=new QGroupBox();
    QGroupBox *valueBox=new QGroupBox();

    //The layout for the open widget is created, a vertical
    //box layout
    QVBoxLayout *editLayout = new QVBoxLayout(this);
    QVBoxLayout *titleLayout = new QVBoxLayout(titleBox);
    QVBoxLayout *valueLayout = new QVBoxLayout(valueBox);
    QVBoxLayout *boxLayout = new QVBoxLayout();

    QCheckBox *previousLine = new QCheckBox("Print on previous line:");

    //The widgets for setting the style of the title are created
    //QCheckbox for selecting whether to print or not
    //QFontComboBox to select the font
    //QComboBox to select the size
    //3 QToolButtons to select B I U
    //3 QToolButtons to select alignment
    QCheckBox *titleVisible=new QCheckBox("Print Field Name",titleBox);
    QFontComboBox *titleFontStyle=new QFontComboBox(titleBox);
    QLabel *lblTitleFont=new QLabel("Font ",titleBox);
    QLabel *lblTitleSize=new QLabel("Size: ",titleBox);
    QComboBox *titleSize=new QComboBox(titleBox);
    QToolButton *titleBold=new QToolButton(titleBox);
    QToolButton *titleUnderlined=new QToolButton(titleBox);
    QToolButton *titleItalics=new QToolButton(titleBox);

    QToolButton *titleLeft = new QToolButton(valueBox);
    QToolButton *titleCenter = new QToolButton(valueBox);
    QToolButton *titleRight = new QToolButton(valueBox);
    QToolButton *titleJustify = new QToolButton(valueBox);

    QButtonGroup *titleAlignGroup = new QButtonGroup();

    void buildWidget();
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    QSize sizeHint();
    
private slots:
    void titlePrintChanged();
    void titleFontChanged();
    void titleAlignmentChanged(int align);
    void positioningChanged();

signals:
    void closeWidget();
    void itemModified();
};

#endif // PRINTITEMBASE_H
