#pragma once
#ifndef LABELEDTEXT_H
#define LABELEDTEXT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
#include <QListWidget>
#include <QIcon>
#include <QBoxLayout>
#include <QPushButton>
#include <datatype.h>
#include "medialistwidget.h"
//#include "metatypedeclaration.h"

class LabeledText : public QWidget
{
    Q_OBJECT
public:
    LabeledText(QWidget *parent,DataType::dataType text_type,QString label);
    void setValue(QString newValue);
    void setField(QString newField);
    void setTable(QString newTable);
    void setPrimary(int newPrimary);
    void setEditable(bool editable);
    bool eventFilter(QObject * o, QEvent *e);

    QString getLabel();
    QVariant getValue();
    DataType::dataType getType();
    QVector<QVector<QVector<QString> > > getImage();
    QString getField();
    QString getTable();
    int getPrimary();

signals:

public slots:

private slots:
    void addImage();
    void ChangeImage();
    void update();

private:

    QString Value;
    QLabel *txtlabel;
    QLabel *txtNonEdit;
    QLabel *lblImage;
    QTextEdit *txtEdit;
    QLineEdit *lnEdit;
    MediaListWidget *imgEdit;
    DataType::dataType txtType;
    QVector<QVector<QVector<QString> > > Images;
    QString Field_name;
    QString Table_name;
    int Primary;
    QBoxLayout *mainLayout;
    bool isEditable;
    QPushButton *btnAddImage;
    QPushButton *btnRemoveImage;



};

#endif // LABELEDTEXT_H
