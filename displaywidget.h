#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVariant>
#include <QStackedWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>

class DisplayWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY(QVariant Value READ getValue WRITE setValue)
public:
    DisplayWidget(QString newFieldName, QString FieldType, QVariant Value, bool Edit=0, QWidget *parent = 0);
    DisplayWidget(QString newFieldName, QString FieldType, bool Edit=0, QWidget *parent = 0);


    inline QVariant getValue(){
        return m_Value;
    }



    void setImages();

    void removeImages();
    void changeImage();
    void removeImage();
    void reload();
    void construct();
    inline void setValue(QVariant newValue){
        m_Value=newValue;
        reload();
    }

signals:

public slots:
    void ValueChange();
    void addImages();


private:
    QWidget *Editable=new QWidget;
    QWidget *NEditable= new QWidget;
    bool isEdit;
    QVariant m_Value;
    QString Type;
    QString Field;
    QLabel *EFieldName;
    QLabel *NFieldName;
    QTextEdit *LongText;
    QLineEdit *ShortText;
    QLabel *TextDisplay;
    QListWidget *ImagesDisplay;
    QListWidget *ImagesEdit;
    QPushButton *AddImages;
    QPushButton *RemoveImages;
    QLabel *ImageDisplay;
    QLabel *ImageEdit;
    QPushButton* ChangeImage;
    QPushButton* RemoveImage;
    QBoxLayout *EditLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *NEditLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    void updateValue();
private slots:


};

#endif // DISPLAYWIDGET_H
