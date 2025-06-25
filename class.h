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

    bool operator==(const Parameter& other) const {
        return type == other.type &&
               isTypeConst == other.isTypeConst &&
               isPointerConst == other.isPointerConst &&
               isReference == other.isReference &&
               isPointer == other.isPointer &&
               isArray == other.isArray &&
               isPointerToArray == other.isPointerToArray &&
               arrayDimensions == other.arrayDimensions;
    }
};

/*!
 * \brief Функция хеширования для Parameters
 */
inline uint qHash(const Parameter& param, uint seed = 0) noexcept {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, param.type);
    seed = hash(seed, param.isTypeConst);
    seed = hash(seed, param.isPointerConst);
    seed = hash(seed, param.isReference);
    seed = hash(seed, param.isPointer);
    seed = hash(seed, param.isArray);
    seed = hash(seed, param.isPointerToArray);
    for (int dim : param.arrayDimensions) {
        seed = hash(seed, dim);
    }
    return seed;
}


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

    bool operator==(const Method& other) const {
        return isVirtual == other.isVirtual &&
               returnType == other.returnType &&
               methodName == other.methodName &&
               parameters == other.parameters;
    }

};

/*!
 * \brief Функция хеширования для Method
 */
inline uint qHash(const Method& method, uint seed = 0) noexcept {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, method.isVirtual);
    seed = hash(seed, method.returnType);
    seed = hash(seed, method.methodName);
    for (const Parameter& param : method.parameters) {
        seed = hash(seed, qHash(param));
    }
    return seed;
}

/*!
 * \brief Класс для хранения информации о классе.
 */
class Class {
public:
    QString className;
    QList<QString> directAncestors;
    QSet<Method*> methods;


    /*!
    * \brief Конструктор с параметрами для Class
    */
    Class(const QString& name = QString(),
          const QStringList& ancestors = {},
          const QSet<Method*>& m = {})
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
