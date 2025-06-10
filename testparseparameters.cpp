#include "testparseparameters.h"
#include "rhombus.h"
#include "class.h"
#include <QDebug>


void TestParseParameters::testParseParameters_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QList<Parameter>>("expected");

    // Тест 1: Числовой тип
    QTest::addRow("numericType") << "int a"
                                 << (QList<Parameter>{Parameter("int", "a")});

    // Тест 2: Символьный тип
    QTest::addRow("charType") << "char top"
                              << (QList<Parameter>{Parameter("char", "top")});

    // Тест 3: Два разных типа
    QTest::addRow("twoDifferentTypes") << "int a, char b"
                                       << (QList<Parameter>{Parameter("int", "a"), Parameter("char", "b")});

    // Тест 4: Константный тип
    QTest::addRow("constType") << "const int a"
                               << (QList<Parameter>{Parameter("int", "a", true)});

    // Тест 5: Указатель
    QTest::addRow("pointer") << "int* a"
                             << (QList<Parameter>{Parameter("int", "a", false, false, false, true)});

    // Тест 6: Константный указатель
    QTest::addRow("constPointer") << "int* const a"
                                  << (QList<Parameter>{Parameter("int", "a", false, true, false, true)});

    // Тест 7: Ссылка
    QTest::addRow("reference") << "char& ref"
                               << (QList<Parameter>{Parameter("char", "ref", false, false, true)});

    // Тест 8: Одномерный массив
    QTest::addRow("oneDimensionalArray") << "int a[]"
                                         << (QList<Parameter>{Parameter("int", "a", false, false, false, false, true, false, {-1})});

    // Тест 9: Многомерный массив
    QList<int> multiDims = {-1, 5, 6};
    QTest::addRow("multiDimensionalArray") << "int a[][5][6]"
                                           << (QList<Parameter>{Parameter("int", "a", false, false, false, false, true, false, multiDims)});

    // Тест 10: Указатель на массив
    QList<int> ptrToArrDims = {-1, 5, 6};
    QTest::addRow("pointerToArray") << "int (*a)[5][6]"
                                    << (QList<Parameter>{Parameter("int", "a", false, false, false, true, false, true, ptrToArrDims)});

    // Тест 11: Константный указатель на массив
    QTest::addRow("constPointerToArray") << "int (*const a)[5][6]"
                                         << (QList<Parameter>{Parameter("int", "a", false, true, false, true, false, true, ptrToArrDims)});

    // Тест 12: Константный тип с указателем
    QTest::addRow("constTypeWithPointer") << "const int* a"
                                          << (QList<Parameter>{Parameter("int", "a", true, false, false, true)});

    // Тест 13: Константный тип с массивом
    QTest::addRow("constTypeWithArray") << "const int a[10]"
                                        << (QList<Parameter>{Parameter("int", "a", true, false, false, false, true, false, {10})});

    // Тест 14: Ссылка на константный тип
    QTest::addRow("referenceToConstType") << "const char& ref"
                                          << (QList<Parameter>{Parameter("char", "ref", true, false, true)});

    // Тест 15: Пустая строка
    QTest::addRow("emptyString") << ""
                                 << (QList<Parameter>{});

    // Тест 16: Комбинация типов
    QTest::addRow("combinationOfTypes") << "int* const ptr, char& ref, int a[5]"
                                        << (QList<Parameter>{
                                               Parameter("int", "ptr", false, true, false, true),
                                               Parameter("char", "ref", false, false, true),
                                               Parameter("int", "a", false, false, false, false, true, false, {5})
                                           });

    // Тест 17: Const перед типом и после указателя
    QTest::addRow("constBeforeAndAfterPointer") << "const int* const a"
                                                << (QList<Parameter>{Parameter("int", "a", true, true, false, true)});

    // Тест 18: Const перед типом и после указателя на массив
    QTest::addRow("constBeforeAndAfterPointerToArray") << "const int (*const a)[5][6]"
                                                       << (QList<Parameter>{Parameter("int", "a", true, true, false, true, false, true, ptrToArrDims)});
}

void TestParseParameters::testParseParameters()
{
    QFETCH(QString, input);
    QFETCH(QList<Parameter>, expected);

    QList<Parameter> result = parseParameters(input);
    QCOMPARE(result.size(), expected.size());

    for (int i = 0; i < result.size(); ++i) {
        const Parameter& r = result[i];
        const Parameter& e = expected[i];
        QCOMPARE(r.type, e.type);
        QCOMPARE(r.name, e.name);
        QCOMPARE(r.isTypeConst, e.isTypeConst);
        QCOMPARE(r.isPointerConst, e.isPointerConst);
        QCOMPARE(r.isReference, e.isReference);
        QCOMPARE(r.isPointer, e.isPointer);
        QCOMPARE(r.isArray, e.isArray);
        QCOMPARE(r.isPointerToArray, e.isPointerToArray);
        QCOMPARE(r.arrayDimensions, e.arrayDimensions);
    }
}
