#ifndef PRINTITEMSTATIC_H
#define PRINTITEMSTATIC_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QTextEdit>
#include <QLineEdit>
#include "printitembase.h"
#include "myfield.h"

class PrintItemStatic : public PrintItemBase
{
    Q_OBJECT
public:
    PrintItemStatic(MyField newField, QWidget *parent = 0);
    void paintItem(QTextDocument *doc, QTextCursor *cursor, QSqlRecord *record);

private:
    QStackedWidget *mainWidget = new QStackedWidget();
    QGridLayout *mainLayout = new QGridLayout();
    QComboBox *cmbStaticElmt = new QComboBox();
    QWidget *wdgNewPage = new QWidget();
    QGroupBox *wdgNewLine = new QGroupBox();
    QTextEdit *txtedtStaticText = new QTextEdit();
    //QWidget *wdgStaticText = new QWidget();
    QLineEdit *ledtNewLine = new QLineEdit();

    void buildWidget();

private slots:
    void staticElementChange(int currElmt);
};

#endif // PRINTITEMSTATIC_H
