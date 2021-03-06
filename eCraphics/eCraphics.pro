#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24T13:48:48
#
#-------------------------------------------------

QT       += core gui charts sql xml

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = eGraphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwnd.cpp \
    db.cpp \
    mychart.cpp \
    xmlparser.cpp \
    update.cpp \
    myconfig.cpp \
    mylogger.cpp \
    settings.cpp \
    changelist.cpp \
    mylistwidget.cpp \
    myuithreadblocker.cpp \
    authorization.cpp

HEADERS  += mainwnd.h \
    db.h \
    headers.h \
    mychart.h \
    xmlparser.h \
    update.h \
    myconfig.h \
    mylogger.h \
    settings.h \
    changelist.h \
    mylistwidget.h \
    myuithreadblocker.h \
    authorization.h

FORMS    += mainwnd.ui \
    settings.ui \
    changelist.ui \
    authorization.ui

RESOURCES += res.qrc

RC_FILE = myapp.rc

DISTFILES +=

DESTDIR += E:\eGraphics
