#ifndef FINDINTERMEDIATES_H
#define FINDINTERMEDIATES_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>

QList<QString> findIntermediates(
    const QString& top,
    const QString& bottom,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, void*>& classes
    );

#endif // FINDINTERMEDIATES_H
