#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modifydialog.h"
#include "querygenerator.h"
#include "labeledtext.h"
#include "previewpopup.h"
#include "mysqltablemodel.h"
#include "editdatabasedialog.h"
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QTableView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>
#include <QSqlRecord>
#include <QStringList>
#include <QMessageBox>
#include <QScrollArea>
#include <QRect>
#include <QDesktopWidget>
#include <QCursor>
#include <QPoint>
#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDataWidgetMapper>
#include <displaywidget.h>
#include <mysqltablemodel.h>
#include "myfield.h"
#include <QFontDatabase>
#include <QFont>
#include "printsetup.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    qDebug()<<"Begin Program\n";

    QFontDatabase fontDB;
    fontDB.addApplicationFont(":/Fonts/CMR_Regular.ttf");
    fontDB.addApplicationFont(":/Fonts/CMR_Bold.ttf");
    fontDB.addApplicationFont(":/Fonts/CMR_Italic.ttf");
    fontDB.addApplicationFont(":/Fonts/CMR_BoldItalic.ttf");
    QFont CMRFont = QFont("Computer Modern Roman");
    QApplication::setFont(CMRFont);




    //Create the central widget where the other frames will be inserted
    QWidget *wdgMain = new QWidget(this);
    qDebug()<<"Widget Initaited";
    setCentralWidget(wdgMain);

    //Create the outermost Layout to divide between the table, the preview and the buttons
    //QGridLayout *mainLayout = new QGridLayout;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15,5,15,10);


    //The frame for the buttons is created and the ADD Record REMOVE Record and MODIFY Button are added in a QHBOX
    QFrame *frmButtons = new QFrame();
    frmButtons->setFrameStyle(QFrame::Box | QFrame::Raised);
    frmButtons->setLineWidth(1);

    //QPushButton *btnAddRecord = new QPushButton("AddRecord",frmButtons);
    btnAddRecord->setEnabled(false);
    //QPushButton *btnRemoveRecord = new QPushButton("Remove Record",frmButtons);
    btnRemoveRecord->setEnabled(false);
    //QPushButton *btnModifyRecord = new QPushButton("Modify Record",frmButtons);
    btnModifyRecord->setEnabled(false);
    //QPushButton *btnPrint = new QPushButton("Export",frmButtons);
    btnPrint->setEnabled(false);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnAddRecord);
    btnLayout->addWidget(btnRemoveRecord);
    btnLayout->addWidget(btnModifyRecord);
    btnLayout->addWidget(btnPrint);
    frmButtons->setLayout(btnLayout);

    //The frame for the preview is created



    frmPreview->setLayout(prwLayout);


    //The Table viewer is created without grid and is added to the main layout
    dbTableView=new QTableView(this);
    dbTableView->setShowGrid(false);
    dbTableView->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    //mainLayout->addWidget(dbTableView,0,0,5,4);
    //The left frame is created and the table and button frames are added
    QVBoxLayout *lftLayout = new QVBoxLayout;
    lftLayout->addWidget(dbTableView);
    lftLayout->addWidget(frmButtons);
    //The main layout is created by adding the left layout and the preview widget
    mainLayout->addLayout(lftLayout);
    mainLayout->addWidget(scrPreview);

    //A value is given to the database and a connetion is made using the custom CreatConnection
    //dbName="/Users/Castro/Documents/Svago/Programmazione/Qt/database/movies.sqlite";
    //CreateConnection(dbName); //Go to method, return bool
    //QSqlQuery querytest (q.createUpdate("Main_table",Update,Where),db);




    //Ther preview frame is populated with the labeled text
    //preview field is enabeled in order to navigate throught he preview fields array

    //The scroll area is set to a fixed width wich allowes to view the whole contents without scrolling to the side
    frmPreview->setMinimumWidth(prwLayout->sizeHint().width());
    scrPreview->setWidget(frmPreview);
    scrPreview->setFixedWidth(frmPreview->width()+20);

    wdgMain->setLayout(mainLayout);
    wdgMain->show();
    createActions();
    createMenu();
    //The table view signal "current changed" is connected to the slot which changes the selected record
    //connect(dbTableView->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            //SLOT(currentSelectionChanged(const QModelIndex &)));
    connect(dbTableView,SIGNAL(doubleClicked(QModelIndex)),SLOT(recordDoubleClicked(const QModelIndex)));
    connect(dbmodel, SIGNAL(modelReset()), this, SLOT(modelHasReset()));
    connect(dbmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(dataChangeEmitted()));
    //connect(dbmodel,SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInsertedEmitted(QModelIndex,int,int)));
    connect(btnAddRecord,SIGNAL(clicked()),SLOT(createNewRecord()));
    connect(btnRemoveRecord,SIGNAL(clicked()),SLOT(deleteRecord()));
    connect(btnModifyRecord,SIGNAL(clicked()),SLOT(updateRecord()));
    connect(btnPrint,SIGNAL(clicked()),this,SLOT(printRecord()));

}

void MainWindow::createActions(){
    newAct = new QAction(tr("&New Database"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new database"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newDB);

    openAct = new QAction(tr("&Open Database"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an exising database"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openDB);

    exportAct = new QAction(tr("&Export Records"), this);
    exportAct->setShortcuts(QKeySequence::New);
    exportAct->setStatusTip(tr("Export records as PDF"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::printRecord);

    editDBAct = new QAction(tr("&Edit Database"), this);
    //editDBAct->setShortcuts(QKeySequence::New);
    editDBAct->setStatusTip(tr("Edit database field"));
    connect(editDBAct, &QAction::triggered, this, &MainWindow::editDB);

}

void MainWindow::createMenu(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(exportAct);
    exportAct->setEnabled(false);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editDBAct);
    editDBAct->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printRecord(){
    QVector<QSqlRecord> printRecords;
    QModelIndexList selRows = dbTableView->selectionModel()->selectedRows();
    for (int i =0; i<selRows.count();i++){
        printRecords.append(dbmodel->record(selRows[i].row()));
    }

    PrintSetup *printWidget=new PrintSetup(dbmodel->getFields(),printRecords);
    QWidget *compWidget= new QWidget();
    QHBoxLayout *test =new QHBoxLayout();
    compWidget->setLayout(test);
    test->addWidget(printWidget);
    compWidget->show();

}

bool MainWindow::CreateConnection(QString dbDir){
    //The sql database is created, and defined as SQLite
    qDebug()<<"Connection initiated";
    db=QSqlDatabase::addDatabase("QSQLITE");

    //db.setDatabaseName(QString("%1/%1").arg(dbDir));
    QFileInfo *dbFile = new QFileInfo(dbDir);
    QDir::setCurrent(dbFile->absolutePath());
    qDebug()<<"Set current directory to: "<<dbFile->absolutePath();
    db.setDatabaseName(dbFile->fileName());
    qDebug()<<db.databaseName();

    if (!db.open()){
        qDebug()<<"No Connection made to file";
        return false;
    }
    qDebug()<<"DB Created";
    dbmodel= new MySqlTableModel(this,db);
    //dbmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dbmodel->select();

    //The main table is loaded. It contains all the main data.

    //The table view is prepared by setting the model and other options

    dbTableView->setModel(dbmodel);

    dbTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //dbTableView->resizeColumnsToContents();

    //The fields array is iterated and each data column in the model is named, while the ones which should not
    //be previewd in the table are hidden.
    mapper->setModel(dbmodel);

    for(int n=0;n<dbmodel->getFields().size();n++){
        //dbmodel->setHeaderData(n,Qt::Horizontal,Fields[n][0]);
        if(!dbmodel->getField(n).getVisTable()){
            dbTableView->hideColumn(n);
        }
        //Each field is checked to see whether it should appear in the preview frame.
        if (dbmodel->getField(n).getVisPreview()){
            prwItems.push_back(new DisplayWidget(dbmodel->getField(n),false, frmPreview));
            mapper->addMapping(prwItems.last(),n,"Value");
        }
    }
    connect(dbTableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    dbTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Sets alternating Colors
    dbTableView->setAlternatingRowColors(true);
    dbTableView->setStyleSheet("alternate-background-color:#99DDFF;background-color:white;");
    //sets no triggers to edit the information
    dbTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //qDebug()<<"EDIT StRATEGY "<<dbmodel->editStrategy();
    return true;


}

//void MainWindow::currentSelectionChanged(const QModelIndex &index){
//}

void MainWindow::recordDoubleClicked(const QModelIndex index){
    updateRecord();
}

QVector<QVector<QString> > MainWindow::getPrimary(){
    return Primary_keys;
}

//QVector<QVector<QString> > MainWindow::getFields(){
QVector<MyField> MainWindow::getFields(){
    return Fields;
}

void MainWindow::createNewRecord(){
    //This slot is linked to the AddRecord QPushButton signal
    //The ModfyDialog is created, passing in the parent and the name of the fields
    qDebug()<<"Main Window called to create a new record";
    qDebug()<<"Currently there are "<<dbmodel->rowCount()<<" rows in the table";
    dbmodel->insertRow(dbmodel->rowCount());
    qDebug()<<"There are now "<<dbmodel->rowCount()<<" rows in the table";
    //dbmodel->insertRow(dbmodel->rowCount(),1,QModelIndex());
    qDebug()<<"Setting the mappers index to: "<<dbmodel->rowCount()-1;
    ModifyDialog modDialog(this,dbmodel,dbmodel->rowCount()-1);
    //If the query is accepted

    modDialog.exec();


}

bool MainWindow::deleteRecord(){
    qDebug()<<"deleting Record in position: "<<dbTableView->currentIndex().row();
    int row = dbTableView->currentIndex().row();
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete the record?");
    //msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec()==QMessageBox::Yes){
        qDebug()<<"Decided to remove record";
        dbmodel->removeRows(row,1);
        return dbmodel->submit();
    }
    return false;
}

bool MainWindow::updateRecord(){
    //This slot is linked to the AddRecord QPushButton signal
    ModifyDialog modDialog(this,dbmodel,dbTableView->currentIndex().row());
    modDialog.exec();
    return true;
}

bool MainWindow::newDB(){
    return true;
}

bool MainWindow::openDB(){
    QFileDialog fileDiag(this, "Open Database");
    fileDiag.setFileMode(QFileDialog::ExistingFile);
    fileDiag.setNameFilter("Database (*.db *.sql *.sqlite)");
    fileDiag.setDirectory(QStandardPaths::displayName(QStandardPaths::DocumentsLocation));

    if (!fileDiag.exec()){
        return false;
    }
    QString fileName = fileDiag.selectedFiles().first();
    foreach(QWidget* currItem, prwItems){
        prwLayout->removeWidget(currItem);
        currItem->setParent(NULL);
        delete currItem;
    }
    delete dbmodel;
    //dbTableView->setModel();
    prwItems.clear();
    dbName=fileName;
    CreateConnection(dbName); //Go to method, return bool
    //for (int i=0;i<prwItems.size();i++){
    foreach(QWidget* currItem, prwItems){
        //If the field has to be previewed in the preview frame then the field name label is added
        //prwLayout->addWidget(prwItems[i]);
        //prwItems[i]->show();
        prwLayout->addWidget(currItem);
        currItem->show();
    }
    prwLayout->update();
    frmPreview->update();
    frmPreview->adjustSize();
    scrPreview->setFixedWidth(frmPreview->width()+20);
    btnAddRecord->setEnabled(true);
    btnRemoveRecord->setEnabled(true);
    btnModifyRecord->setEnabled(true);
    btnPrint->setEnabled(true);
    editDBAct->setEnabled(true);
    qDebug()<<"The layout has: "<<prwLayout->count()<<" Widgets";

}

bool MainWindow::editDB(){
    //EditDatabaseDialog *editDBDiag = new EditDatabaseDialog(dbmodel->getFields());
    QVector<MyField> avlbFields = dbmodel->getFields();
    QList<FieldEdit*> changes = EditDatabaseDialog::editDatabase(avlbFields, this);


    qDebug()<<"Got to here with "<<changes.count()<<" changes";
    dbmodel->editTableStructure(changes);
    //editDBDiag->show();
    bool rewrite = false;
    foreach(FieldEdit *change, changes){
        qDebug()<<change->getField().getName()<<" majorChange: "<<change->mainEdit();
        if (change->mainEdit()){
            rewrite = true;
            break;
        }
    }
    if (rewrite){
        QStringList changeCommands = QueryGenerator::multipleChanges("Main_table",changes);
    }

    return true;
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU
