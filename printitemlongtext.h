#pragma once
#ifndef PRINTITEMLONGTEXT_H
#define PRINTITEMLONGTEXT_H

#include <QObject>
#include <QWidget>
#include "myfield.h"
#include <QFontComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <printitembase.h>

class PrintItemLongText : public PrintItemBase
{
    Q_OBJECT
public:
    PrintItemLongText();
    PrintItemLongText(MyField field);

    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record);

    ~PrintItemLongText(){}

private:

    QFont valueFont;
    bool valueNewLine=true;

    QFontComboBox *valueFontStyle = new QFontComboBox(valueBox);
    QCheckBox * NewLine= new QCheckBox("Print value on a new line",valueBox);
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

#endif // PRINTITEMLONGTEXT_H
