#ifndef FIND_MERGE_POINT_H
#define FIND_MERGE_POINT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"

QString findMergePoint(
    QString& bottom,
    QString& top,
    QList<QString>& intermediates,
    QMap<QString, QSet<QString>>& inheritanceMatrix,
    QMap<QString, Class*>& classes
    );

#endif // FIND_MERGE_POINT_H
