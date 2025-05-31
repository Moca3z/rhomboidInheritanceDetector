#ifndef TESTFINDINTERMEDIATES_H
#define TESTFINDINTERMEDIATES_H

#include <QObject>
#include <QtTest/QtTest>

class TestFindIntermediates : public QObject
{
    Q_OBJECT

private slots:
    void testFindIntermediates_data();
    void testFindIntermediates();
};

#endif // TESTFINDINTERMEDIATES_H

/*
 * \brief находит промежуточные классы между bottom и top ромба
 * \param [in] top - вершина ромба
 * \param [in] bottom - основание ромба
 * \param [in] inheritanceMatrix - матрица наследования, хранящая информацию о всех предках для каждого класса
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return набор классов между bottom и top
*/

