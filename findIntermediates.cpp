#include "findintermediates.h"

QList<QString> findIntermediates(
    QString top,
    QString bottom,
    QMap<QString, QSet<QString>>& inheritanceMatrix,
    QMap<QString, Class*>& classes)
{
    Q_UNUSED(top);
    Q_UNUSED(bottom);
    Q_UNUSED(inheritanceMatrix);
    Q_UNUSED(classes);

    // Заглушка - возвращаем пустой список
    return {"ABC"};
}
