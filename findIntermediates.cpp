#include "findintermediates.h"

QMultiMap<int, QString> findIntermediates(
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
    QMultiMap<int, QString> test;
    test.insert(1, "Z");

    return test;
}
