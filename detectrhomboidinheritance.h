#ifndef DETECTRHOMBOIDINHERITANCE_H
#define DETECTRHOMBOIDINHERITANCE_H

#include <QSet>
#include <QMap>
#include "rhombus.h"
#include "class.h"
#include "findIntermediates.h"
#include "findmergepoint.h"
#include "collectoverriddenmethods.h"

QSet<Rhombus> detectRhomboidInheritance(const QMap<QString, Class*>& classes);


#endif // DETECTRHOMBOIDINHERITANCE_H
