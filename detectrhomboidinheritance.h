#ifndef DETECTRHOMBOIDINHERITANCE_H
#define DETECTRHOMBOIDINHERITANCE_H

#include <QSet>
#include <QMap>
#include "rhombus.h"
#include "class.h"

QSet<Rhombus> detectRhomboidInheritance(const QMap<QString, Class*>& classes);


#endif // DETECTRHOMBOIDINHERITANCE_H
