/*!
* \file
* \brief Заголовочный файл класса classDiagram.
*/
#ifndef CLASSDIAGRAM_H
#define CLASSDIAGRAM_H

#include <QMap>
#include <QString>
#include "class.h"
#include "rhombus.h"

/*!
 * \brief Класс для хранения информации о classDiagram.
 */
class ClassDiagram {
public:

    /*!
     * \brief Конструктор для набора классов
     */
    ClassDiagram(QMap<QString, Class*>& inputClasses);

    /*!
     * \brief Конструктор для объекта Rhombus
     */
    ClassDiagram(const Rhombus& rhombus, const QMap<QString, Class*>& classes);

    /*!
     * \brief Функция для генерации DOT файла
     */
    QString generateDotFile() const;

private:
    QMap<QString, Class*> classes; // Контейнер классов диаграммы

    /*!
     * \brief Вспомогательный метод для форматирования узла класса
     */
    QString formatClassNode(const Class* cls) const;
};

#endif // CLASSDIAGRAM_H
