#include "findIntermediates.h"


QList<QString> findIntermediates(
    const QString& top,
    const QString& bottom,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, void*>& classes
    ) {
    Q_UNUSED(top);
    Q_UNUSED(bottom);
    Q_UNUSED(inheritanceMatrix);
    Q_UNUSED(classes);
    return {}; // Возвращаем пустой список
}
