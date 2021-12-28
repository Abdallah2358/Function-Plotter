QT += testlib widgets


CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_funcobj.cpp \
    ../../HelperClasses/functionobj.cpp

HEADERS += \
    ../../HelperClasses/functionobj.h
