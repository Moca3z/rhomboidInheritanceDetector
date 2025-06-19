/*!
* \file
* \brief Заголовочный файл класса testCollectOverriddenMethods для тестирования функции collectOverriddenMethods.
*/
#ifndef TESTCOLLECTOVERRIDDENMETHODS_H
#define TESTCOLLECTOVERRIDDENMETHODS_H

#include <QObject>
#include <QTest>

/*!
 * \brief Класс для тестирования функций
 */
class TestCollectOverridden : public QObject
{
    Q_OBJECT
private slots:
    void testCollectOverridden_data();
    void testCollectOverridden();

};

#endif // TESTCOLLECTOVERRIDDENMETHODS_H
