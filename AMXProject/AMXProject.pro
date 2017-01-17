#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T13:36:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AMXProject
TEMPLATE = lib
CONFIG += plugin #no_plugin_name_prefix


DEFINES   += AMXPROJECT

SOURCES += amxproject.cpp \
    codeanalyzer.cpp \
    setupwidget.cpp

HEADERS += amxproject.h \
    codeanalyzer.h \
    setupwidget.h

FORMS += \
    setupwidget.ui

RESOURCES += resources.qrc

DISTFILES += \
    AMXProject.json

#DEST
CONFIG(release, debug|release): DESTDIR = release
else:CONFIG(debug, debug|release): DESTDIR = debug
OBJECTS_DIR = $$DESTDIR
MOC_DIR = $$DESTDIR
RCC_DIR = $$DESTDIR
UI_DIR = $$DESTDIR

#LIB
CONFIG(release, debug|release): LIBS += -L$$PWD/../BaseProject/release/ -Wl,-rpath,. -lBaseProject
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BaseProject/debug/ -Wl,-rpath,. -lBaseProject
INCLUDEPATH += $$PWD/../BaseProject
DEPENDPATH += $$PWD/../BaseProject
