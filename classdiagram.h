#ifndef CLASSDIAGRAM_H
#define CLASSDIAGRAM_H

#include <QMap>
#include <QString>
#include "class.h"
#include "rhombus.h"

class ClassDiagram {
public:
    // Конструкторы
    ClassDiagram(QMap<QString, Class*>& inputClasses);
    ClassDiagram(const Rhombus& rhombus, const QMap<QString, Class*>& classes);

    // Генерация DOT-файла
    QString generateDotFile() const;

private:
    QMap<QString, Class*> classes; // Контейнер классов диаграммы

    // Вспомогательный метод для форматирования узла класса
    QString formatClassNode(const Class* cls) const;
};

#endif // CLASSDIAGRAM_H
