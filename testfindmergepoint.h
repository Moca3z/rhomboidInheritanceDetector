/*!
* \file
* \brief Заголовочный файл класса testFindMergePoint для тестирования функции findMergePoint.
*/
#ifndef TEST_FIND_MERGE_POINT_H
#define TEST_FIND_MERGE_POINT_H

#include <QObject>
#include <QTest>

/*!
 * \brief Класс для тестирования функций
 */
class TestFindMergePoint : public QObject
{
    Q_OBJECT
private slots:
    void testFindMergePoint_data();
    void testFindMergePoint();

};

#endif // TEST_FIND_MERGE_POINT_H
