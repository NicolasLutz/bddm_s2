#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T11:14:02
#
#-------------------------------------------------

QT       += core gui
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interfaceBDDM
TEMPLATE = app

CONFIG += c++11
DESTDIR = $$PWD/bin

app_data.path    = $${DESTDIR}
app_data.files   = $$PWD/data

INSTALLS += app_data

win32:DEPLOY_COMMAND    = windeployqt
macx:DEPLOY_COMMAND     = macdeployqt


Debug:BUILDDIR      = debug
Release:BUILDDIR    = release
#OBJECTS_DIR         = $${BUILDDIR}/.obj
#MOC_DIR             = $${BUILDDIR}/.moc
#RCC_DIR             = $${BUILDDIR}/.rcc
#UI_DIR              = $${BUILDDIR}/.ui


INCLUDEPATH +=  $$PWD/src
INCLUDEPATH += include/

VPATH   =   $$PWD/src

SOURCES += main.cpp\
        mainwindow.cpp \
    src/Analyser.cpp \
    src/FrameValue.cpp \
    src/analyser_hud_detection.cpp \
    src/abstractsubanalyser.cpp \
    src/analyser_color_detection.cpp \
    src/database.cpp \
    src/game.cpp \
    src/game.inl \
    src/database.inl

HEADERS  += mainwindow.h \
    src/Analyser.h \
    src/Common.h \
    src/FrameValue.h \
    src/analyser_hud_detection.h \
    src/abstractsubanalyser.h \
    src/analyser_color_detection.h \
    src/database.h \
    src/game.h

FORMS    += mainwindow.ui

message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

unix|win32: LIBS += -L"lib/" -lavformat -lavutil -lavcodec -lswscale -lsqlite3
