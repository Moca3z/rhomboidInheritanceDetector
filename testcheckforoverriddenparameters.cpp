/*!
 * \file
 * \brief Файл содержит реализацию тестов для функции checkForOverriddenParameters.
 */
#include "testcheckforoverriddenparameters.h"
#include "rhombus.h"
#include "class.h"
#include <QDebug>


void TestCheckForOverriddenParameters::testCheckForOverriddenParameters_data()
{
    QTest::addColumn<Parameter>("param1");
    QTest::addColumn<Parameter>("param2");
    QTest::addColumn<bool>("expected");

    // Тест 1: Полностью одинаковые числовые параметры
    Parameter p1_1 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_1 = {"int", "a", false, false, false, false, false, false, {}};
    QTest::addRow("identicalInt") << p1_1 << p2_1 << true;

    // Тест 2: Полностью одинаковые символьные параметры
    Parameter p1_2 = {"char", "a", false, false, false, false, false, false, {}};
    Parameter p2_2 = {"char", "a", false, false, false, false, false, false, {}};
    QTest::addRow("identicalChar") << p1_2 << p2_2 << true;

    // Тест 3: Разные имена
    Parameter p1_3 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_3 = {"int", "b", false, false, false, false, false, false, {}};
    QTest::addRow("differentNames") << p1_3 << p2_3 << true;

    // Тест 4: Наличие константности типа данных
    Parameter p1_4 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_4 = {"int", "a", true, false, false, false, false, false, {}};
    QTest::addRow("constType") << p1_4 << p2_4 << true;

    // Тест 5: Наличие указателя
    Parameter p1_5 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_5 = {"int", "a", false, false, false, true, false, false, {}};
    QTest::addRow("pointer") << p1_5 << p2_5 << false;

    // Тест 6: 2 указателя
    Parameter p1_6 = {"int", "a", false, false, false, true, false, false, {}};
    Parameter p2_6 = {"int", "b", false, false, false, true, false, false, {}};
    QTest::addRow("twoPointers") << p1_6 << p2_6 << true;

    // Тест 7: Наличие ссылки
    Parameter p1_7 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_7 = {"int", "a", false, false, true, false, false, false, {}};
    QTest::addRow("reference") << p1_7 << p2_7 << false;

    // Тест 8: Наличие константности типа данных и указателя
    Parameter p1_8 = {"int", "a", false, false, false, true, false, false, {}};
    Parameter p2_8 = {"int", "a", true, false, false, true, false, false, {}};
    QTest::addRow("constPointerType") << p1_8 << p2_8 << false;

    // Тест 9: Наличие константности типа данных и ссылки
    Parameter p1_9 = {"int", "a", false, false, true, false, false, false, {}};
    Parameter p2_9 = {"int", "a", true, false, true, false, false, false, {}};
    QTest::addRow("constReferenceType") << p1_9 << p2_9 << false;

    // Тест 10: Наличие массива
    Parameter p1_10 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_10 = {"int", "a", false, false, false, false, true, false, {-1}};
    QTest::addRow("array") << p1_10 << p2_10 << false;


    // Тест 11: 2 одномерных массива одинакового размера
    Parameter p1_11 = {"int", "a", false, false, false, false, true, false, {-1}};
    Parameter p2_11 = {"int", "b", false, false, false, false, true, false, {-1}};
    QTest::addRow("identicalArrays") << p1_11 << p2_11 << true;

    // Тест 12: 2 одномерных массива разных размеров
    Parameter p1_12 = {"int", "a", false, false, false, false, true, false, {4}};
    Parameter p2_12 = {"int", "b", false, false, false, false, true, false, {5}};
    QTest::addRow("differentArraySizes") << p1_12 << p2_12 << false;

    // Тест 13: одномерный массив и указатель
    Parameter p1_13 = {"int", "a", false, false, false, false, true, false, {-1}};
    Parameter p2_13 = {"int", "a", false, false, false, true, false, false, {}};
    QTest::addRow("oneDimensionalArrayAndPointer") << p1_13 << p2_13 << false;

    // Тест 14: Наличие многомерного массива
    Parameter p1_14 = {"int", "a", false, false, false, false, false, false, {}};
    Parameter p2_14 = {"int", "a", false, false, false, false, true, false, {-1, 5, 6}};
    QTest::addRow("multiArray") << p1_14 << p2_14 << false;

    // Тест 15: Идентичный многомерный массив с равными размерами
    Parameter p1_15 = {"int", "a", false, false, false, false, true, false, {-1, 5, 6}};
    Parameter p2_15 = {"int", "b", false, false, false, false, true, false, {-1, 5, 6}};
    QTest::addRow("identicalMultiArray") << p1_15 << p2_15 << true;

    // Тест 16: Идентичный многомерный массив с отличающимися размерами
    Parameter p1_16 = {"int", "a", false, false, false, false, true, false, {-1, 5, 6}};
    Parameter p2_16 = {"int", "b", false, false, false, false, true, false, {-1, 5, 8}};
    QTest::addRow("differentMultiArraySizes") << p1_16 << p2_16 << false;

    // Тест 17: Разная мерность массива
    Parameter p1_17 = {"int", "a", false, false, false, false, true, false, {-1, 5, 6}};
    Parameter p2_17 = {"int", "b", false, false, false, false, true, false, {-1, 5, 8, 9}};
    QTest::addRow("differentArrayDimensions") << p1_17 << p2_17 << false;

    // Тест 18: Многомерный массив и указатель на массив такой же мерности и размерности
    Parameter p1_18 = {"int", "a", false, false, false, false, true, false, {-1, 5, 6}};
    Parameter p2_18 = {"int", "a", false, false, false, true, false, true, {5, 6}};
    QTest::addRow("arrayVsPointerToArray") << p1_18 << p2_18 << false;

    // Тест 19: Многомерный массив и указатель на массив другой мерности
    Parameter p1_19 = {"int", "a", false, false, false, false, true, false, {-1, 5, 6}};
    Parameter p2_19 = {"int", "a", false, false, false, true, false, true, {5, 6, 8}};
    QTest::addRow("differentPointerToArray") << p1_19 << p2_19 << false;

    // Тест 20: Константность типа с массивом
    Parameter p1_20 = {"int", "a", false, false, false, false, true, false, {10}};
    Parameter p2_20 = {"int", "b", true, false, false, false, true, false, {10}};
    QTest::addRow("constArray") << p1_20 << p2_20 << false;

    // Тест 21: Константность типа данных и указателя
    Parameter p1_21 = {"int", "a", true, false, false, true, false, false, {}};
    Parameter p2_21 = {"int", "b", false, true, false, true, false, false, {}};
    QTest::addRow("constPointerAndDataType") << p1_21 << p2_21 << false;

    // Тест 22: указатели на константный тип
    Parameter p1_22 = {"int", "a", true, false, false, true, false, false, {}};
    Parameter p2_22 = {"int", "b", true, false, false, true, false, false, {}};
    QTest::addRow("constPointerDataTypeReversed") << p1_22 << p2_22 << true;

    // Тест 23: указатель на константный тип и константный указатель
    Parameter p1_23 = {"int", "a", true, false, false, true, false, false, {}};
    Parameter p2_23 = {"int", "b", false, true, false, true, false, false, {}};
    QTest::addRow("constPointerAndDataTypeReversed") << p1_23 << p2_23 << false;

}

void TestCheckForOverriddenParameters::testCheckForOverriddenParameters()
{
    QFETCH(Parameter, param1);
    QFETCH(Parameter, param2);
    QFETCH(bool, expected);

    bool result = checkForOverriddenParameters(param1, param2);

    if (result != expected) {
        qDebug() << "Тест провалился для параметров:";
        qDebug() << "param1: { type:" << param1.type << ", name:" << param1.name
                 << ", isTypeConst:" << param1.isTypeConst
                 << ", isPointerConst:" << param1.isPointerConst
                 << ", isReference:" << param1.isReference
                 << ", isPointer:" << param1.isPointer
                 << ", isArray:" << param1.isArray
                 << ", isPointerToArray:" << param1.isPointerToArray
                 << ", arrayDimensions:" << param1.arrayDimensions << "}";
        qDebug() << "param2: { type:" << param2.type << ", name:" << param2.name
                 << ", isTypeConst:" << param2.isTypeConst
                 << ", isPointerConst:" << param2.isPointerConst
                 << ", isReference:" << param2.isReference
                 << ", isPointer:" << param2.isPointer
                 << ", isArray:" << param2.isArray
                 << ", isPointerToArray:" << param2.isPointerToArray
                 << ", arrayDimensions:" << param2.arrayDimensions << "}";
        qDebug() << "Expected:" << expected << ", Actual:" << result;
    }

    QCOMPARE(result, expected);
}
