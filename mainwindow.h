#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QLabel>
#include <QTextEdit>
#include <QVariant>
#include <labeledtext.h>
#include <displaywidget.h>
#include <QDataWidgetMapper>
#include "myfield.h"
#include "mysqltablemodel.h"
//#include "metatypedeclaration.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void currentSelectionChanged(const QModelIndex &);
    void recordDoubleClicked(const QModelIndex index);
    QVector<QVector<QString> > getPrimary();
    //QVector<QVector<QString> > getFields();
    QVector<MyField> getFields();
    void createNewRecord();
    bool deleteRecord();
    bool updateRecord();
    void printRecord();
    inline void modelHasReset(){
        qDebug()<<"ModelHasReset";
    }

    inline void dataChangeEmitted(){
        qDebug()<<"Model emitted data change";
    }

    inline void rowsInsertedEmitted(QModelIndex parent,int first,int last){
        qDebug()<<"Model emitted rowsInserted";
        qDebug()<<"With parent: "<<parent<<"\n first row: "<<first<<"\n last row"<<last;
    }


private:
    Ui::MainWindow *ui;
    bool CreateConnection(QString dbDir);
    QString dbName;
    QSqlDatabase db;
    //QSqlTableModel *dbmodel;
    MySqlTableModel *dbmodel;
    QTableView *dbTableView;
    QVector<QString> Tables_List;
    QVector<MyField> Fields;
    QVector<QVector<QString> > Primary_keys;
    QVector<QLabel*> prwFields;
    QVector<QLabel*> prwValues;
    QVector<DisplayWidget*> prwItems;
    //QVector<DisplayWidgetBase*> prwItems;
    QDataWidgetMapper *mapper= new QDataWidgetMapper;



};

#endif // MAINWINDOW_H
