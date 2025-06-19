/*!
* \file
* \brief Файл содержит реализацию функции collectOverriddenMethods.
*/
#include "collectoverriddenmethods.h"
#include "rhombus.h"

QMap<QString, QList<Method*>> collectOverriddenMethods(
    QString bottom,
    QString top,
    QString mergePoint,
    QMultiMap<int, QString> intermediates,
    QMap<QString, Class*> classes,
    QMap<QString, QSet<QString>> inheritanceMatrix
    ) {
    QMap<QString, QList<Method*>> overriddenMethods;
    QList<QString> ancestorsBeforeMergePoint;

    Class* topClass = classes[top];

    // Шаг 1: Определение классов для проверки (предки до точки слияния или все промежуточные)
    if (!mergePoint.isEmpty() && mergePoint != "Null") {
        for (const QString& candidate : intermediates) {
            if (inheritanceMatrix[candidate].contains(mergePoint)) {
                ancestorsBeforeMergePoint.append(candidate);
            }
        }
    } else {
        // Если точки слияния нет, включаем все промежуточные классы
        ancestorsBeforeMergePoint = intermediates.values();
    }

    // Шаг 2: Проверка каждого кандидатского класса на переопределенные методы
    for (const QString& candidate : ancestorsBeforeMergePoint) {
        if (!classes.contains(candidate) || !classes.contains(top)) {
            continue; // Пропускаем, если классы отсутствуют в карте
        }

        Class* candidateClass = classes[candidate];


        // Шаг 3: Сравнение методов кандидатского класса с верхним классом
        for (Method* candidateMethod : candidateClass->methods) {
            for (Method* topMethod : topClass->methods) {
                // Проверка совпадения имен методов
                if (candidateMethod->methodName == topMethod->methodName) {
                    // Проверка совпадения типов возвращаемого значения
                    if (candidateMethod->returnType == topMethod->returnType) {
                        // Проверка одинаковой длины списков параметров
                        if (candidateMethod->parameters.size() == topMethod->parameters.size()) {
                            bool parametersMatch = true;
                            // Сравнение каждого параметра с помощью checkForOverriddenParameters
                            for (int i = 0; i < candidateMethod->parameters.size(); ++i) {
                                if (!checkForOverriddenParameters(candidateMethod->parameters[i], topMethod->parameters[i])) {
                                    parametersMatch = false;
                                    break;
                                }
                            }
                            // Если все параметры совпадают, это переопределение
                            if (parametersMatch) {
                                overriddenMethods[candidate].append(candidateMethod);
                            }
                        }
                    }
                }
            }
        }
    }

    return overriddenMethods;
}
