#ifndef FIND_MERGE_POINT_H
#define FIND_MERGE_POINT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"

QString findMergePoint(
    QString bottom,
    QString top,
    const QMultiMap<int, QString>& intermediates,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes
    );

#endif // FIND_MERGE_POINT_H
