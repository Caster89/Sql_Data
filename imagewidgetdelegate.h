#pragma once
#ifndef IMAGEWIDGETDELEGATE_H
#define IMAGEWIDGETDELEGATE_H
#include <QWidget>
#include <QDialog>
#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QImage>
#include <QFocusEvent>
#include <QCloseEvent>
#include <QVariant>
#include <QSqlRecord>
#include "metatypedeclaration.h"

class ImageWidgetDelegate: public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QSqlRecord currValue READ getValue WRITE setValue)
public:
    ImageWidgetDelegate(QWidget *parent, const QModelIndex & index, const QStyleOptionViewItem &option, bool readOnly = false);

    QString getText() const;
    void setText(const QString & text);
    void setStatus(QString);
    QString getStatus();
    void closeEvent(QCloseEvent *event);
    inline QModelIndex getIndex(){ return currIndex;}
    inline void setValue(QSqlRecord newValue){

    }

    QSqlRecord getValue();

signals:
    void editingFinished(ImageWidgetDelegate*);

private:
    QSqlRecord currRecord;
    QTextEdit *txtLabelEdit;
    QLabel *lblImage;
    QString strStatus;
    QString description;
    QImage imgPic;
    QModelIndex currIndex;

public slots:
    void changeDescription();
};

//Q_DECLARE_METATYPE(QSqlRecord);
#endif // IMAGEWIDGETDELEGATE_H
