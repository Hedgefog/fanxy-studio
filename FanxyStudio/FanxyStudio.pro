#-------------------------------------------------
#
# Project created by QtCreator 2015-08-29T22:28:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FanxyStudio
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    fileedit.cpp \
    finddialog.cpp \
    gotodialog.cpp \
    projectmanager.cpp \
    inputdialog.cpp \
    linenumberwidget.cpp \
    syntaxhighlighter.cpp \
    skinloader.cpp \
    completeritemmodel.cpp \
    codeedit.cpp \
    createprojectdialog.cpp \
    settingloader.cpp \
    settingwindow.cpp \
    projectpluginloader.cpp \
    logger.cpp \
    syntaxtemplateloader.cpp

HEADERS  += mainwindow.h \
    fileedit.h \
    finddialog.h \
    gotodialog.h \
    projectmanager.h \
    inputdialog.h \
    linenumberwidget.h \
    syntaxhighlighter.h \
    skinloader.h \
    completeritemmodel.h \
    codeedit.h \
    projectinterface.h \
    createprojectdialog.h \
    settingloader.h \
    settingwindow.h \
    projectpluginloader.h \
    logger.h \
    syntaxtemplateloader.h

FORMS    += mainwindow.ui \
    finddialog.ui \
    gotodialog.ui \
    inputdialog.ui \
    createprojectdialog.ui \
    settingwindow.ui

RESOURCES += \
    resources.qrc

#DEST
CONFIG(release, debug|release): DESTDIR = release
else:CONFIG(debug, debug|release): DESTDIR = debug
OBJECTS_DIR = $$DESTDIR
MOC_DIR = $$DESTDIR
RCC_DIR = $$DESTDIR
UI_DIR = $$DESTDIR
