/*!
* \file
* \brief Заголовочный файл класса функции findMergePoint.
*/
#ifndef FIND_MERGE_POINT_H
#define FIND_MERGE_POINT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"

/*!
 * \brief находит первую найденную точку слияния в ромбе
 * \param [in] bottom - основание ромба
 * \param [in] top - вершина ромба
 * \param [in] intermediates- набор классов между bottom и top и глубинами их наследования от bottom
 * \param [in] inheritanceMatrix - матрица наследования, хранящая информацию о всех предках для каждого класса
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return первая найденная точка слияния в ромбе
 */
QString findMergePoint(
    QString bottom,
    QString top,
    const QMultiMap<int, QString>& intermediates,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes
    );

#endif // FIND_MERGE_POINT_H
