/*!
* \file
* \brief Заголовочный файл функции collectOverriddenMethods.
*/
#ifndef COLLECTOVERRIDDENMETHODS_H
#define COLLECTOVERRIDDENMETHODS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"

/*!
 * \brief находит промежуточные классы между bottom и top ромба
 * \param [in] top - вершина ромба
 * \param [in] bottom - основание ромба
 * \param [in] mergePoint- первая найденная точка слияния ромба
 * \param [in] intermediates- набор классов между bottom и top и глубинами их наследования от bottom
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \param [in] inheritanceMatrix - матрица наследования, хранящая информацию о всех предках для каждого класса
 * \return набор классов, в которых были переопределены методы класса top
 */
QMap<QString, QList<Method*>> collectOverriddenMethods(
    QString bottom,
    QString top,
    QString mergePoint,
    QMultiMap<int, QString> intermediates,
    QMap<QString, Class*> classes,
    QMap<QString, QSet<QString>> inheritanceMatrix
    );



#endif
