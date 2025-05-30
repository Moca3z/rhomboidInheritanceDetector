#include <QCoreApplication>
#include "testFindIntermediates.h"

int main(int argc, char *argv[])
{

    TestFindIntermediates tests;
    QTest::qExec(&tests);

}
