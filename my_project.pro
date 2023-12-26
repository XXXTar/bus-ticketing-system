QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddata.cpp \
    adwidget.cpp \
    databases.cpp \
    hisorder.cpp \
    inlog.cpp \
    main.cpp \
    myorder.cpp \
    order.cpp \
    pay.cpp \
    preferential.cpp \
    regist.cpp \
    viewwindow.cpp \
    widget.cpp

HEADERS += \
    adddata.h \
    adwidget.h \
    databases.h \
    hisorder.h \
    inlog.h \
    myorder.h \
    order.h \
    pay.h \
    preferential.h \
    regist.h \
    viewwindow.h \
    widget.h

FORMS += \
    adddata.ui \
    adwidget.ui \
    hisorder.ui \
    inlog.ui \
    myorder.ui \
    order.ui \
    pay.ui \
    preferential.ui \
    regist.ui \
    viewwindow.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
