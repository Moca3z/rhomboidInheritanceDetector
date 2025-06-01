QT = core
QT += testlib core
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        collectoverriddenmethods.cpp \
        detectrhomboidinheritance.cpp \
        findIntermediates.cpp \
        findmergepoint.cpp \
        main.cpp \
        testFindIntermediates.cpp \
        testcollectoverriddenmethods.cpp \
        testdetectrhomboidinheritance.cpp \
        testfindmergepoint.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    class.h \
    collectoverriddenmethods.h \
    detectrhomboidinheritance.h \
    findIntermediates.h \
    findmergepoint.h \
    rhombus.h \
    testFindIntermediates.h \
    testcollectoverriddenmethods.h \
    testdetectrhomboidinheritance.h \
    testfindmergepoint.h
