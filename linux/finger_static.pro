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
    ../src/FingerMgr.cpp \
    ../src/FingerDb.cpp \
    ../src/Finger.cpp \
    ../src/CppSQLite3.cpp \
    ../src/src/serial.cpp \
    ../src/src/impl/unix.cpp \
    ../src/Rs2323.cpp

HEADERS +=\
    ../src/sqlite3.h \
    ../src/FingerMgr.h \
    ../src/FingerDb.h \
    ../src/Finger.h \
    ../src/CppSQLite3.h

LIBS+= -ldl -lpthread -lrt
INCLUDEPATH+=../extlib/include
linux-g++ {
    message(g++ = linux-g++)
}
linux-arm-g++ {
    message(g++ = linux-g++)
}
