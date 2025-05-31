#ifndef CLASS_H
#define CLASS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QSet>

struct Parameter {
    QString type;
    QString name;
    bool isConst;
    bool isReference;
    bool isPointer;
};

struct Method {
    bool isVirtual;
    QString returnType;
    QString methodName;
    QList<Parameter> parameters;

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
};

#endif // CLASS_H
