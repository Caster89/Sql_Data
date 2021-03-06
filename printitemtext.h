#pragma once
#ifndef PRINTITEMTEXT_H
#define PRINTITEMTEXT_H

#include <QObject>
#include <QWidget>
#include "printitembase.h"
#include <QGroupBox>
#include <QFont>
#include <QCheckBox>
#include <QTextDocument>
#include <QButtonGroup>

class PrintItemText : public PrintItemBase
{
    Q_OBJECT
public:
    PrintItemText();
    PrintItemText(MyField field);
    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record);
    
    ~PrintItemText(){}
    
private:
    QFont valueFont;
    bool valueNewLine=false;

    //The widgets to set the values properties are prepared

    QFontComboBox *valueFontStyle = new QFontComboBox(valueBox);
    QCheckBox * NewLine = new QCheckBox("Print value on a new line",valueBox);

    QLabel *lblValueFont=new QLabel("Font: ",valueBox);
    QLabel *lblValueSize=new QLabel("Size: ",valueBox);

    QComboBox *valueSize = new QComboBox(valueBox);

    QToolButton *valueBold = new QToolButton(valueBox);
    QToolButton *valueUnderlined = new QToolButton(valueBox);
    QToolButton *valueItalics = new QToolButton(valueBox);

    QToolButton *valueLeft = new QToolButton(valueBox);
    QToolButton *valueCenter = new QToolButton(valueBox);
    QToolButton *valueRight = new QToolButton(valueBox);
    QToolButton *valueJustify = new QToolButton(valueBox);

    QButtonGroup *alignGroup = new QButtonGroup();


    void buildWidget();

    
    
private slots:
    void valueFontChanged();
    void alignmentChanged(int align);
    
};

#endif // PRINTITEMTEXT_H
