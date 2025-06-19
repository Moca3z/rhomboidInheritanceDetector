/*!
* \file
* \brief Заголовочный файл класса функции findIntermediates.
*/
#ifndef FINDINTERMEDIATES_H
#define FINDINTERMEDIATES_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"
#include <QMultiMap>
#include <QQueue>

/*!
 * \brief находит промежуточные классы между bottom и top ромба
 * \param [in] top - вершина ромба
 * \param [in] bottom - основание ромба
 * \param [in] inheritanceMatrix - матрица наследования, хранящая информацию о всех предках для каждого класса
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return ассоциативный контейнер с информацией о классах между bottom и top, где ключ - глубина кратчайшего наследования класса от bottom
 */
QMultiMap<int, QString> findIntermediates(
    QString top,
    QString bottom,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes
    );

#endif // FINDINTERMEDIATES_H
