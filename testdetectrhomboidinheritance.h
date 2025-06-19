/*!
* \file
* \brief Заголовочный файл класса testDetectRhomboidInheritance для тестирования функции detectRhomboidInheritance.
*/
#ifndef TESTDETECTRHOMBOIDINHERITANCE_H
#define TESTDETECTRHOMBOIDINHERITANCE_H

#include <QObject>
#include <QTest>

/*!
 * \brief Класс для тестирования функций
 */
class TestDetectRhomboid : public QObject
{
    Q_OBJECT
private slots:
    void testDetectRhomboidInheritance_data();
    void testDetectRhomboidInheritance();


};

#endif // TESTDETECTRHOMBOIDINHERITANCE_H
