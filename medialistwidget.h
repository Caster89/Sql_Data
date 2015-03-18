#ifndef MEDIALISTWIDGET_H
#define MEDIALISTWIDGET_H
#include <QListWidget>
#include <QWidget>
#include <QListWidgetItem>
#include <QSqlRecord>
#include <QVector>




class MediaListWidget : public QListWidget
{
    Q_OBJECT
public:
    MediaListWidget(QWidget *parent);
    void addItem(QListWidgetItem* newitem);
    void insertItem(int row, QListWidgetItem*);
    void clear();
    QVector<QSqlRecord> getRecords();
    void commitData(QWidget *e);

    //void setItemDelegate(QAbstractItemDelegate *delegate);

    //bool event(QEvent *e);

    //~MediaListWidget();



private:
    QListWidgetItem *addBtnItem;

private slots:
    void addNewItem();

signals:
    void updatedImages();

};

#endif // MEDIALISTWIDGET_H
