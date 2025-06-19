/*!
* \file
* \brief Заголовочный файл класса testFindIntermediates для тестирования функции findIntermediates.
*/
#ifndef TESTFINDINTERMEDIATES_H
#define TESTFINDINTERMEDIATES_H

#include <QObject>
#include <QtTest/QtTest>

/*!
 * \brief Класс для тестирования функций
 */
class TestFindIntermediates : public QObject
{
    Q_OBJECT

private slots:
    void testFindIntermediates_data();
    void testFindIntermediates();
};

#endif // TESTFINDINTERMEDIATES_H

