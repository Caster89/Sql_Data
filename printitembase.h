
#ifndef PRINTITEMBASE_H
#define PRINTITEMBASE_H

#include <QObject>
#include "myfield.h"
#include <datatype.h>
#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QGroupBox>
#include <QFont>
#include <QCheckBox>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>
#include <QStringList>
#include <QLabel>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLayout>
#include "mystackedwidget.h"

class PrintItemBase: public QFrame
{
    Q_OBJECT
public:
    PrintItemBase();

    PrintItemBase(MyField Field);

    PrintItemBase(const PrintItemBase &orgItem);

    PrintItemBase &operator=(const PrintItemBase &orgItem);

    virtual void paint(QPainter *p){}


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

    //The data varaibles are created as protected
    MyField field;
    DataType::dataType type;
    QFont titleFont;
    bool showTitle;
    bool itemOnNewLine;
    bool expanded=true;
    QStringList fontSize;

    //The labels for the expanded and closed widget are created
    //and arrow (Up of right) and a label with the name of the field
    QLabel *lblTitle = new QLabel(this);
    //QLabel *lblArrow=new QLabel(QChar(9,207));
    QLabel *lblArrow=new QLabel("q");
    QLabel *lblClose = new QLabel("X");

    //The three basic widgets ar created, a stacked widget
    //to switch between the expanded and the closed widget

    QFrame *editWidget=new QFrame();

    //Two groupboxes are created, one for editing
    //the font of the title and on the value
    QGroupBox *titleBox=new QGroupBox(this);
    QGroupBox *valueBox=new QGroupBox(this);

    //The layout for the open widget is created, a vertical
    //box layout
    QVBoxLayout *editLayout = new QVBoxLayout(this);
    QVBoxLayout *titleLayout = new QVBoxLayout(titleBox);
    QVBoxLayout *valueLayout = new QVBoxLayout(valueBox);
    QVBoxLayout *boxLayout = new QVBoxLayout();








    //The widgets for setting the style of the title are created
    //QCheckbox for selecting whether to print or not
    //QFontComboBox to select the font
    //QComboBox to select the size
    //3 QToolButtons so select B I U
    QCheckBox *titleVisible=new QCheckBox("Print Field Name",titleBox);
    QFontComboBox *titleFontStyle=new QFontComboBox(titleBox);
    QLabel *lblTitleFont=new QLabel("Font ",titleBox);
    QLabel *lblTitleSize=new QLabel("Size: ",titleBox);
    QComboBox *titleSize=new QComboBox(titleBox);
    QToolButton *titleBold=new QToolButton(titleBox);
    QToolButton *titleUnderlined=new QToolButton(titleBox);
    QToolButton *titleItalics=new QToolButton(titleBox);

    void buildWidget();
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    QSize sizeHint();
    
private slots:
    void titlePrintChanged();
    void titleFontChanged();


};

#endif // PRINTITEMBASE_H
