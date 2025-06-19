/*!
* \file
* \brief Заголовочный файл класса testCheckForOverriddenParameters для тестирования функции checkForOverriddenParameters.
*/
#ifndef TESTCHECKFOROVERRIDDENPARAMETERS_H
#define TESTCHECKFOROVERRIDDENPARAMETERS_H

#include <QObject>
#include <QTest>

/*!
 * \brief Класс для тестирования функций
 */
class TestCheckForOverriddenParameters : public QObject
{
    Q_OBJECT
private slots:
    void testCheckForOverriddenParameters_data();
    void testCheckForOverriddenParameters();

};

#endif // TESTCHECKFOROVERRIDDENPARAMETERS_H
