/*!
 * \file
 * \brief Файл содержит реализацию тестов для функции parseParameters.
 */
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
    QTest::addRow("reference") << "char&amp; ref"
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
    QTest::addRow("referenceToConstType") << "const char&amp; ref"
                                          << (QList<Parameter>{Parameter("char", "ref", true, false, true)});

    // Тест 15: Пустая строка
    QTest::addRow("emptyString") << ""
                                 << (QList<Parameter>{});

    // Тест 16: Комбинация типов
    QTest::addRow("combinationOfTypes") << "int* const ptr, char&amp; ref, int a[5]"
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

    // Тест 19: Константный тип с указателем в другом порядке
    QTest::addRow("constTypeWithPointerReverse") << "int const* a"
                                          << (QList<Parameter>{Parameter("int", "a", true, false, false, true)});
}

void TestParseParameters::testParseParameters()
{
    QFETCH(QString, input);
    QFETCH(QList<Parameter>, expected);

    QList<Parameter> result = parseParameters(input);
    bool countError = false;
    bool paramsError = false;

    // Проверка количества параметров
    if (result.size() != expected.size()) {
        qDebug() << "ОШИБКА: Несоответствие количества параметров";
        qDebug() << "Входная строка:" << input;
        qDebug() << "Получено:" << result.size() << "Ожидалось:" << expected.size();
        countError = true;
    }

    // Проверка каждого параметра
    for (int i = 0; i < qMin(result.size(), expected.size()); ++i) {
        const Parameter& r = result[i];
        const Parameter& e = expected[i];
        bool hasMismatch = false;

        if (r.type != e.type) {
            qDebug() << "Параметр" << i+1 << "- несоответствие типа:"
                     << "Получено:" << r.type << "Ожидалось:" << e.type;
            hasMismatch = true;
        }
        if (r.name != e.name) {
            qDebug() << "Параметр" << i+1 << "- несоответствие имени:"
                     << "Получено:" << r.name << "Ожидалось:" << e.name;
            hasMismatch = true;
        }
        if (r.isTypeConst != e.isTypeConst) {
            qDebug() << "Параметр" << i+1 << "- несоответствие const типа:"
                     << "Получено:" << r.isTypeConst << "Ожидалось:" << e.isTypeConst;
            hasMismatch = true;
        }
        if (r.isPointerConst != e.isPointerConst) {
            qDebug() << "Параметр" << i+1 << "- несоответствие const указателя:"
                     << "Получено:" << r.isPointerConst << "Ожидалось:" << e.isPointerConst;
            hasMismatch = true;
        }
        if (r.isReference != e.isReference) {
            qDebug() << "Параметр" << i+1 << "- несоответствие ссылки:"
                     << "Получено:" << r.isReference << "Ожидалось:" << e.isReference;
            hasMismatch = true;
        }
        if (r.isPointer != e.isPointer) {
            qDebug() << "Параметр" << i+1 << "- несоответствие указателя:"
                     << "Получено:" << r.isPointer << "Ожидалось:" << e.isPointer;
            hasMismatch = true;
        }
        if (r.isArray != e.isArray) {
            qDebug() << "Параметр" << i+1 << "- несоответствие массива:"
                     << "Получено:" << r.isArray << "Ожидалось:" << e.isArray;
            hasMismatch = true;
        }
        if (r.isPointerToArray != e.isPointerToArray) {
            qDebug() << "Параметр" << i+1 << "- несоответствие указателя на массив:"
                     << "Получено:" << r.isPointerToArray << "Ожидалось:" << e.isPointerToArray;
            hasMismatch = true;
        }
        if (r.arrayDimensions != e.arrayDimensions) {
            qDebug() << "Параметр" << i+1 << "- несоответствие размерностей массива:"
                     << "Получено:" << r.arrayDimensions << "Ожидалось:" << e.arrayDimensions;
            hasMismatch = true;
        }

        if (hasMismatch) {
            qDebug() << "Для входной строки:" << input;
            paramsError = true;
        }
    }

    // Завершение теста
    if (countError && paramsError) {
        QFAIL("params count and values error!");
    } else if (countError) {
        QFAIL("params count error!");
    } else if (paramsError) {
        QFAIL("params values error!");
    }
}
