#ifndef FIND_MERGE_POINT_H
#define FIND_MERGE_POINT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"

QString findMergePoint(
    const QString& bottom,
    const QString& top,
    const QList<QString>& intermediates,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes
    );

#endif // FIND_MERGE_POINT_H
