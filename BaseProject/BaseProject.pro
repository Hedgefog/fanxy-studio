#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T18:36:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BaseProject
TEMPLATE = lib
CONFIG += plugin #no_plugin_name_prefix

DEFINES += BASEPROJECT

SOURCES += baseproject.cpp

HEADERS += baseproject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    BaseProject.json

RESOURCES += \
    resources.qrc

#DEST
CONFIG(release, debug|release): DESTDIR = release
else:CONFIG(debug, debug|release): DESTDIR = debug
OBJECTS_DIR = $$DESTDIR
MOC_DIR = $$DESTDIR
RCC_DIR = $$DESTDIR
UI_DIR = $$DESTDIR
