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

LIBS+=./libfinger.a
linux-arm-g++ {
    message(g++ = linux-arm-g++ compile)
    INCLUDEPATH+=../extlib/include
    LIBS+=../extlib/arm/libwxctb-0.9.a
}

linux-g++ {
    message(g++ = linux-g++)
    INCLUDEPATH+=../extlib/include
    LIBS+=../extlib/x86/libwxctb-0.9.a
}
