#ifndef FIELDITEM_H
#define FIELDITEM_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QMap>
#include "myfield.h"
#include "draggablewidget.h"

class FieldItem : public DraggableWidget
{
    Q_OBJECT
public:
    FieldItem(QWidget *parent = 0);

    FieldItem(MyField field, QWidget *parent = 0);

    void buildWidget();
    FieldEdit changes();

    inline QString getAction(){
        return fieldEdit.getAction();
        //return action;
    }

    inline void setAction(QString newAction){
        fieldEdit.setAction(newAction);
        action = newAction;
        qDebug()<<"Setting action to :"<<newAction;
        if (fieldEdit.getAction() == "Remove"){
            setVisible(false);
        }
    }

    inline MyField getField(){
        return fieldEdit.getField();
        //return editedField;
    }

    inline MyField getOldField(){
        return fieldEdit.getOldField();
        //return origField;
    }


    /*foreach(QString key, dataTypeMap.keys()){
            cmbFieldType->addItem(key, dataTypeMap.value(key));
        }*/
    void setPosition(int i);

private:
    QGridLayout *fieldLayout = new QGridLayout();
    QComboBox  *cmbFieldType = new QComboBox();
    //QCheckBox *ckbFieldPrimary = new QCheckBox("Primary Field");
    QCheckBox *ckbFieldVisTable = new QCheckBox("Visible in Table");
    QCheckBox *ckbFieldVisPreview = new QCheckBox("Visible in Preview");
    QCheckBox *ckbFieldUnique = new QCheckBox("Unique Field");
    QLineEdit *ledtFieldName = new QLineEdit();
    QMap<QString, DataType::dataType> dataTypeMap{{"Text",DataType::Text},
                                   {"Long Text" ,DataType::LongText},
                                   {"Image", DataType::Image},
                                   {"Images", DataType::Images,},
                                   {"Multiple Choice", DataType::MultipleChoice}};

    FieldEdit fieldEdit;
    QString action ="";
    MyField origField;
    MyField editedField;








private slots:
    void nameChanged();

    void positionChanged();

    void typeChanged();

    //void principalChanged();

    void visTableChanged();

    void visPreviewChanged();

    void uniqueChanged();

    void commentsChanged();

};

#endif // FIELDITEM_H
