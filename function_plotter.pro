QT       += core gui \
        quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Main/main.cpp \
    Main/mainwindow.cpp \
    HelperClasses/functionobj.cpp \
    HelperClasses/plot.cpp \
    HelperClasses/qcustomplot.cpp \

HEADERS += \
    Main/mainwindow.h \
    HelperClasses\functionobj.h \
    HelperClasses\plot.h \
    HelperClasses\qcustomplot.h

FORMS += \
    UI/mainwindow.ui \
    UI/plot.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
