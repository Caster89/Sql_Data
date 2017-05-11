#pragma once
#ifndef MEDIALISTWIDGET_H
#define MEDIALISTWIDGET_H
#include <QListWidget>
#include <QWidget>
#include <QListWidgetItem>
#include <QSqlRecord>
#include <QVector>
#include <QDebug>
#include "metatypedeclaration.h"



class MediaListWidget : public QListWidget
{
    Q_OBJECT
public:
    MediaListWidget(QWidget *parent, bool setEditable = false);
    void addItem(QListWidgetItem* newitem);
    void insertItem(int row, QListWidgetItem*);
    void clear();
    QVector<QSqlRecord> getRecords();
    void commitData(QWidget *e);

    inline bool isEditable(){
        return editable;
    }


    inline QSizePolicy sizePolicy(){
        return QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    }

    inline int getItemSize(){
        return itemSize;
    }

    inline float getItemScale(){
        return itemScale;
    }



    inline QSize sizeHint() const{
        //QSize preferredSize = QSize(3*(itemSize+2*itemMargin), 2*itemSize+5);
        qDebug()<<"Medial List widget is returning a size hint of: "<<3*(itemScale*itemSize+2*itemMargin)<<"x"<<itemScale*itemSize*3/4;
        return QSize(3*(itemScale*itemSize+2*itemMargin),itemScale*itemSize*3/4);
                //QSize(330, 515);
    }

    //void setItemDelegate(QAbstractItemDelegate *delegate);

    //bool event(QEvent *e);

    //~MediaListWidget();


public slots:
    inline void increaseItemScale(){
        itemScale += 0.1;
        this->setMinimumHeight(itemScale*itemSize*1.5+5);
        emit imageScaleChanged();
        doItemsLayout();
        repaint();
        updateGeometry();
        parentWidget()->updateGeometry();
        qDebug()<<'MediaList size is now: '<<this->size();
    }

    inline void decreaseItemScale(){
        itemScale -= 0.1;
        this->setMinimumHeight(itemScale*itemSize*1.5+5);
        emit imageScaleChanged();
        doItemsLayout();
        repaint();
        updateGeometry();
        parentWidget()->updateGeometry();

    }

    void removeSelectedItem();
private:
    QListWidgetItem *addBtnItem;
    int itemSize = 100;
    float itemScale = 1.0;
    int itemMargin = 5;
    bool editable;

private slots:
    void addNewItem();

signals:
    void updatedImages();
    void imageScaleChanged();
};
//Q_DECLARE_METATYPE(QSqlRecord);
#endif // MEDIALISTWIDGET_H
