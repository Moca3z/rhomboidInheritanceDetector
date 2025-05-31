#include <QCoreApplication>
#include "testFindIntermediates.h"
#include "testfindmergepoint.h"


int main(int argc, char *argv[])
{

    TestFindIntermediates testsFindIntermediates;
    TestFindMergePoint testsFindMergePoint;

    QTest::qExec(&testsFindIntermediates);
    QTest::qExec(&testsFindMergePoint);
    //test comment
}
