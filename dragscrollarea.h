#ifndef DRAGSCROLLAREA_H
#define DRAGSCROLLAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QList>
#include <QVBoxLayout>

class DragScrollArea : public QScrollArea
{
 Q_OBJECT
public:
    DragScrollArea(QWidget *parent);
    virtual ~DragScrollArea() {};
    void addWidget(QWidget* newWidget);
    void removeWidget(QWidget* removeWidget);

signals:
    void itemAdded();
    void itemRemoved();
    void itemMoved();
    void itemModified();


protected:
    QList<QWidget *> widgetList;
private:
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();


    void clearArea();
    void repopulateArea();

private slots:

};

#endif // DRAGSCROLLAREA_H
