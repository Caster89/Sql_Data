#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T09:09:34
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sql_Data
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modifydialog.cpp \
    displaywidget.cpp \
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
    mystackedwidget.cpp

HEADERS  += mainwindow.h \
    modifydialog.h \
    displaywidget.h \
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
    mystackedwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
