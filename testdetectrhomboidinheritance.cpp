#include "testdetectrhomboidinheritance.h"
#include "detectrhomboidinheritance.h"
#include "class.h"

void TestDetectRhomboid::testNoRhombus() {

    Method* method = new Method();
    method->returnType = "Void";
    method->methodName = "method";

    Parameter p1;
    p1.type = "int";
    p1.name = "count";
    p1.isConst = false;
    p1.isReference = false;
    p1.isPointer = false;

    method->parameters = {p1};


    QMap<QString, Class*> classes;
    Class* a = new Class();
    a->className = "A";
    a->methods.append(method);


    Class* b = new Class();
    b->className = "B";
    b->directAncestors << "A";

    Class* c = new Class();
    c->className = "C";
    c->directAncestors << "A";

    Class* d = new Class();
    d->className = "D";
    d->directAncestors << "B" << "C";

    classes.insert("A", a);
    classes.insert("B", b);
    classes.insert("C", c);
    classes.insert("D", d);


    QSet<Rhombus> result = detectRhomboidInheritance(classes);
    QVERIFY(result.isEmpty()); // Ожидаем пустой результат

    qDeleteAll(classes);


}
