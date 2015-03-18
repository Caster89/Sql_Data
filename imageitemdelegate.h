
//Need to fix the out of focus problem
#ifndef IMAGEITEMDELEGATE_H
#define IMAGEITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <imagewidgetdelegate.h>
#include <QWidget>
#include <QObject>
#include <QSize>
#include <QString>
#include <QEvent>
#include <QPoint>

class ImageItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum EditMode {Editable, ReadOnly};
    enum Status{New,Old,Modified};
    ImageItemDelegate(QWidget *parent = 0);




    //QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize  sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QWidget *origParent;
    int margin=5;
    QPoint posDelete(const QStyleOptionViewItem & option) const;
private slots:
    void updateObject(ImageWidgetDelegate* editor);
    void buttonClicked();
    //bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);
};

#endif // IMAGEITEMDELEGATE_H
