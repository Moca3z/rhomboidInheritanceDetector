QT = core
QT += testlib core
QT += core xml
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        classdiagram.cpp \
        collectoverriddenmethods.cpp \
        detectrhomboidinheritance.cpp \
        findIntermediates.cpp \
        findmergepoint.cpp \
        main.cpp \
        rhombus.cpp \
        testFindIntermediates.cpp \
        testcheckforoverriddenparameters.cpp \
        testcollectoverriddenmethods.cpp \
        testdetectrhomboidinheritance.cpp \
        testfindmergepoint.cpp \
        testparseparameters.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    class.h \
    classdiagram.h \
    collectoverriddenmethods.h \
    detectrhomboidinheritance.h \
    error.h \
    findIntermediates.h \
    findmergepoint.h \
    rhombus.h \
    testFindIntermediates.h \
    testcheckforoverriddenparameters.h \
    testcollectoverriddenmethods.h \
    testdetectrhomboidinheritance.h \
    testfindmergepoint.h \
    testparseparameters.h
