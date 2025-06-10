#ifndef TESTCHECKFOROVERRIDDENPARAMETERS_H
#define TESTCHECKFOROVERRIDDENPARAMETERS_H

#include <QObject>
#include <QTest>

class TestCheckForOverriddenParameters : public QObject
{
    Q_OBJECT
private slots:
    void testCheckForOverriddenParameters_data();
    void testCheckForOverriddenParameters();

};

#endif // TESTCHECKFOROVERRIDDENPARAMETERS_H
