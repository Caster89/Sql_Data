#ifndef PREVIEWPOPUP_H
#define PREVIEWPOPUP_H

#include <QScrollArea>
#include <QWidget>
#include <QVector>
#include <QModelIndex>
#include <QLabel>
#include <QSize>
#include <QSqlRecord>

class PreviewPopup  : public QWidget
{
    Q_OBJECT
public:
    PreviewPopup(QWidget *parent, QSqlRecord sel_record,QVector<QLabel*> prwFields, QVector<QVector<QString> > Fields);
    bool eventFilter(QObject * o, QEvent *e);

    virtual void closeEvent(QCloseEvent *closeEv);
    QSize sizeHint() const;

signals:
    void popupClosed();
};

#endif // PREVIEWPOPUP_H

