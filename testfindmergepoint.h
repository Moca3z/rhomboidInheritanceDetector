#ifndef TEST_FIND_MERGE_POINT_H
#define TEST_FIND_MERGE_POINT_H

#include <QObject>
#include <QTest>

class TestFindMergePoint : public QObject
{
    Q_OBJECT
private slots:
    void testFindMergePoint_data();
    void testFindMergePoint();

};

#endif // TEST_FIND_MERGE_POINT_H

/*
 * \brief находит промежуточные классы между bottom и top ромба
 * \param [in] top - вершина ромба
 * \param [in] bottom - основание ромба
 * \param [in] intermediates- набор классов между bottom и top
 * \param [in] inheritanceMatrix - матрица наследования, хранящая информацию о всех предках для каждого класса
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return первая найденная точка слияния в ромбе
*/
