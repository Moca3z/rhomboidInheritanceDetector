#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"
#include <QDebug>

struct Rhombus {
    QString bottom;
    QString top;
    QList<QString> intermediates;
    QMap<QString, QList<Method*>> overriddenMethods;

    // Полное сравнение всех полей, включая методы
    bool operator==(const Rhombus& other) const {
        // 1. Сравнение базовых полей
        if (bottom != other.bottom || top != other.top || intermediates != other.intermediates) {
            return false;
        }

        // 2. Сравнение переопределенных методов
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
                        p1.isConst != p2.isConst ||
                        p1.isReference != p2.isReference ||
                        p1.isPointer != p2.isPointer) {
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

// Функция хеширования
inline uint qHash(const Rhombus& rhombus, uint seed = 0) {
    uint hash = qHash(rhombus.bottom, seed) ^ qHash(rhombus.top, seed);

    // Хеширование intermediates
    for (const QString& item : rhombus.intermediates) {
        hash ^= qHash(item);
    }

    // Хеширование overriddenMethods
    for (auto it = rhombus.overriddenMethods.constBegin(); it != rhombus.overriddenMethods.constEnd(); ++it) {
        hash ^= qHash(it.key());
        for (const Method* method : it.value()) {
            hash ^= qHash(method->methodName);
            hash ^= qHash(method->returnType);
            hash ^= method->isVirtual;

            for (const Parameter& param : method->parameters) {
                hash ^= qHash(param.type);
                hash ^= qHash(param.name);
                hash ^= param.isConst;
                hash ^= param.isReference;
                hash ^= param.isPointer;
            }
        }
    }

    return hash;
}


#endif // RHOMBUS_H
