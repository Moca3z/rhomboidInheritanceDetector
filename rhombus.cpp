// rhombus.cpp
#include "rhombus.h"

// Функция для разбора строки параметров с использованием локальных статических регулярных выражений
QList<Parameter> parseParameters(const QString& parametersString) {
    QList<Parameter> params;
    QStringList decls = parametersString.split(",", Qt::SkipEmptyParts);

    // Локальные статические регулярные выражения
    static const QRegularExpression standardRe(
        R"(^(?:\s*(const)\s+)?(\w+)(\s+const)?\s*(\*)?\s*(const)?\s*(&)?\s*(\w+)\s*((?:\[\s*\d*\s*\])*)?\s*$)"
        );
    static const QRegularExpression pointerToArrayRe(
        R"(^(?:\s*(const)\s+)?(\w+)\s*\(\s*\*\s*(const)?\s*(\w+)\s*\)\s*((?:\[\s*\d*\s*\])*)?\s*$)"
        );
    static const QRegularExpression dimensionRe(
        R"(\[\s*(\d*)\s*\])"
        );

    for (const QString& decl : decls) {
        QString trimmedDecl = decl.trimmed();
        if (trimmedDecl.isEmpty()) continue;

        Parameter param;
        QRegularExpressionMatch match;

        // Проверяем, является ли объявление указателем на массив
        if ((match = pointerToArrayRe.match(trimmedDecl)).hasMatch()) {
            param.isTypeConst = !match.captured(1).isEmpty(); // const перед типом
            param.type = match.captured(2); // тип
            param.isPointerConst = !match.captured(3).isEmpty(); // const перед именем
            param.name = match.captured(4); // имя
            param.isPointer = true;
            param.isPointerToArray = true;

            // Извлекаем размерности массива
            QString dimsStr = match.captured(5).trimmed();
            param.arrayDimensions << -1; // Добавляем -1 для указателя на массив
            if (!dimsStr.isEmpty()) {
                QRegularExpressionMatchIterator dimIt = dimensionRe.globalMatch(dimsStr);
                while (dimIt.hasNext()) {
                    QRegularExpressionMatch dimMatch = dimIt.next();
                    QString dim = dimMatch.captured(1);
                    param.arrayDimensions << (dim.isEmpty() ? -1 : dim.toInt());
                }
            }
        } else if ((match = standardRe.match(trimmedDecl)).hasMatch()) {
            param.isTypeConst = !match.captured(1).isEmpty() || !match.captured(3).isEmpty(); // const перед или после типа
            param.type = match.captured(2); // тип
            param.isPointer = !match.captured(4).isEmpty(); // *
            param.isPointerConst = !match.captured(5).isEmpty(); // const после *
            param.isReference = !match.captured(6).isEmpty(); // &
            param.name = match.captured(7); // имя

            // Извлекаем размерности массива
            QString dimsStr = match.captured(8).trimmed();
            if (!dimsStr.isEmpty()) {
                param.isArray = true;
                QRegularExpressionMatchIterator dimIt = dimensionRe.globalMatch(dimsStr);
                while (dimIt.hasNext()) {
                    QRegularExpressionMatch dimMatch = dimIt.next();
                    QString dim = dimMatch.captured(1);
                    param.arrayDimensions << (dim.isEmpty() ? -1 : dim.toInt());
                }
                if (param.arrayDimensions.isEmpty() && trimmedDecl.contains("[]")) {
                    param.arrayDimensions << -1;
                }
            }
        } else {
            continue; // Предполагаем корректный ввод согласно тестам
        }

        params << param;
    }

    return params;
}

bool checkForOverriddenParameters(const Parameter& param1, const Parameter& param2) {
    // Проверка совпадения базовых типов
    if (param1.type != param2.type) {
        return false;
    }

    // Проверка константности типа игнорируется для параметров по значению
    // (когда параметр не является указателем, ссылкой, массивом или указателем на массив)
    if (param1.isPointer || param1.isReference || param1.isArray || param1.isPointerToArray ||
        param2.isPointer || param2.isReference || param2.isArray || param2.isPointerToArray) {
        if (param1.isTypeConst != param2.isTypeConst) {
            return false;
        }
    }

    // Проверка совпадения константности указателя
    if (param1.isPointerConst != param2.isPointerConst) {
        return false;
    }

    // Проверка совпадения свойства ссылки
    if (param1.isReference != param2.isReference) {
        return false;
    }

    // Проверка совпадения свойства указателя
    if (param1.isPointer != param2.isPointer) {
        return false;
    }

    // Проверка совпадения свойства массива
    if (param1.isArray != param2.isArray) {
        return false;
    }

    // Проверка совпадения свойства указателя на массив
    if (param1.isPointerToArray != param2.isPointerToArray) {
        return false;
    }

    // Если параметры являются массивами, проверка совпадения размеров массива
    if (param1.isArray || param2.isArray) {
        if (param1.arrayDimensions != param2.arrayDimensions) {
            return false;
        }
    }

    // Имена параметров не влияют на переопределение
    return true;
}


// Вспомогательная функция для построения матрицы наследования
QMap<QString, QSet<QString>> buildInheritanceMatrix(const QMap<QString, Class*>& classes) {
    QMap<QString, QSet<QString>> inheritanceMatrix;

    // Инициализация матрицы для всех классов
    for (const QString& className : classes.keys()) {
        inheritanceMatrix[className] = QSet<QString>();
    }

    // Для каждого класса собираем всех его предков (прямых и косвенных)
    for (const QString& className : classes.keys()) {
        QQueue<QString> queue;
        QSet<QString> visited;

        // Начинаем с текущего класса
        queue.enqueue(className);
        visited.insert(className);

        while (!queue.isEmpty()) {
            QString current = queue.dequeue();
            Class* currentClass = classes.value(current);

            // Добавляем всех прямых предков
            for (const QString& ancestor : currentClass->directAncestors) {
                if (!visited.contains(ancestor)) {
                    queue.enqueue(ancestor);
                    visited.insert(ancestor);
                    inheritanceMatrix[className].insert(ancestor);
                }
            }
        }
    }

    return inheritanceMatrix;
}
