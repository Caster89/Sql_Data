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
#include <QMenu>
#include <QAction>
#include <QToolBar>
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
    void recordDoubleClicked(const QModelIndex index);
    QVector<QVector<QString> > getPrimary();
    //QVector<QVector<QString> > getFields();
    QVector<MyField> getFields();
    void createNewRecord();
    bool deleteRecord();
    bool updateRecord();
    void printRecord();
    bool newDB();
    bool openDB();
    bool editDB();
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

    inline void nextRecord(){
        int pos =dbTableView->currentIndex().row();
        if (pos==0 && dbmodel->rowCount()>1)
            previousAct->setEnabled(true);
        pos++;
        if (pos>=dbmodel->rowCount()){
            nextAct->setEnabled(false);
            return;
        }
        dbTableView->selectRow(pos);
        if ((pos+1) >= dbmodel->rowCount()){
            nextAct->setEnabled(false);
            return;
        }
        return;
    }

    inline void previousRecord(){
        int pos =dbTableView->currentIndex().row();
        if (pos==(dbmodel->rowCount()-1) && dbmodel->rowCount()>1)
            nextAct->setEnabled(true);
        pos--;
        qDebug()<<"movign to "<<pos;
        if (pos<0){
            previousAct->setEnabled(false);
            return;
        }
        dbTableView->selectRow(pos);
        if ((pos-1) < 0){
            previousAct->setEnabled(false);
            return;
        }
        return;
    }


private:
    Ui::MainWindow *ui;
    void createMenu();
    void createActions();


    bool CreateConnection(QString dbDir);

    QString dbName;
    QSqlDatabase db;
    //QSqlTableModel *dbmodel;
    MySqlTableModel *dbmodel = new MySqlTableModel();
    QTableView *dbTableView;
    QVector<QString> Tables_List;
    QVector<MyField> Fields;
    QVector<QVector<QString> > Primary_keys;

    QVector<DisplayWidget*> prwItems;
    //QVector<DisplayWidgetBase*> prwItems;
    QDataWidgetMapper *mapper= new QDataWidgetMapper;
    QVBoxLayout *prwLayout = new QVBoxLayout();
    QFrame *frmPreview = new QFrame();
    QScrollArea *scrPreview = new QScrollArea();

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *toolBar = new QToolBar();
    QAction *newAct;
    QAction *openAct;
    QAction *exportAct;
    QAction *editDBAct;
    QAction *addRowAct;
    QAction *removeRowAct;
    QAction *previousAct;
    QAction *nextAct;

    QPushButton *btnAddRecord = new QPushButton("AddRecord");

    QPushButton *btnRemoveRecord = new QPushButton("Remove Record");

    QPushButton *btnModifyRecord = new QPushButton("Modify Record");

    QPushButton *btnPrint = new QPushButton("Export");


protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

};

#endif // MAINWINDOW_H
