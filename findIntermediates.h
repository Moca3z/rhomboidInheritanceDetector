#ifndef FINDINTERMEDIATES_H
#define FINDINTERMEDIATES_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"
#include <QMultiMap>
#include <QQueue>

QMultiMap<int, QString> findIntermediates(
    QString top,
    QString bottom,
    QMap<QString, QSet<QString>>& inheritanceMatrix,
    QMap<QString, Class*>& classes
    );

#endif // FINDINTERMEDIATES_H
