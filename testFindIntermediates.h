#ifndef TESTFINDINTERMEDIATES_H
#define TESTFINDINTERMEDIATES_H

#include <QObject>
#include <QtTest/QtTest>

class TestFindIntermediates : public QObject
{
    Q_OBJECT

private slots:
    void testEmptyResult();
    void testWithSampleData();
};

#endif // TESTFINDINTERMEDIATES_H
