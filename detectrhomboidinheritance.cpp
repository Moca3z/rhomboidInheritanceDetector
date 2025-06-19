/*!
* \file
* \brief Файл содержит реализацию функции detectRhomboidInheritance.
*/
#include "detectrhomboidinheritance.h"


QSet<Rhombus> detectRhomboidInheritance(const QMap<QString, Class*>& classes)
{
    QSet<Rhombus> rhombuses;

    // Шаг 1: Построить матрицу наследования
    QMap<QString, QSet<QString>> inheritanceMatrix = buildInheritanceMatrix(classes);

    // Шаг 2: Для каждого класса (потенциального "нижнего" класса ромба)
    for (const QString& bottom : classes.keys()) {
        Class* bottomClass = classes.value(bottom);

        // Проверяем, имеет ли класс более одного прямого предка
        if (bottomClass->directAncestors.size() > 1) {
            // Шаг 3: Для каждого предка bottom (потенциального "верхнего" класса ромба)
            for (const QString& top : inheritanceMatrix[bottom]) {
                // Шаг 3.1: Найти промежуточные узлы между bottom и top
                QMultiMap<int, QString> intermediates = findIntermediates(top, bottom, inheritanceMatrix, classes);

                // Шаг 3.2: Найти первую точку слияния
                QString mergePoint = findMergePoint(bottom, top, intermediates, inheritanceMatrix, classes);

                // Шаг 3.3: Проверить наличие переопределенных методов
                QMap<QString, QList<Method*>> overriddenMethods = collectOverriddenMethods(bottom, top, mergePoint, intermediates, classes, inheritanceMatrix);

                // Шаг 3.4: Если есть переопределенные методы, создать объект Rhombus
                if (!overriddenMethods.isEmpty()) {
                    Rhombus rhombus;
                    rhombus.bottom = bottom;
                    rhombus.top = top;
                    rhombus.overriddenMethods = overriddenMethods;

                    // Заполняем intermediates для Rhombus
                    for (const QString& intermediate : intermediates) {
                        rhombus.intermediates.insert(intermediates.key(intermediate), intermediate);
                    }

                    rhombuses.insert(rhombus);
                }
            }
        }
    }

    return rhombuses;
}
