#ifndef EDITDATABASEDIALOG_H
#define EDITDATABASEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QVector>
#include "dragscrollarea.h"
#include "myfield.h"

class EditDatabaseDialog : public QDialog
{
    Q_OBJECT
public:
    EditDatabaseDialog( QWidget *parent = 0);
    EditDatabaseDialog(QVector<MyField> fields, QWidget *parent = 0);

    QList<FieldEdit*> getChanges();
    static QList<FieldEdit*> editDatabase(QVector<MyField> fieldsList, QWidget *parent = 0);

private:
    QGridLayout *mainLayout = new QGridLayout();
    QVBoxLayout *scrollLayout = new QVBoxLayout();
    DragScrollArea *dragWidget = new DragScrollArea();
    QWidget *mainWidget = new QWidget();
    bool valid();


    void buildWidget();


private slots:
    void addNewField();
    void removeField();
    void resetPositions();
    void acceptChanges();

};

#endif // EDITDATABASEDIALOG_H
