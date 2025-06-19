/*!
* \file
* \brief Файл содержит реализацию функции findIntermediates.
*/
#include "findintermediates.h"


QMultiMap<int, QString> findIntermediates(
    QString top,
    QString bottom,
    const QMap<QString, QSet<QString>>& inheritanceMatrix,
    const QMap<QString, Class*>& classes)
{
    QMultiMap<int, QString> intermediates;

    // Если bottom и top совпадают или top не предок bottom, возвращаем пустой результат
    if (bottom == top || !inheritanceMatrix[bottom].contains(top)) {
        return intermediates;
    }

    QSet<QString> visited;                   // Множество посещённых классов
    QQueue<QPair<QString, int>> queue;      // Очередь для BFS: (класс, расстояние)
    queue.enqueue(qMakePair(bottom, 0)); // Начинаем с bottom на расстоянии 0
    visited.insert(bottom);

    while (!queue.isEmpty()) {
        QPair<QString, int> pair = queue.dequeue();
        QString current = pair.first;
        int distance = pair.second;

        // Проверяем наличие текущего класса в classes
        if (classes.contains(current)) {
            const QList<QString>& ancestors = classes[current]->directAncestors;
            for (const QString& ancestor : ancestors) {
                if (!visited.contains(ancestor)) {
                    visited.insert(ancestor);
                    queue.enqueue(qMakePair(ancestor, distance + 1));
                    // Если ancestor лежит на пути к top и не является top
                    if (ancestor != top && inheritanceMatrix[ancestor].contains(top)) {
                        intermediates.insert(distance + 1, ancestor);
                    }
                }
            }
        }
    }

    return intermediates;
}
