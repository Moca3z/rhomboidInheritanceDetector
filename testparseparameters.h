/*!
* \file
* \brief Заголовочный файл класса testParseParameters для тестирования функции parseParameters.
*/
#ifndef TESTPARSEPARAMETERS_H
#define TESTPARSEPARAMETERS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QTest>
#include <QDebug>

/*!
 * \brief Класс для тестирования функций
 */
class TestParseParameters : public QObject
{
    Q_OBJECT
private slots:
    void testParseParameters_data();
    void testParseParameters();

};

#endif // TESTPARSEPARAMETERS_H
