#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"
#include <QDebug>
#include <QMultiMap>

struct Rhombus {
    QString bottom;
    QString top;
    QMultiMap<int, QString> intermediates;
    QMap<QString, QList<Method*>> overriddenMethods;

    Rhombus(const QString& b = QString(), const QString& t = QString(),
            const QMultiMap<int, QString>& i = QMultiMap<int, QString>(),
            const QMap<QString, QList<Method*>>& m = QMap<QString, QList<Method*>>())
        : bottom(b), top(t), intermediates(i), overriddenMethods(m) {}


    // Полное сравнение всех полей, включая методы
    bool operator==(const Rhombus& other) const {
        // 1. Сравнение базовых полей
        if (bottom != other.bottom || top != other.top || intermediates != other.intermediates) {
            return false;
        }

        // 2. Сравнение переопределённых методов
        if (overriddenMethods.size() != other.overriddenMethods.size()) {
            return false;
        }

        // 3. Поэлементное сравнение методов
        auto it1 = overriddenMethods.constBegin();
        auto it2 = other.overriddenMethods.constBegin();

        while (it1 != overriddenMethods.constEnd()) {
            // 3.1. Проверка имени класса
            if (it1.key() != it2.key()) {
                return false;
            }

            // 3.2. Проверка количества методов
            const QList<Method*>& methods1 = it1.value();
            const QList<Method*>& methods2 = it2.value();

            if (methods1.size() != methods2.size()) {
                return false;
            }

            // 3.3. Проверка каждого метода
            for (int i = 0; i < methods1.size(); ++i) {
                const Method* m1 = methods1[i];
                const Method* m2 = methods2[i];

                if (m1->methodName != m2->methodName ||
                    m1->isVirtual != m2->isVirtual ||
                    m1->returnType != m2->returnType ||
                    m1->parameters.size() != m2->parameters.size()) {
                    return false;
                }

                // 3.4. Проверка параметров методов
                for (int j = 0; j < m1->parameters.size(); ++j) {
                    const Parameter& p1 = m1->parameters[j];
                    const Parameter& p2 = m2->parameters[j];

                    if (p1.type != p2.type ||
                        p1.name != p2.name ||
                        p1.isTypeConst != p2.isTypeConst ||
                        p1.isPointerConst != p2.isPointerConst ||
                        p1.isReference != p2.isReference ||
                        p1.isPointer != p2.isPointer ||
                        p1.isArray != p2.isArray ||
                        p1.isPointerToArray != p2.isPointerToArray ||
                        p1.arrayDimensions != p2.arrayDimensions) {
                        return false;
                    }
                }
            }

            ++it1;
            ++it2;
        }

        return true;
    }
};

// Функция хеширования для QMultiMap<int, QString>
inline size_t qHash(const QMultiMap<int, QString>& map, size_t seed = 0) {
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        seed = qHash(it.key(), seed);        // Хешируем ключ (int)
        seed = qHash(it.value(), seed);      // Хешируем значение (QString)
    }
    return seed;
}

// Функция хеширования для Rhombus
inline size_t qHash(const Rhombus& rhombus, size_t seed = 0) {
    seed = qHash(rhombus.bottom, seed);
    seed = qHash(rhombus.top, seed);
    seed = qHash(rhombus.intermediates, seed); // Теперь работает с QMultiMap<int, QString>

    for (auto it = rhombus.overriddenMethods.constBegin(); it != rhombus.overriddenMethods.constEnd(); ++it) {
        seed = qHash(it.key(), seed);
        for (const Method* method : it.value()) {
            seed = qHash(method->methodName, seed);
            seed = qHash(method->returnType, seed);
            seed = qHash(method->isVirtual, seed);
            for (const Parameter& param : method->parameters) {
                seed = qHash(param.type, seed);
                seed = qHash(param.name, seed);
                seed = qHash(param.isTypeConst, seed);
                seed = qHash(param.isPointerConst, seed);
                seed = qHash(param.isReference, seed);
                seed = qHash(param.isPointer, seed);
                seed = qHash(param.isArray, seed);
                seed = qHash(param.isPointerToArray, seed);
                seed = qHash(param.arrayDimensions, seed);
            }
        }
    }

    return seed;
}

QList<Parameter> parseParameters(const QString& parametersString);

bool checkForOverriddenParameters(const Parameter& param1, const Parameter& param2);



#endif // RHOMBUS_H
