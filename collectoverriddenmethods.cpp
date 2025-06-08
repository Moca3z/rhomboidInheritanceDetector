#include "collectoverriddenmethods.h"

QMap<QString, QList<Method*>> collectOverriddenMethods(
    QString bottom,
    QString top,
    QString mergePoint,
    QMultiMap<int, QString> intermediates,
    QMap<QString, Class*> classes,
    QMap<QString, QSet<QString>> inheritanceMatrix
    ) {
    Q_UNUSED(bottom);
    Q_UNUSED(top);
    Q_UNUSED(mergePoint);
    Q_UNUSED(intermediates);
    Q_UNUSED(classes);
    Q_UNUSED(inheritanceMatrix);

    // Заглушка в виде неверных данных
    QMap<QString, QList<Method*>> wrongResult;

    Method* fakeMethod1 = new Method();

    fakeMethod1->methodName = "FAKE_METHOD_1";

    wrongResult["WrongClass1"] = {fakeMethod1};

    return wrongResult;
}
