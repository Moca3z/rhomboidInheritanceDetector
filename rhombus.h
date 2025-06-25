/*!
* \file
* \brief Заголовочный файл класса Rhombus и вспомогательных функций.
*/
#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>
#include "class.h"
#include <QDebug>
#include <QMultiMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QQueue>
#include <QDomDocument>
#include "error.h"

/*!
 * \brief Структура объекта Rhombus
 */
struct Rhombus {
    QString bottom;
    QString top;
    QMultiMap<int, QString> intermediates;
    QMap<QString, QSet<Method*>> overriddenMethods;

    Rhombus(const QString& b = QString(), const QString& t = QString(),
            const QMultiMap<int, QString>& i = QMultiMap<int, QString>(),
            const QMap<QString, QSet<Method*>>& m = QMap<QString, QSet<Method*>>())
        : bottom(b), top(t), intermediates(i), overriddenMethods(m) {}

    /*!
    * \brief Перегрузка оператора == для Rhombus
    */
    // Полное сравнение всех полей, включая методы
    bool operator==(const Rhombus& other) const {
        // 1. Сравнение базовых полей
        if (bottom != other.bottom || top != other.top) {
            return false;
        }

        // 2. Сравнение intermediates с игнорированием порядка
        // Получаем уникальные ключи
        QList<int> resultKeys = intermediates.uniqueKeys();
        QList<int> expectedKeys = other.intermediates.uniqueKeys();

        // Сравниваем наборы ключей
        if (resultKeys != expectedKeys) {
            return false;
        }

        // Для каждого ключа сравниваем отсортированные списки значений
        for (int key : resultKeys) {
            QList<QString> resultValues = intermediates.values(key);
            QList<QString> expectedValues = other.intermediates.values(key);

            // Сортируем списки для корректного сравнения
            std::sort(resultValues.begin(), resultValues.end());
            std::sort(expectedValues.begin(), expectedValues.end());

            if (resultValues != expectedValues) {
                return false;
            }
        }

        // 3. Сравнение переопределённых методов
        if (overriddenMethods.size() != other.overriddenMethods.size()) {
            return false;
        }

        // 4. Поэлементное сравнение методов
        if (overriddenMethods.size() != other.overriddenMethods.size()) {
            return false;
        }

        for (auto it = overriddenMethods.constBegin(); it != overriddenMethods.constEnd(); ++it) {
            const QString& className = it.key();
            if (!other.overriddenMethods.contains(className)) {
                return false;
            }

            const QSet<Method*>& methods1 = it.value();
            const QSet<Method*>& methods2 = other.overriddenMethods[className];

            if (methods1.size() != methods2.size()) {
                return false;
            }

            // Сравнение содержимого множеств
            for (Method* m1 : methods1) {
                bool found = false;
                for (Method* m2 : methods2) {
                    if (*m1 == *m2) {  // Требуется operator== для Method
                        found = true;
                        break;
                    }
                }
                if (!found) return false;
            }
        }
        return true;
    }
};

/*!
 * \brief Функция хеширования для QMultiMap<int, QString> с игнорированием порядка
 */
inline size_t qHash(const QMultiMap<int, QString>& map, size_t seed = 0) {
    // Получаем уникальные ключи и сортируем их
    QList<int> keys = map.uniqueKeys();
    std::sort(keys.begin(), keys.end());

    // Для каждого ключа получаем отсортированные значения
    for (int key : keys) {
        QList<QString> values = map.values(key);
        std::sort(values.begin(), values.end());

        // Хешируем ключ и отсортированные значения
        seed = qHash(key, seed);
        for (const QString& value : values) {
            seed = qHash(value, seed);
        }
    }
    return seed;
}

/*!
 * \brief Функция хеширования для Rhombus
 */
inline size_t qHash(const Rhombus& rhombus, size_t seed = 0) {
    seed = qHash(rhombus.bottom, seed);
    seed = qHash(rhombus.top, seed);
    seed = qHash(rhombus.intermediates, seed); // Теперь работает с учетом порядка, игнорируя исходный порядок

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


/*!
 * \brief Разбирает строку параметров метода
 * \param [in] parametersString - строка параметров, разделённых запятыми
 * \return список структур Parameter, содержащих информацию о каждом параметре
 */
QList<Parameter> parseParameters(const QString& parametersString);

/*!
 * \brief Проверяет, является ли один параметр переопределением другого
 * \param [in] param1 - первый параметр для сравнения
 * \param [in] param2 - второй параметр для сравнения
 * \return true, если param2 является переопределением param1, false в противном случае
 */
bool checkForOverriddenParameters(const Parameter& param1, const Parameter& param2);

/*!
 * \brief Создает матрицу наследования из набора классов
 * \param [in] classes - набор классов
 * \return контейнер, где для каждого класса собраны все его предки
 */
QMap<QString, QSet<QString>> buildInheritanceMatrix(const QMap<QString, Class*>& classes);

/*!
 * \brief Разбирает поступающий на вход xml - файл
 * \param [in] filename - название xml файла
 * \return контейнер, хранящий набор классов и список ошибок, если они были найдены
 */
QPair<QMap<QString, Class*>, QList<Error>> parseXmlFile(const QString& filename);

#endif // RHOMBUS_H
