#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T20:14:31
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = l2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    listener.cpp \
    serveruser.cpp \
    crypto.cpp \
    protocol.cpp \
    doc.cpp \
    client.cpp \
    clientdialog.cpp \
    serverdialog.cpp \
    userconnection.cpp \
    tcpmessager.cpp \
    newuserdialog.cpp \
    usercontroldialog.cpp \
    doccontroldialog.cpp \
    newdocdialog.cpp \
    srddialog.cpp \
    downloaddialog.cpp

HEADERS  += mainwindow.h \
    server.h \
    listener.h \
    serveruser.h \
    crypto.h \
    protocol.h \
    doc.h \
    client.h \
    clientdialog.h \
    serverdialog.h \
    userconnection.h \
    tcpmessager.h \
    newuserdialog.h \
    commonheader.h \
    usercontroldialog.h \
    doccontroldialog.h \
    newdocdialog.h \
    srddialog.h \
    downloaddialog.h

FORMS    += mainwindow.ui \
    clientdialog.ui \
    serverdialog.ui \
    newuserdialog.ui \
    usercontroldialog.ui \
    doccontroldialog.ui \
    newdocdialog.ui \
    srddialog.ui \
    downloaddialog.ui

DISTFILES +=
