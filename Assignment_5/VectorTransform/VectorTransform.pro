QT       += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    canvas.cpp \
    algorithms.cpp \
    matrix3x3.cpp

HEADERS += \
    mainwindow.h \
    canvas.h \
    algorithms.h \
    matrix3x3.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
