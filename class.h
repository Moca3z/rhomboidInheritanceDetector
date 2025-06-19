/*!
* \file
* \brief Заголовочный файл класса class.
*/
#ifndef CLASS_H
#define CLASS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>

/*!
 * \brief Структура параметра
 */
struct Parameter {
    QString type;             // Базовый тип (например, "int")
    QString name;             // Имя параметра
    bool isTypeConst;         // Является ли базовый тип константным?
    bool isPointerConst;      // Является ли указатель/указатель на массив константным?
    bool isReference;         // Является ли ссылкой?
    bool isPointer;           // Является ли указателем?
    bool isArray;             // Является ли массивом?
    bool isPointerToArray;    // Является ли указателем на массив?
    QList<int> arrayDimensions; // Размеры массива

    /*!
    * \brief Конструктор с параметрами для Parameter
    */
    Parameter(const QString& t = QString(), const QString& n = QString(),
              bool typeConst = false, bool ptrConst = false, bool ref = false,
              bool ptr = false, bool arr = false, bool ptrToArr = false,
              const QList<int>& dims = {})
        : type(t), name(n), isTypeConst(typeConst), isPointerConst(ptrConst),
        isReference(ref), isPointer(ptr), isArray(arr),
        isPointerToArray(ptrToArr), arrayDimensions(dims) {}
};

/*!
 * \brief Структура метода
 */
struct Method {
    bool isVirtual;
    QString returnType;
    QString methodName;
    QList<Parameter> parameters;

    /*!
    * \brief Конструктор с параметрами для Method
    */
    Method(const QString& rt = QString(), const QString& mn = QString(),
           const QList<Parameter>& params = {}, bool virt = false)
        : isVirtual(virt), returnType(rt), methodName(mn), parameters(params) {}

};

/*!
 * \brief Класс для хранения информации о классе.
 */
class Class {
public:
    QString className;
    QList<QString> directAncestors;
    QList<Method*> methods;


    /*!
    * \brief Конструктор с параметрами для Class
    */
    Class(const QString& name = QString(),
          const QStringList& ancestors = {},
          const QList<Method*>& m = {})
        : className(name), directAncestors(ancestors), methods(m) {}

    /*!
    * \brief Деструктор для Class
    */
    ~Class() {
        for (Method* method : methods) {
            delete method;
        }
    }


};

#endif // CLASS_H
