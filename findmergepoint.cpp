#include "findmergepoint.h"

QString findMergePoint(
    QString& bottom,
    QString& top,
    QMultiMap<int, QString>& intermediates,
    QMap<QString, QSet<QString>>& inheritanceMatrix,
    QMap<QString, Class*>& classes
    ) {
    Q_UNUSED(bottom);
    Q_UNUSED(top);
    Q_UNUSED(intermediates);
    Q_UNUSED(inheritanceMatrix);
    Q_UNUSED(classes);

    // Заглушка: всегда возвращаем некорректное значение
    return "InvalidMergePoint";
}
