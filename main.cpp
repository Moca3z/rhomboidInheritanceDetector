#include <QCoreApplication>
#include "testFindIntermediates.h"
#include "testfindmergepoint.h"
#include "testcollectoverriddenmethods.h"
#include "testdetectrhomboidinheritance.h"

int main(int argc, char *argv[])
{

    TestFindIntermediates testsFindIntermediates;
    TestFindMergePoint testsFindMergePoint;
    TestCollectOverridden testsCollectOverriddenMethods;
    TestDetectRhomboid testsDetectRhomboidInheritance;

    QTest::qExec(&testsFindIntermediates);
    QTest::qExec(&testsFindMergePoint);
    QTest::qExec(&testsCollectOverriddenMethods);
    QTest::qExec(&testsDetectRhomboidInheritance);
    //test comment
}
