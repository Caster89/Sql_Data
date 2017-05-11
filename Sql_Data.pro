#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T09:09:34
#
#-------------------------------------------------

QT       += core gui
QT       += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = Sql_Data
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modifydialog.cpp \
    imageitemdelegate.cpp \
    imagewidgetdelegate.cpp \
    labeledtext.cpp \
    myfield.cpp \
    mysqltablemodel.cpp \
    previewpopup.cpp \
    querygenerator.cpp \
    medialistwidget.cpp \
    printitembase.cpp \
    printitemtext.cpp \
    printitemlongtext.cpp \
    printitemimage.cpp \
    printitem.cpp \
    printsetup.cpp \
    mystackedwidget.cpp \
    printitemimages.cpp \
    displaywidgetbase.cpp \
    displaywidgettext.cpp \
    displaywidgetlongtext.cpp \
    displaywidgetimage.cpp \
    displaywidgetimages.cpp \
    displaywidget_old.cpp \
    displaywidget.cpp \
    myprintlist.cpp \
    dragscrollarea.cpp\
    printitemstatic.cpp \
    printitemcombined.cpp \
    draggablewidget.cpp \
    displaywidgetmultiplechoice.cpp \
    editdatabasedialog.cpp \
    fielditem.cpp

HEADERS  += mainwindow.h \
    modifydialog.h \
    imageitemdelegate.h \
    imagewidgetdelegate.h \
    labeledtext.h \
    myfield.h \
    mysqltablemodel.h \
    previewpopup.h \
    querygenerator.h \
    datatype.h \
    medialistwidget.h \
    printitembase.h \
    printitemtext.h \
    printitemlongtext.h \
    printitemimage.h \
    printitem.h \
    printsetup.h \
    mystackedwidget.h \
    printitemimages.h \
    displaywidgetbase.h \
    displaywidgettext.h \
    displaywidgetlongtext.h \
    displaywidgetimage.h \
    displaywidgetimages.h \
    metatypedeclaration.h \
    displaywidget_old.h \
    displaywidget.h \
    myprintlist.h \
    dragscrollarea.h\
    printitemstatic.h \
    printitemcombined.h \
    draggablewidget.h \
    displaywidgetmultiplechoice.h \
    editdatabasedialog.h \
    fielditem.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    Modify.txt \
    Structure.txt
