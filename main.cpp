#include <QCoreApplication>
#include "testFindIntermediates.h"
#include "testfindmergepoint.h"
#include "testcollectoverriddenmethods.h"
#include "testdetectrhomboidinheritance.h"
#include "testparseparameters.h"

int main(int argc, char *argv[])
{

    TestFindIntermediates testsFindIntermediates;
    TestFindMergePoint testsFindMergePoint;
    TestCollectOverridden testsCollectOverriddenMethods;
    TestDetectRhomboid testsDetectRhomboidInheritance;
    TestParseParameters testsParseParameters;

    QTest::qExec(&testsFindIntermediates);
    QTest::qExec(&testsFindMergePoint);
    QTest::qExec(&testsCollectOverriddenMethods);
    QTest::qExec(&testsDetectRhomboidInheritance);
    QTest::qExec(&testsParseParameters);
    //test comment
}
