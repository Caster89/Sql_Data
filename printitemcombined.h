#ifndef PRINTITEMCOMBINED_H
#define PRINTITEMCOMBINED_H

#include <QObject>
#include <QWidget>
#include "printitembase.h"
#include "myfield.h"
#include <QComboBox>
#include <QStringList>
#include <QLineEdit>
#include <QScrollArea>
#include <QPushButton>


class PrintItemCombined : public PrintItemBase
{
    Q_OBJECT
public:
    PrintItemCombined();
    PrintItemCombined(MyField newField, QWidget *parent = 0);
    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record);

    ~PrintItemCombined(){}

private:
    QFont valueFont;
    bool valueNewLine=false;

    QList<QString> avlbFields;
    QComboBox *baseCMB = new QComboBox();
    QList<QComboBox*> selectedFields;
    QList<QLineEdit*> separators;
    QLineEdit *ledtTitleName = new QLineEdit();
    QLabel *lblTitleName = new QLabel("Field Title: ");
    QLineEdit *numbFields = new QLineEdit();
    QScrollArea *scrollWidget = new QScrollArea();
    QWidget *listWidget = new QWidget();
    //QVBoxLayout *stackLayout = new QVBoxLayout();
    QVBoxLayout *scrollLayout = new QVBoxLayout();
    //QFontComboBox *valueFontStyle = new QFontComboBox();
    QCheckBox * NewLine = new QCheckBox("Print value on a new line");

    QToolButton *valueBold = new QToolButton();
    QToolButton *valueUnderlined = new QToolButton(valueBox);
    QToolButton *valueItalics = new QToolButton(valueBox);

    QToolButton *valueLeft = new QToolButton(valueBox);
    QToolButton *valueCenter = new QToolButton(valueBox);
    QToolButton *valueRight = new QToolButton(valueBox);
    QToolButton *valueJustify = new QToolButton(valueBox);

    QButtonGroup *alignGroup = new QButtonGroup();

    QLabel *lblValueFont = new QLabel("Font: ");

    QLabel *lblValueSize=new QLabel("Size: ");
    QFontComboBox *valueFontStyle = new QFontComboBox();

    QComboBox *valueSize = new QComboBox(valueBox);
    QPushButton *btnAddField = new QPushButton("+");
    QPushButton *btnRemoveField = new QPushButton("-");

/*
    QLabel *lblValueFont=new QLabel("Font: ",valueBox);
    QLabel *lblValueSize=new QLabel("Size: ",valueBox);

    QComboBox *valueSize = new QComboBox(valueBox);




    QToolButton *valueLeft = new QToolButton(valueBox);
    QToolButton *valueCenter = new QToolButton(valueBox);
    QToolButton *valueRight = new QToolButton(valueBox);
    QToolButton *valueJustify = new QToolButton(valueBox);


    */
    void buildWidget();

private slots:
    void addField();
    void removeField();

    void valueFontChanged();
    void alignmentChanged(int align);
};

#endif // PRINTITEMCOMBINED_H
