#ifndef CLASS_H
#define CLASS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>

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

    // Конструктор
    Parameter(const QString& t = QString(), const QString& n = QString(),
              bool typeConst = false, bool ptrConst = false, bool ref = false,
              bool ptr = false, bool arr = false, bool ptrToArr = false,
              const QList<int>& dims = {})
        : type(t), name(n), isTypeConst(typeConst), isPointerConst(ptrConst),
        isReference(ref), isPointer(ptr), isArray(arr),
        isPointerToArray(ptrToArr), arrayDimensions(dims) {}
};

struct Method {
    bool isVirtual;
    QString returnType;
    QString methodName;
    QList<Parameter> parameters;

    Method(const QString& rt = QString(), const QString& mn = QString(),
           const QList<Parameter>& params = {}, bool virt = false)
        : isVirtual(virt), returnType(rt), methodName(mn), parameters(params) {}

    QString getSignature() const {
        // Реализация
        return QString();
    }
};

class Class {
public:
    QString className;
    QList<QString> directAncestors;
    QList<Method*> methods;


    // Конструктор
    Class(const QString& name = QString(),
          const QStringList& ancestors = {},
          const QList<Method*>& m = {})
        : className(name), directAncestors(ancestors), methods(m) {}

    // Деструктор
    ~Class() {
        for (Method* method : methods) {
            delete method;
        }
    }


};

#endif // CLASS_H
