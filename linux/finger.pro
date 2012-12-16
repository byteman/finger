TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    ../src/sqlite3.c \
    ../src/FingerMgr.cpp \
    ../src/FingerDb.cpp \
    ../src/CppSQLite3.cpp \
    ../src/Finger.cpp \
    ../src/main.cpp \
    ../src/src/serial.cpp \
    ../src/src/impl/unix.cpp \
    ../src/Rs2323.cpp


HEADERS += \
    ../include/FingerMgr.h \
    ../include/Finger.h \
    ../src/codec.h


LIBS+= -ldl -lpthread -lrt
#LIBS+= -lprotobuf -lczmq
OTHER_FILES += \
    ../src/ReadMe.txt \
    ../src/finger.db \
    finger.proto
INCLUDEPATH+=../extlib/include

linux-arm-g++ {
    message(g++ = linux-arm-g++ compile)
}

linux-g++ {
    message(g++ = linux-g++)
}

