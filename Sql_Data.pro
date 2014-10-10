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
    querygenerator.cpp

HEADERS  += mainwindow.h \
    modifydialog.h \
    displaywidget.h \
    imageitemdelegate.h \
    imagewidgetdelegate.h \
    labeledtext.h \
    myfield.h \
    mysqltablemodel.h \
    previewpopup.h \
    querygenerator.h

FORMS    += mainwindow.ui
