#-------------------------------------------------
#
# Project created by QtCreator 2012-11-24T17:24:13
#
#-------------------------------------------------

QT       -= core gui

TARGET = finger
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ../src/sqlite3.c \
    ../src/Rs232_Ctb.cpp \
    ../src/FingerMgr.cpp \
    ../src/FingerDb.cpp \
    ../src/Finger.cpp \
    ../src/CppSQLite3.cpp

HEADERS +=\
    ../src/sqlite3.h \
    ../src/Rs232_Ctb.h \
    ../src/FingerMgr.h \
    ../src/FingerDb.h \
    ../src/Finger.h \
    ../src/CppSQLite3.h

LIBS+= -ldl -lpthread
 INCLUDEPATH+=../extlib/include
linux-g++ {
    message(g++ = linux-g++)

    LIBS+=../extlib/x86/libwxctb-0.9.a
}
linux-arm-g++ {
    message(g++ = linux-g++)

    LIBS+=../extlib/arm/libwxctb-0.9.a
}
