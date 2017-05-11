#ifndef DRAGSCROLLAREA_H
#define DRAGSCROLLAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QList>
#include <QVBoxLayout>
#include <QPoint>

class DragScrollArea : public QScrollArea
{
 Q_OBJECT
public:
    DragScrollArea(QWidget *parent = 0);
    virtual ~DragScrollArea() {};
    void addWidget(QWidget* newWidget);
    void removeWidget(QWidget* removeWidget);
    void reorderWidget(QWidget *widget, int newPos);
    void reorderWidget(int orig, int dest);
    inline int count(){
        return widgetList.count();
    }

    inline QWidget* widgetAt(int pos){
        return widgetList.at(pos);
    }

    inline int widgetPosition(QWidget * widget){
        return widgetList.lastIndexOf(widget);
    }

    inline QWidget* lastWidget(){
        return widgetList.last();
    }

signals:
    void itemAdded();
    void itemRemoved();
    void itemMoved();
    void itemModified();


protected:
    QList<QWidget *> widgetList;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QPoint dragStartPosition;
    int moveToPosition = 0;
    int moveFromPosition = 0;
    int minDragMove = 4;

    void clearArea();
    void repopulateArea();
    int destinationPosition(QPoint pos);

private slots:

};

#endif // DRAGSCROLLAREA_H
