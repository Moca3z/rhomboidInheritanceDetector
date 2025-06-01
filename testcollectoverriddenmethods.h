#ifndef TESTCOLLECTOVERRIDDENMETHODS_H
#define TESTCOLLECTOVERRIDDENMETHODS_H

#include <QObject>
#include <QTest>

class TestCollectOverridden : public QObject
{
    Q_OBJECT
private slots:
    void testCollectOverridden_data();
    void testCollectOverridden();

};

#endif // TESTCOLLECTOVERRIDDENMETHODS_H

/*
 * \brief находит промежуточные классы между bottom и top ромба
 * \param [in] top - вершина ромба
 * \param [in] bottom - основание ромба
 * \param [in] mergePoint- первая найденная точка слияния ромба
 * \param [in] intermediates- набор классов между bottom и top
 * \param [in] inheritanceMatrix - матрица наследования, хранящая информацию о всех предках для каждого класса
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return набор классов, в которых были переопределены методы класса top
*/
