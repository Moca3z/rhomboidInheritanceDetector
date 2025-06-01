#ifndef TESTDETECTRHOMBOIDINHERITANCE_H
#define TESTDETECTRHOMBOIDINHERITANCE_H

#include <QObject>
#include <QTest>

class TestDetectRhomboid : public QObject
{
    Q_OBJECT
private slots:
    void testDetectRhomboidInheritance_data();
    void testDetectRhomboidInheritance();


};

#endif // TESTDETECTRHOMBOIDINHERITANCE_H
