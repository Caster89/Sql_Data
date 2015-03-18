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
    void paint(QPainter *p){}

    ~PrintItemImage(){}

private:

    QGroupBox *captionBox=new QGroupBox(valueBox);

    QCheckBox * NewLine = new QCheckBox("Print value on a new line",valueBox);

    QLabel *lblImageSize=new QLabel("Image Size: ",titleBox);
    QComboBox *cmbImageSize=new QComboBox();
    

    QLabel *lblWidth=new QLabel("W: ",valueBox);
    QLineEdit *ledtImageWidth=new QLineEdit(valueBox);
    QLabel *lblHeight= new QLabel("H: ",valueBox);
    QLineEdit *ledtImageHeight=new QLineEdit(valueBox);




    QLabel *lblCaptionPos=new QLabel("Position: ",captionBox);
    QComboBox *cmbCaptionPos= new QComboBox(captionBox);
    QLabel *lblCaptionFont=new QLabel("Font: ",captionBox);
    QFontComboBox *captionFontStyle= new QFontComboBox(captionBox);
    QLabel *lblCaptionSize=new QLabel("Size: ",captionBox);
    QComboBox *captionSize = new QComboBox(captionBox);
    QLabel *lblCaptionPre=new QLabel("Initial String: ",captionBox);
    QLineEdit *ledtCaptionPre = new QLineEdit(captionBox);
    QCheckBox *capNumbered = new QCheckBox("Numbered Images",captionBox);





    QComboBox *imageSize;

    QToolButton *captionBold;
    QToolButton *captionUnderlined;
    QToolButton *captionItalics;

    void buildWidget();

    //
    QFont captionFont;
    bool valueNewLine=true;

    int captionPosition;
    QString captionPrefix;
    //bool

private slots:
    void captionPrintChanged();
    void imagePrintChanged();
};

#endif // PRINTITEMIMAGE_H
