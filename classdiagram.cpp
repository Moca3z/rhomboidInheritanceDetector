#include "classdiagram.h"
#include <QStringBuilder>
#include <QTextStream>

ClassDiagram::ClassDiagram(QMap<QString, Class*>& inputClasses) {
    for (auto it = inputClasses.begin(); it != inputClasses.end(); ++it) {
        Class* simplifiedClass = new Class(it.key());
        simplifiedClass->directAncestors = it.value()->directAncestors;
        classes.insert(it.key(), simplifiedClass);
    }
}

ClassDiagram::ClassDiagram(const Rhombus& rhombus, const QMap<QString, Class*>& allClasses) {
    // 1. Собираем все переопределенные методы
    QMap<QString, Method*> allOverriddenMethods;
    for (auto it = rhombus.overriddenMethods.constBegin(); it != rhombus.overriddenMethods.constEnd(); ++it) {
        for (Method* method : it.value()) {
            allOverriddenMethods.insert(method->methodName, method);
        }
    }

    // 2. Добавляем промежуточные классы
    for (const QString& className : rhombus.intermediates) {
        Class* cls = new Class(className);

        // Фильтруем предков - только те, что в ромбе
        if (allClasses.contains(className)) {
            for (const QString& ancestor : allClasses[className]->directAncestors) {
                // Прямая проверка принадлежности к ромбу
                if (ancestor == rhombus.top ||
                    ancestor == rhombus.bottom ||
                    rhombus.intermediates.values().contains(ancestor)) {
                    cls->directAncestors.append(ancestor);
                }
            }
        }

        // Добавляем переопределенные методы
        if (rhombus.overriddenMethods.contains(className)) {
            for (Method* method : rhombus.overriddenMethods[className]) {
                cls->methods.append(new Method(*method));
            }
        }

        classes.insert(className, cls);
    }

    // 3. Добавляем основание ромба (bottom)
    Class* bottomClass = new Class(rhombus.bottom);
    if (allClasses.contains(rhombus.bottom)) {
        for (const QString& ancestor : allClasses[rhombus.bottom]->directAncestors) {
            // Прямая проверка принадлежности к ромбу
            if (ancestor == rhombus.top || rhombus.intermediates.values().contains(ancestor)) {
                bottomClass->directAncestors.append(ancestor);
            }
        }
    }

    if (rhombus.overriddenMethods.contains(rhombus.bottom)) {
        for (Method* method : rhombus.overriddenMethods[rhombus.bottom]) {
            bottomClass->methods.append(new Method(*method));
        }
    }
    classes.insert(rhombus.bottom, bottomClass);

    // 4. Добавляем вершину ромба (top) с собранными методами
    Class* topClass = new Class(rhombus.top);
    for (Method* method : allOverriddenMethods) {
        topClass->methods.append(new Method(*method));
    }
    classes.insert(rhombus.top, topClass);
}

QString ClassDiagram::generateDotFile() const {
    QString dot;
    QTextStream stream(&dot);

    stream << "digraph G {\n";
    stream << "  rankdir=BT;\n";
    stream << "  node [shape=record, fontname=\"Arial\", fontsize=10];\n\n";

    // Сначала добавляем вершину (top)
    if (classes.contains(classes.firstKey())) {
        const Class* topClass = classes[classes.firstKey()];
        stream << formatClassNode(topClass);
    }

    // Затем остальные классы
    for (auto it = classes.constBegin(); it != classes.constEnd(); ++it) {
        if (it.key() != classes.firstKey()) {
            stream << formatClassNode(it.value());
        }
    }

    // Добавляем связи наследования
    for (auto it = classes.constBegin(); it != classes.constEnd(); ++it) {
        const Class* cls = it.value();
        for (const QString& ancestor : cls->directAncestors) {
            if (classes.contains(ancestor)) {
                stream << "  \"" << cls->className << "\" -> \"" << ancestor << "\";\n";
            }
        }
    }

    stream << "}\n";
    return dot;
}

QString ClassDiagram::formatClassNode(const Class* cls) const {
    QString node;
    QTextStream stream(&node);

    if (cls->methods.isEmpty()) {
        stream << "  \"" << cls->className << "\";\n";
    } else {
        stream << "  \"" << cls->className << "\" [label=<\n";
        stream << "    <table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n";
        stream << "      <tr><td bgcolor=\"lightgray\"><b>" << cls->className << "</b></td></tr>\n";

        for (const Method* method : cls->methods) {
            stream << "      <tr><td align=\"left\">"
                   << method->returnType << " " << method->methodName << "(";

            for (int i = 0; i < method->parameters.size(); ++i) {
                const Parameter& p = method->parameters[i];

                // Константность типа
                if (p.isTypeConst) stream << "const ";

                // Основной тип
                stream << p.type;

                // Обработка указателей на массивы (особый случай)
                if (p.isPointerToArray) {
                    stream << " (*)";  // Правильный формат для указателя на массив
                    // Выводим только размерности массива (пропускаем -1 для указателя)
                    for (int j = 1; j < p.arrayDimensions.size(); ++j) {
                        stream << "[";
                        if (p.arrayDimensions[j] > 0) stream << p.arrayDimensions[j];
                        stream << "]";
                    }
                }
                // Обычные указатели
                else if (p.isPointer) {
                    stream << "*";
                    if (p.isPointerConst) stream << " const";
                }
                // Ссылки
                else if (p.isReference) {
                    stream << "&amp;";
                }

                // Обычные массивы (не указатели на массивы)
                if (p.isArray && !p.isPointerToArray) {
                    for (int dim : p.arrayDimensions) {
                        stream << "[";
                        if (dim > 0) stream << dim;
                        stream << "]";
                    }
                }

                if (i < method->parameters.size() - 1) stream << ", ";
            }

            stream << ")</td></tr>\n";
        }

        stream << "    </table>\n";
        stream << "  >];\n";
    }

    return node;
}
