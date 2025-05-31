#include "findmergepoint.h"

QString findMergePoint(
    const QString& bottom,
    const QString& top,
    const QList<QString>& intermediates,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes
    ) {
    Q_UNUSED(bottom);
    Q_UNUSED(top);
    Q_UNUSED(intermediates);
    Q_UNUSED(inheritanceMatrix);
    Q_UNUSED(classes);

    // Заглушка: всегда возвращаем некорректное значение
    return "InvalidMergePoint";
}
