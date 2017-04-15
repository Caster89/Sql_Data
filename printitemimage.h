#pragma once
#ifndef PRINTITEMIMAGE_H
#define PRINTITEMIMAGE_H

#include <QObject>
#include <QWidget>
#include <printitembase.h>
#include <myfield.h>
#include <QLineEdit>

class PrintItemImage : public PrintItemBase
{
    Q_OBJECT
public:
    PrintItemImage();
    PrintItemImage(MyField field);
    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record);

    ~PrintItemImage(){}

private:

    //QFont captionFont;
    QFont prefixFont;
    int imageAlignment;

    //QGroupBox *captionBox=new QGroupBox(valueBox);

    QCheckBox * NewLine = new QCheckBox("Print value on a new line",valueBox);

    QLabel *lblImageSize=new QLabel("Image Size: ",titleBox);
    QComboBox *cmbImageSize=new QComboBox();
    

    QLabel *lblWidth=new QLabel("W: ",valueBox);
    QLineEdit *ledtImageWidth=new QLineEdit(valueBox);
    QLabel *lblHeight= new QLabel("H: ",valueBox);
    QLineEdit *ledtImageHeight=new QLineEdit(valueBox);

    QToolButton *valueLeft = new QToolButton(valueBox);
    QToolButton *valueCenter = new QToolButton(valueBox);
    QToolButton *valueRight = new QToolButton(valueBox);
    QToolButton *valueJustify = new QToolButton(valueBox);

    QButtonGroup *alignGroup = new QButtonGroup();
    /*
    QLabel *lblCaptionPos=new QLabel("Position: ",captionBox);
    QComboBox *cmbCaptionPos= new QComboBox(captionBox);
    QLabel *lblCaptionFont=new QLabel("Font: ",captionBox);
    QFontComboBox *captionFontStyle= new QFontComboBox(captionBox);
    QLabel *lblCaptionSize=new QLabel("Size: ",captionBox);
    QComboBox *captionSize = new QComboBox(captionBox);
    QLabel *lblCaptionPre=new QLabel("Initial String: ",captionBox);
    QLineEdit *ledtCaptionPre = new QLineEdit(captionBox);
    QCheckBox *capNumbered = new QCheckBox("Numbered Images",captionBox);
    */




    //QComboBox *imageSize;
    /*
    QToolButton *captionBold =new  QToolButton();
    QToolButton *captionUnderlined=new QToolButton();
    QToolButton *captionItalics=new QToolButton();
    */
    void buildWidget();

    //
    bool valueNewLine=true;
    int imageScaling = 0;

    //int captionPosition;
    //QString captionPrefix;
    //bool

private slots:
    //void captionPrintChanged();
    void imagePrintChanged();
    void alignmentChanged(int align);

};

#endif // PRINTITEMIMAGE_H
