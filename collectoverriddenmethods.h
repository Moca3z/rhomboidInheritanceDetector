#ifndef COLLECTOVERRIDDENMETHODS_H
#define COLLECTOVERRIDDENMETHODS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"

QMap<QString, QList<Method*>> collectOverriddenMethods(
    QString bottom,
    QString top,
    QString mergePoint,
    QList<QString> intermediates,
    QMap<QString, Class*> classes,
    QMap<QString, QSet<QString>> inheritanceMatrix
    );

#endif
