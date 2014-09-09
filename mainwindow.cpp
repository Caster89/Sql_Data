#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modifydialog.h"
#include "querygenerator.h"
#include "labeledtext.h"
#include "previewpopup.h"
#include "mysqltablemodel.h"
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
#include <QDataWidgetMapper>
#include <displaywidget.h>
#include <mysqltablemodel.h>
#include "myfield.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    qDebug()<<"Begin Program\n";

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

    QPushButton *btnAddRecord = new QPushButton("Add Record",frmButtons);
    QPushButton *btnRemoveRecord = new QPushButton("Remove Record",frmButtons);
    QPushButton *btnModifyRecord = new QPushButton("Modify Record",frmButtons);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnAddRecord);
    btnLayout->addWidget(btnRemoveRecord);
    btnLayout->addWidget(btnModifyRecord);
    frmButtons->setLayout(btnLayout);

    //The frame for the preview is created
    QFrame *frmPreview = new QFrame();
    QVBoxLayout *prwLayout = new QVBoxLayout;
    QScrollArea *scrPreview = new QScrollArea;
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
    dbName="movies";
    CreateConnection(dbName); //Go to method, return bool
    //QSqlQuery querytest (q.createUpdate("Main_table",Update,Where),db);




    //Ther preview frame is populated with the labeled text
    //preview field is enabeled in order to navigate throught he preview fields array

    for (int i=0;i<prwItems.size();i++){
        //If the field has to be previewed in the preview frame then the field name label is added
        prwLayout->addWidget(prwItems[i]);
    }
    //The scroll area is set to a fixed width wich allowes to view the whole contents without scrolling to the side
    frmPreview->setMinimumWidth(prwLayout->sizeHint().width());
    scrPreview->setWidget(frmPreview);
    scrPreview->setFixedWidth(frmPreview->width()+20);

    wdgMain->setLayout(mainLayout);
    qDebug()<<"Main Window Created";
    wdgMain->show();
    qDebug()<<"Main Window Showed";

    //The table view signal "current changed" is connected to the slot which changes the selected record
    connect(dbTableView->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(currentSelectionChanged(const QModelIndex &)));

    connect(dbTableView,SIGNAL(doubleClicked(QModelIndex)),SLOT(recordDoubleClicked(const QModelIndex)));

    connect(btnAddRecord,SIGNAL(clicked()),SLOT(createNewRecord()));






}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::CreateConnection(QString dbDir){
    //The sql database is created, and defined as SQLite
    qDebug()<<"Connection initiated";
    db=QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(QString("%1/%1").arg(dbDir));


    if (!db.open()){
        qDebug()<<"No Connection made to file";
        return false;
    }
    qDebug()<<"DB Created";
    dbmodel= new MySqlTableModel(this,db);
    dbmodel->select();
    //The main table is loaded. It contains all the main data.



    //The table view is prepared by setting the model and other options
    dbTableView->setModel(dbmodel);
    dbTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    dbTableView->resizeColumnsToContents();
    //The fields array is iterated and each data column in the model is named, while the ones which should not
    //be previewd in the table are hidden.
    mapper->setModel(dbmodel);
    for(int n=0;n<dbmodel->getFields().size();n++){
        //dbmodel->setHeaderData(n,Qt::Horizontal,Fields[n][0]);
        if(!dbmodel->getField(n).getVisTable()){
            dbTableView->hideColumn(n);
        }
        //Each field is checked to see whether it should appear in the preview frame.
        //If so a label is created and added to the prwFields, ad a blank one is added to prwValues.
        if (dbmodel->getField(n).getVisPreview()){

            prwItems.push_back(new DisplayWidget(dbmodel->getField(n).getName(),dbmodel->getField(n).getType(),false));
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

    return true;


}

void MainWindow::currentSelectionChanged(const QModelIndex &index){
}

void MainWindow::recordDoubleClicked(const QModelIndex index){
//    QRect deskRect = QApplication::desktop()->rect();
//    int wid = QApplication::desktop()->width();
//    int height = QApplication::desktop()->height();
//    QPoint curPosition = QCursor::pos();
//    //curPosition=QWidget::mapFromGlobal(curPosition);
//    QSqlRecord sel_record = dbmodel->record(index.row());
//    PreviewPopup *prev = new PreviewPopup(this,sel_record,prwFields, Fields);
//    prev->setWindowFlags(Qt::Popup);
//    qDebug()<<"Mouse x:"<<curPosition.x();
//    qDebug()<<"Mouse y:"<<curPosition.y();
//    qDebug()<<deskRect.width()<<"x"<<deskRect.height();
//    //prev->setStyleSheet("background-color:red;");
//    prev->show();
}

QVector<QVector<QString> > MainWindow::getPrimary(){
    return Primary_keys;
}

QVector<QVector<QString> > MainWindow::getFields(){
    //return Fields;
}

void MainWindow::createNewRecord(){
    //This slot is linked to the AddRecord QPushButton signal
    //The ModfyDialog is created, passing in the parent and the name of the fields
    ModifyDialog querygen(this,dbmodel,-1);
    //If the query is accepted
    querygen.exec();
//    if(querygen.exec()==QDialog::Accepted){
//        //The inserted values are retreived
//        QVector<DisplayWidget*>vecValues=querygen.getValues();
//        //Four vectors are created
//        //1. hold the values of the primary field for the current record
//        //2. hold the values for all the fields in the current record
//        //3. holds the information of the images
//        //4. used as vector to create the previos two
//        QVector<QVector<QString> > temp_Primary;
//        QVector<QVector<QString> > new_values;
//        QVector<QVector<QVector<QString> > > temp_img;
//        QVector<QString> temp_value(2);
//        QVector<QString> genQuery;



//        //The directory for the new record is created.
//        //The directory will be used to store all the necessary files
//        //The name of the record directory is given by it's primary values
//        QString recordDir = dbName + QDir::separator();
//        //All the fields are scanned to get the primary ones
//        for(int n=0;n<Fields.size();n++){
//            //if(vecValues[n]->getPrimary()==1){
//                //temp_value[0]=vecValues[n]->getField();
//                //temp_value[1]=vecValues[n]->getValue();
//                temp_Primary.append(temp_value);
//                recordDir += temp_value[1] + "_";
//            }
//        }
//        //Chop removes the last "n" caracters off the end of a String
//        //in this case the last _
//        //recordDir.chop(1);
//        //qDebug()<<recordDir;


//        QSqlQuery queryAlreadyExists;

//        queryAlreadyExists.exec(QueryGenerator::createSelect("Main_table",temp_Primary));

//        if(queryAlreadyExists.first()){
//            //A record already exists with those primary fields
//            qDebug()<<"\n Name Already Exists "<<queryAlreadyExists.first();
//        }else{

//        qDebug()<<"Doesn't Exist Yet";
//        //The current directory of the program is used and the new directory is created
//        QDir currentDir(QDir::currentPath());
//        currentDir.mkdir(recordDir);
//        //The various record values are read and separated on the basis of where they should go
//        for(int n=0;n<Fields.size();n++){

//            //If the values has to go in the Main table it is added to the record
//            //if (vecValues[n]->getTable()=="Main_table"){
//                //temp_value[0]=vecValues[n]->getField();
//                //If the field is of type image then the file is copied in the main directory
//                //of the record, and being in the main table it is renamed to the name of the field
//                //if(vecValues[n]->getType()=="IMAGE"){
//                    QFileInfo orgImage(vecValues[n]->getValue().toString());

//                    //The file is copied using QFile::copy
//                    //The main directory  is used, and the name of the field is used, followed by the
//                    //original suffix
//                    //QFile::copy(vecValues[n]->getValue().toString(),
//                      //          QString(recordDir + QDir::separator() + vecValues[n]->getField() +
//                                        //"." + orgImage.suffix()));
//                    //temp_value[1]=recordDir + QDir::separator() + vecValues[n]->getField() + "." + orgImage.suffix();

//                }else{
//                temp_value[1]=vecValues[n]->getValue().toString();
//                }
//                new_values.push_back(temp_value);
//            }
//         }
//         //The SQL string is generated
//         genQuery.append(QueryGenerator::createInsert("Main_table",new_values));
//         //The values are re-read and all values not to be inserted in the main table
//         //are added to their appropriate table
//         //for(int n=0;n<Fields.size();n++){
//           //  if(vecValues[n]->getTable()!="Main_table"){
//                 currentDir.mkdir(recordDir+QDir::separator()+vecValues[n]->getField());
//                 temp_img.clear();
//             //    temp_img=vecValues[n]->getImage();


//                 for(int i=0;i<temp_img.size();i++){
//                     new_values.clear();
//                     new_values=temp_img[i];

//                     QFileInfo orgImage(new_values[0][1]);

//                     //new_values[0][1]=QString(recordDir + QDir::separator()
//                       //                       + vecValues[n]->getField() +
//                         //                     QDir::separator() + orgImage.fileName());
//                     new_values.pop_back();
//                     qDebug()<<new_values.size();

//                     new_values+=temp_Primary;
//                     //genQuery.append(QueryGenerator::createInsert(vecValues[n]->getTable(),new_values));

//                     qDebug()<<new_values[0][0]<<":"<<new_values[0][1]<<"\n"<<new_values[1][0]<<
//                                                                                                 ":"<<new_values[1][1]<<"\n"
//                                                                                                                        <<new_values[2][0]<<":"<<new_values[2][1]<<"\n"                                                                                                                                         <<new_values[3][0]<<":"<<new_values[3][1]<<"\n";
//                     QFile::copy(orgImage.absoluteFilePath(), new_values[0][1]);
//                 }
//              }
//          }
//         qDebug()<<"Generated Queries: \n";
//         for(int i=0;i<genQuery.size();i++){
//             qDebug()<<genQuery[i]<<"\n";
//         }

//         QSqlQuery Create_Record;
//         for(int i=0;i<genQuery.size();i++){
//             Create_Record.exec(genQuery[i]);
//         }
//    }
//    }

}
