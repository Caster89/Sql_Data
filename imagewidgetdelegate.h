#ifndef IMAGEWIDGETDELEGATE_H
#define IMAGEWIDGETDELEGATE_H
#include <QWidget>
#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QModelIndex>
#include <QStyleOptionViewItem>

class ImageWidgetDelegate: public QWidget
{
    Q_OBJECT
public:
    ImageWidgetDelegate(QWidget *parent, const QModelIndex & index, const QStyleOptionViewItem &option);

    QString getText() const;
    void setText(const QString & text);
    void setStatus(QString);
    QString getStatus();

private:
    QTextEdit *txtLabelEdit;
    QLabel *lblImage;
    QString strStatus;
};

#endif // IMAGEWIDGETDELEGATE_H
