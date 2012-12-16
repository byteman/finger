TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    ../src/main.cpp


HEADERS += \
    ../include/FingerMgr.h \
    ../include/Finger.h


LIBS+= -ldl -lpthread
#LIBS+= -lprotobuf -lczmq
OTHER_FILES +=

LIBS+=-L. -lfinger -lrt
INCLUDEPATH+=../extlib/include
linux-arm-g++ {
    message(g++ = linux-arm-g++ compile)
}

linux-g++ {
    message(g++ = linux-g++)

}
