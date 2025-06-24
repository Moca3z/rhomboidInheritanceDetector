/*!
* \file
* \brief Файл содержит реализацию функции findMergePoint.
*/
#include "findmergepoint.h"

QString findMergePoint(
    QString bottom,
    QString top,
    const QMultiMap<int, QString>& intermediates,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes
    ) {
    // Уникальные глубины в порядке возрастания
    QList<int> uniqueDepths = intermediates.uniqueKeys();

    for (int depth : uniqueDepths) {
        QList<QString> classesAtDepth = intermediates.values(depth);

        for (const QString& potentialMergePoint : classesAtDepth) {
            // Находим потомков potentialMergePoint
            QSet<QString> descendants;
            // Напрямую проверяем bottom
            if (inheritanceMatrix[bottom].contains(potentialMergePoint)) {
                descendants.insert(bottom);
            }
            // Проверяем другие классы из intermediates
            for (const QString& cls : intermediates.values()) {
                if (inheritanceMatrix[cls].contains(potentialMergePoint)) {
                    descendants.insert(cls);
                }
            }

            // Проверяем условие для всех потомков
            bool isMergePoint = true;
            for (const QString& descendant : descendants) {
                const QList<QString>& directAncestors = classes[descendant]->directAncestors;
                for (const QString& ancestor : directAncestors) {
                    // Сначала проверяем, равен ли ancestor top
                    if (ancestor == top) {
                        isMergePoint = false;
                        break;
                    }
                    // Пропускаем проверку, если ancestor не в intermediates
                    if (!intermediates.values().contains(ancestor)) {
                        continue;
                    }
                    if (ancestor != potentialMergePoint && !inheritanceMatrix[ancestor].contains(potentialMergePoint)) {
                        isMergePoint = false;
                        break;
                    }
                }
                if (!isMergePoint) break;
            }

            if (isMergePoint) {
                return potentialMergePoint;
            }
        }
    }

    return "";
}
