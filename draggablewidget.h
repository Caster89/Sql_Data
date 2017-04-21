#ifndef DRAGGABLEWIDGET_H
#define DRAGGABLEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class DraggableWidget : public QFrame
{
    Q_OBJECT
public:
    explicit DraggableWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // DRAGGABLEWIDGET_H
