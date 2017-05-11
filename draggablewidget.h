#ifndef DRAGGABLEWIDGET_H
#define DRAGGABLEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>

class DraggableWidget : public QFrame
{
    Q_OBJECT
public:
    explicit DraggableWidget(QWidget *parent = 0);

    bool eventFilter(QObject *obj, QEvent *event);


    inline void setMainLayout(QLayout *newLayout){
        disappearingWidget->setLayout(newLayout);
    }

protected:
    //col 25B row 6
    QChar rightTri = QChar(0xB6,0x25);
    //col 25B row C
    QChar downTri = QChar(0xBC,0x25);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QWidget *disappearingWidget = new QWidget();
    QLabel *lblArrow = new QLabel(downTri);
    QLabel *lblTitle = new QLabel();
    QLabel *lblClose = new QLabel("X");

    bool expanded=true;

    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
signals:
    void closeWidget();
public slots:

private:
    void buildWidget();
};

#endif // DRAGGABLEWIDGET_H
