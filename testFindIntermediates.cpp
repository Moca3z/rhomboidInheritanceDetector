#include "testfindintermediates.h"
#include "findintermediates.h"

void TestFindIntermediates::testEmptyResult()
{
    QMap<QString, QSet<QString>> matrix;
    QMap<QString, void*> classes;
    auto result = findIntermediates("A", "B", matrix, classes);
    QVERIFY(result.isEmpty());
}

void TestFindIntermediates::testWithSampleData()
{
    QMap<QString, QSet<QString>> matrix = {
        {"Child", {"Parent1", "Parent2", "Grandparent"}},
        {"Parent1", {"Grandparent"}},
        {"Parent2", {"Grandparent"}}
    };

    QMap<QString, void*> classes;
    auto result = findIntermediates("Grandparent", "Child", matrix, classes);
    QCOMPARE(result.size(), 2);
    QVERIFY(result.contains("Parent1"));
    QVERIFY(result.contains("Parent2"));
}

