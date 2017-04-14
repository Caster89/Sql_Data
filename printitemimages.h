#pragma once
#ifndef PRINTITEMIMAGES_H
#define PRINTITEMIMAGES_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include "printitembase.h"

class PrintItemImages : public PrintItemBase
{
    Q_OBJECT
public:
    PrintItemImages();
    PrintItemImages(MyField field);
    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record);

    ~PrintItemImages(){}

private:

    QFont captionFont;
    QFont prefixFont;
    int imageAlignment;

    QGroupBox *captionBox=new QGroupBox(valueBox);

    QCheckBox * NewLine = new QCheckBox("Print value on a new line",valueBox);

    QLabel *lblImageSize=new QLabel("Img. Size: ",titleBox);
    QComboBox *cmbImageSize=new QComboBox();


    QLabel *lblWidth=new QLabel("W: ",valueBox);
    QLineEdit *ledtImageWidth=new QLineEdit("100",valueBox);
    QLabel *lblHeight= new QLabel("H: ",valueBox);
    QLineEdit *ledtImageHeight=new QLineEdit("100",valueBox);
    QLabel *lblNumImages=new QLabel("# Img: ",valueBox);
    QLineEdit *ledtNumImages=new QLineEdit("3",valueBox);




    QLabel *lblCaptionPos=new QLabel("Position: ",captionBox);
    QComboBox *cmbCaptionPos= new QComboBox(captionBox);
    QLabel *lblCaptionFont=new QLabel("Font: ",captionBox);
    QFontComboBox *captionFontStyle= new QFontComboBox(captionBox);
    QLabel *lblCaptionSize=new QLabel("Size: ",captionBox);
    QComboBox *captionSize = new QComboBox(captionBox);
    QLabel *lblCaptionPre=new QLabel("Initial String: ",captionBox);
    QLineEdit *ledtCaptionPre = new QLineEdit(captionBox);
    QCheckBox *capNumbered = new QCheckBox("Numbered Images",captionBox);





    //QComboBox *imageSize;

    QToolButton *captionBold =new  QToolButton();
    QToolButton *captionUnderlined=new QToolButton();
    QToolButton *captionItalics=new QToolButton();

    void buildWidget();

    //
    bool valueNewLine=true;

    int captionPosition = 0;
    int imageScaling = 0;
    QString captionPrefix;
    int ImagesPerRow(float avlbSpace, float width);

private slots:
    void captionPrintChanged();
    void imagePrintChanged();
};


#endif // PRINTITEMIMAGES_H
