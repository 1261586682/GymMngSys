QT       += core gui network

INCLUDEPATH += C:\Users\12615\Desktop\tools\protobuf\protobuf-3.20.1\src
LIBS += -LC:\Users\12615\Desktop\tools\protobuf\build\Release -llibprotobuf


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    EditMsgBox.cpp \
    EditRCBox.cpp \
    EditRegBox.cpp \
    EditRprBox.cpp \
    EditUprice.cpp \
    Editconn.cpp \
    LoginBox.cpp \
    MainWidget.cpp \
    Personnel.cpp \
    Repairlog.cpp \
    enum.cpp \
    exsi.cpp \
    main.cpp \
    proto.pb.cc \

HEADERS += \
    EditMsgBox.h \
    EditRCBox.h \
    EditRegBox.h \
    EditRprBox.h \
    EditUprice.h \
    Editconn.h \
    LoginBox.h \
    MainWidget.h \
    Personnel.h \
    Repairlog.h \
    enum.h \
    exsi.h \
    proto.pb.h

FORMS += \
    MainWidget.ui

TRANSLATIONS += \
    GymMngSys_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    proto.proto

DEFINES +=PROTOBUF_USE_DLLS
