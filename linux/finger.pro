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
    ../src/Rs232_Ctb.cpp


HEADERS += \
    ../include/FingerMgr.h \
    ../include/Finger.h \
    ../src/codec.h


LIBS+= -ldl -lpthread
#LIBS+= -lprotobuf -lczmq
OTHER_FILES += \
    ../src/ReadMe.txt \
    ../src/finger.db \
    ../src/FGChar.dat \
    finger.proto

linux-arm-g++ {
    message(g++ = linux-arm-g++ compile)
    LIBS+=../extlib/arm/libwxctb-0.9.a

    INCLUDEPATH+=../extlib/include
}

linux-g++ {
    message(g++ = linux-g++)
    INCLUDEPATH+=../extlib/include
    LIBS+=../extlib/x86/libwxctb-0.9.a
}
