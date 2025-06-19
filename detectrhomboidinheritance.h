/*!
* \file
* \brief Заголовочный файл функции detectRhomboidInheritance.
*/
#ifndef DETECTRHOMBOIDINHERITANCE_H
#define DETECTRHOMBOIDINHERITANCE_H

#include <QSet>
#include <QMap>
#include "rhombus.h"
#include "class.h"
#include "findIntermediates.h"
#include "findmergepoint.h"
#include "collectoverriddenmethods.h"

/*!
 * \brief находит случаи ромбовидного наследования в наборе классов
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return return набор ромбов, в которых возникло ромбовидное наследование
 */
QSet<Rhombus> detectRhomboidInheritance(const QMap<QString, Class*>& classes);


#endif // DETECTRHOMBOIDINHERITANCE_H
