#include <QCoreApplication>
#include "testFindIntermediates.h"
#include "testfindmergepoint.h"
#include "testcollectoverriddenmethods.h"
#include "testdetectrhomboidinheritance.h"
#include "testparseparameters.h"
#include "testcheckforoverriddenparameters.h"
#include <QDebug>
#include "rhombus.h"
#include <QTextStream>
#include <clocale>
#include "detectrhomboidinheritance.h"
#include "classdiagram.h"
#include <QDir>
#include <QFile>

/* int main(int argc, char *argv[])
{

    TestFindIntermediates testsFindIntermediates;
    TestFindMergePoint testsFindMergePoint;
    TestCollectOverridden testsCollectOverriddenMethods;
    TestDetectRhomboid testsDetectRhomboidInheritance;
    TestParseParameters testsParseParameters;
    TestCheckForOverriddenParameters testsCheckForOverriddenParameters;

    QTest::qExec(&testsFindIntermediates);
    QTest::qExec(&testsFindMergePoint);
    QTest::qExec(&testsParseParameters);
    QTest::qExec(&testsCheckForOverriddenParameters);
    QTest::qExec(&testsCollectOverriddenMethods);
    QTest::qExec(&testsDetectRhomboidInheritance);

    //test comment
}
*/

int main(int argc, char* argv[]) {


    // Установка локали для поддержки русского языка
    setlocale(LC_ALL, "Russian_Russia.1251");

    // Если аргумент не указан, используем testxml4.txt по умолчанию
    QString inputFile = (argc > 1) ? argv[1] : "testerrorxml10.txt";

    qDebug() << "Обработка файла:" << inputFile;
    auto [classes, errors] = parseXmlFile(inputFile);

    if (!errors.isEmpty()) {
        qDebug() << "Обнаружены ошибки:";
        for (const Error& error : errors) {
            qDebug() << error.message();
        }
        qDeleteAll(classes);
        return 1;
    }

    qDebug() << "Классы:";
    for (const auto& cls : classes) {
        qDebug() << "Класс:" << cls->className;
        qDebug() << "  Предки:" << cls->directAncestors.join(", ");
        qDebug() << "  Методы:";
        for (const Method* method : cls->methods) {
            qDebug() << "    Метод:" << method->methodName
                     << "Виртуальный:" << method->isVirtual
                     << "Тип возврата:" << method->returnType;
            qDebug() << "    Параметры:";
            for (const Parameter& param : method->parameters) {
                qDebug() << "      Тип:" << param.type
                         << "Имя:" << param.name
                         << "Const типа:" << param.isTypeConst
                         << "Const указателя:" << param.isPointerConst
                         << "Ссылка:" << param.isReference
                         << "Указатель:" << param.isPointer
                         << "Массив:" << param.isArray
                         << "Указатель на массив:" << param.isPointerToArray
                         << "Размерности:" << param.arrayDimensions;
            }
        }
    }

    qDeleteAll(classes);
    qDebug() << "Нажмите Enter для завершения...";
    QTextStream qin(stdin);
    qin.readLine();
    return 0;
}

