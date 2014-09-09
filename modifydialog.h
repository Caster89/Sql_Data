#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H
#include <displaywidget.h>
#include "querygenerator.h"
#include <QDialog>
#include <QVector>
#include <QString>
#include <QSqlTableModel>
#include "mysqltablemodel.h"
#include <QModelIndex>
#include <QDataWidgetMapper>
#include "myfield.h"

class ModifyDialog : public QDialog
{
    Q_OBJECT
public:

    ModifyDialog(QWidget *parent, MySqlTableModel * DBModel, int index);
    ModifyDialog(QWidget *parent,QSqlDatabase *db, QVector<QVector<QString> > Fields, QVector<QVector<QString> > Primary);
    QString modifyRecord(QWidget *parent, QVector<QString> Primary_values);
    QVector<MyField> Fields;
    QString Table_name;
    QString genQuery;
    QVector<DisplayWidget*> getValues();




private:
    QVector<DisplayWidget*> DispFields;
    void construct(QWidget *parent, MySqlTableModel *DBModel);
    QDataWidgetMapper *mapper=new QDataWidgetMapper;
    MySqlTableModel *BaseModel;

private slots:
    void AcceptDialog();
    void RejectDialog();



};

#endif // MODIFYDIALOG_H
