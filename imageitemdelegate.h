
//Need to fix the out of focus problem
#ifndef IMAGEITEMDELEGATE_H
#define IMAGEITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QWidget>
#include <QObject>
#include <QSize>
#include <QString>

class ImageItemDelegate : public QStyledItemDelegate
{
public:
    ImageItemDelegate(QObject *parent = 0);



    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize  sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // IMAGEITEMDELEGATE_H
