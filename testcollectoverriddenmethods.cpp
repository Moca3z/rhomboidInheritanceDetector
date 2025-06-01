#include "testcollectoverriddenmethods.h"
#include "collectoverriddenmethods.h"
#include "class.h"

using InheritanceMatrix = QMap<QString, QSet<QString>>;
using ClassMap = QMap<QString, Class*>;
using MethodMap = QMap<QString, QList<Method*>>;

void TestCollectOverridden::testCollectOverridden_data()
{
    QTest::addColumn<InheritanceMatrix>("inheritanceMatrix");
    QTest::addColumn<ClassMap>("classes");
    QTest::addColumn<QString>("bottomClass");
    QTest::addColumn<QString>("topClass");
    QTest::addColumn<QString>("mergePoint");
    QTest::addColumn<QList<QString>>("intermediates");
    QTest::addColumn<MethodMap>("expectedResult");

    // Тест 1: переопределения нет ни в одном потомке top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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

        MethodMap expectedResult;

        QTest::newRow("NoOverriddenMethods") << inheritanceMatrix << classes << "A" << "B" << "Null" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Тест 2: переопределение есть только в классе bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        Method* methodD = new Method();
        methodD->returnType = "Void";
        methodD->methodName = "method";
        methodD->parameters = {p1};
        d->methods.append(methodD);

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInBottomOnly") << inheritanceMatrix << classes << "D" << "A" << "Null" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Тест 3: переопределение есть в одном из промежуточных классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Method* methodB = new Method();
        methodB->returnType = "Void";
        methodB->methodName = "method";
        methodB->parameters = {p1};
        b->methods.append(methodB);

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

        MethodMap expectedResult;
        Method* expectedMethod = new Method();
        expectedMethod->returnType = "Void";
        expectedMethod->methodName = "method";
        expectedMethod->parameters = {p1};
        expectedResult["B"] = {expectedMethod};

        QTest::newRow("OverriddenInOneIntermediate") << inheritanceMatrix << classes << "D" << "A" << "Null" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Тест 4: переопределение нескольких методов в одном классе
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Method* method1 = new Method();
        method1->returnType = "Void";
        method1->methodName = "method1";
        Parameter p1;
        p1.type = "int";
        p1.name = "count";
        p1.isConst = false;
        p1.isReference = false;
        p1.isPointer = false;
        method1->parameters = {p1};
        Method* method2 = new Method();
        method2->returnType = "Void";
        method2->methodName = "method2";
        method2->parameters = {p1};
        Method* method3 = new Method();
        method3->returnType = "Void";
        method3->methodName = "method3";
        method3->parameters = {p1};
        a->methods.append(method1);
        a->methods.append(method2);
        a->methods.append(method3);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Method* method1B = new Method();
        method1B->returnType = "Void";
        method1B->methodName = "method1";
        method1B->parameters = {p1};
        Method* method3B = new Method();
        method3B->returnType = "Void";
        method3B->methodName = "method3";
        method3B->parameters = {p1};
        b->methods.append(method1B);
        b->methods.append(method3B);

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* method2C = new Method();
        method2C->returnType = "Void";
        method2C->methodName = "method2";
        method2C->parameters = {p1};
        c->methods.append(method2C);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;
        Method* expectedMethod1 = new Method();
        expectedMethod1->returnType = "Void";
        expectedMethod1->methodName = "method1";
        expectedMethod1->parameters = {p1};
        Method* expectedMethod3 = new Method();
        expectedMethod3->returnType = "Void";
        expectedMethod3->methodName = "method3";
        expectedMethod3->parameters = {p1};
        Method* expectedMethod2 = new Method();
        expectedMethod2->returnType = "Void";
        expectedMethod2->methodName = "method2";
        expectedMethod2->parameters = {p1};
        expectedResult["B"] = {expectedMethod1, expectedMethod3};
        expectedResult["C"] = {expectedMethod2};

        QTest::newRow("MultipleMethodsOverriddenInOneClass") << inheritanceMatrix << classes << "D" << "A" << "Null" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Тест 5: переопределение происходит и в промежуточном классе, и в bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Method* methodB = new Method();
        methodB->returnType = "Void";
        methodB->methodName = "method";
        methodB->parameters = {p1};
        b->methods.append(methodB);

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";
        Method* methodD = new Method();
        methodD->returnType = "Void";
        methodD->methodName = "method";
        methodD->parameters = {p1};
        d->methods.append(methodD);

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;
        Method* expectedMethod = new Method();
        expectedMethod->returnType = "Void";
        expectedMethod->methodName = "method";
        expectedMethod->parameters = {p1};
        expectedResult["B"] = {expectedMethod};

        QTest::newRow("OverriddenInIntermediateAndBottom") << inheritanceMatrix << classes << "D" << "A" << "Null" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Тест 6: отсутствие intermediates
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A";

        classes.insert("A", a);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("NoIntermediates") << inheritanceMatrix << classes << "D" << "A" << "Null" << QList<QString>{} << expectedResult;
    }

    // Тест 7: переопределение в прямой цепи наследования (не в первой точке слияния)
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"C", "B", "A"}},
            {"C", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Method* methodB = new Method();
        methodB->returnType = "Void";
        methodB->methodName = "method";
        methodB->parameters = {p1};
        b->methods.append(methodB);

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "C";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInDirectChainNotMergePoint") << inheritanceMatrix << classes << "D" << "A" << "C" << QList<QString>{"C", "B"} << expectedResult;
    }

    // Тест 8: переопределение в прямой цепи наследования (в первой точке слияния)
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"C", "B", "A"}},
            {"C", {"A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Method* methodC = new Method();
        methodC->returnType = "Void";
        methodC->methodName = "method";
        methodC->parameters = {p1};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "C";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInDirectChainInMergePoint") << inheritanceMatrix << classes << "D" << "A" << "C" << QList<QString>{"C", "B"} << expectedResult;
    }

    // Тест 9: переопределение в точке слияния, являющейся прямым предком bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A"}},
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        Method* methodD = new Method();
        methodD->returnType = "Void";
        methodD->methodName = "method";
        methodD->parameters = {p1};
        d->methods.append(methodD);

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInMergePointDirectAncestor") << inheritanceMatrix << classes << "E" << "A" << "D" << QList<QString>{"D", "B", "C"} << expectedResult;
    }

    // Тест 10: переопределение в предках точки слияния, являющейся прямым предком bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A"}},
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* methodC = new Method();
        methodC->returnType = "Void";
        methodC->methodName = "method";
        methodC->parameters = {p1};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInAncestorsOfMergePoint") << inheritanceMatrix << classes << "E" << "A" << "D" << QList<QString>{"D", "B", "C"} << expectedResult;
    }

    // Тест 11: переопределение в точке слияния, являющейся прямым потомком top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Method* methodB = new Method();
        methodB->returnType = "Void";
        methodB->methodName = "method";
        methodB->parameters = {p1};
        b->methods.append(methodB);

        Class* c = new Class();
        c->className = "C";
        c->directAncestors = {"B"};

        Class* d = new Class();
        d->className = "D";
        d->directAncestors = {"B"};

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"C", "D"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInMergePointDirectDescendant") << inheritanceMatrix << classes << "E" << "A" << "B" << QList<QString>{"C", "D", "B"} << expectedResult;
    }

    // Тест 12: переопределение потомком точки слияния, являющейся прямым потомком top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Method* methodC = new Method();
        methodC->returnType = "Void";
        methodC->methodName = "method";
        methodC->parameters = {p1};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        MethodMap expectedResult;
        Method* expectedMethod = new Method();
        expectedMethod->returnType = "Void";
        expectedMethod->methodName = "method";
        expectedMethod->parameters = {p1};
        expectedResult["C"] = {expectedMethod};

        QTest::newRow("OverriddenInDescendantOfMergePoint") << inheritanceMatrix << classes << "E" << "A" << "B" << QList<QString>{"C", "D", "B"} << expectedResult;
    }

    // Тест 13: переопределение в двух точках слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"E", "C", "D", "B", "A"}},
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Method* methodB = new Method();
        methodB->returnType = "Void";
        methodB->methodName = "method";
        methodB->parameters = {p1};
        b->methods.append(methodB);

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D";
        Method* methodE = new Method();
        methodE->returnType = "Void";
        methodE->methodName = "method";
        methodE->parameters = {p1};
        e->methods.append(methodE);

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "E";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);

        MethodMap expectedResult;

        QTest::newRow("OverriddenInTwoMergePoints") << inheritanceMatrix << classes << "F" << "A" << "E" << QList<QString>{"E", "C", "D", "B"} << expectedResult;
    }

    // Тест 14: переопределение в потомках обеих точек слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"H", "G", "E", "C", "D", "B", "A"}},
            {"H", {"E", "C", "D", "B", "A"}},
            {"G", {"E", "C", "D", "B", "A"}},
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Method* methodC = new Method();
        methodC->returnType = "Void";
        methodC->methodName = "method";
        methodC->parameters = {p1};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"C", "D"};

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "E";
        Method* methodH = new Method();
        methodH->returnType = "Void";
        methodH->methodName = "method";
        methodH->parameters = {p1};
        h->methods.append(methodH);

        Class* f = new Class();
        f->className = "F";
        f->directAncestors = {"H", "G"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("F", f);

        MethodMap expectedResult;
        Method* expected = new Method();
        expected->returnType = "Void";
        expected->methodName = "method";
        expected->parameters = {p1};
        expectedResult["H"] = {expected};

        QTest::newRow("OverriddenInDescendantsOfTwoMergePoints") << inheritanceMatrix << classes << "F" << "A" << "E" << QList<QString>{"H", "G", "E", "C", "D", "B"} << expectedResult;
    }

    // Тест 15: переопределение в одной из потенциальных точек слияния, которую можно пройти в обход
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"H", "G", "E", "C", "D", "B", "A"}},
            {"H", {"E", "C", "D", "B", "A"}},
            {"G", {"E", "C", "D", "B", "A"}},
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"C", "D"};
        Method* methodE = new Method();
        methodE->returnType = "Void";
        methodE->methodName = "method";
        methodE->parameters = {p1};
        e->methods.append(methodE);

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "E";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors = {"B", "H", "G"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("G", g);
        classes.insert("H", h);
        MethodMap expectedResult;
        Method* expected = new Method();
        expected->returnType = "Void";
        expected->methodName = "method";
        expected->parameters = {p1};
        expectedResult["E"] = {expected};

        QTest::newRow("OverriddenInBypassableMergePoint") << inheritanceMatrix << classes << "F" << "A" << "B" << QList<QString>{"B", "H", "G", "E", "C", "D"} << expectedResult;
    }

    // Тест 16: переопределение и в потомках и в предках потенциальной точки слияния, которую можно пройти в обход
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"H", "G", "E", "C", "D", "B", "A"}},
            {"H", {"E", "C", "D", "B", "A"}},
            {"G", {"E", "C", "D", "B", "A"}},
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Method* methodC = new Method();
        methodC->returnType = "Void";
        methodC->methodName = "method";
        methodC->parameters = {p1};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"C", "D"};

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "E";
        Method* methodH = new Method();
        methodH->returnType = "Void";
        methodH->methodName = "method";
        methodH->parameters = {p1};
        h->methods.append(methodH);

        Class* f = new Class();
        f->className = "F";
        f->directAncestors = {"B", "H", "G"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("F", f);

        MethodMap expectedResult;
        Method* expectedMethodC = new Method();
        expectedMethodC->returnType = "Void";
        expectedMethodC->methodName = "method";
        expectedMethodC->parameters = {p1};
        Method* expectedMethodH = new Method();
        expectedMethodH->returnType = "Void";
        expectedMethodH->methodName = "method";
        expectedMethodH->parameters = {p1};
        expectedResult["C"] = {expectedMethodC};
        expectedResult["H"] = {expectedMethodH};

        QTest::newRow("OverriddenInAncestorsAndDescendantsOfBypassableMergePoint") << inheritanceMatrix << classes << "F" << "A" << "B" << QList<QString>{"B", "H", "G", "E", "C", "D"} << expectedResult;
    }

    // Тест 17: переопределение в одном из двух ромбов, связанных друг с другом
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"D", "E", "B", "C", "A"}},
            {"D", {"B", "A"}},
            {"E", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
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
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* methodC = new Method();
        methodC->returnType = "Void";
        methodC->methodName = "method";
        methodC->parameters = {p1};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"B", "C"};

        Class* f = new Class();
        f->className = "F";
        f->directAncestors = {"D", "E"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);

        MethodMap expectedResult;
        Method* expectedMethod = new Method();
        expectedMethod->returnType = "Void";
        expectedMethod->methodName = "method";
        expectedMethod->parameters = {p1};
        expectedResult["C"] = {expectedMethod};

        QTest::newRow("OverriddenInOneOfTwoLinkedRhombuses") << inheritanceMatrix << classes << "F" << "A" << "Null" << QList<QString>{"D", "E", "B", "C"} << expectedResult;
    }

    // Тест 18: переопределеление различных методов в нескольких промежуточных классах
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"D", "E", "G", "H", "B", "C", "K", "I", "A"}},
            {"D", {"B", "A"}},
            {"E", {"B", "C", "A"}},
            {"G", {"I", "A"}},
            {"H", {"I", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"K", {"A"}},
            {"I", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Method* method1 = new Method();
        method1->returnType = "Void";
        method1->methodName = "method1";
        Parameter p1;
        p1.type = "int";
        p1.name = "count";
        p1.isConst = false;
        p1.isReference = false;
        p1.isPointer = false;
        method1->parameters = {p1};
        Method* method2 = new Method();
        method2->returnType = "Void";
        method2->methodName = "method2";
        method2->parameters = {p1};
        Method* method3 = new Method();
        method3->returnType = "Void";
        method3->methodName = "method3";
        method3->parameters = {p1};
        a->methods.append(method1);
        a->methods.append(method2);
        a->methods.append(method3);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* method2C = new Method();
        method2C->returnType = "Void";
        method2C->methodName = "method2";
        method2C->parameters = {p1};
        c->methods.append(method2C);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";
        Method* method1D = new Method();
        method1D->returnType = "Void";
        method1D->methodName = "method1";
        method1D->parameters = {p1};
        d->methods.append(method1D);

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"B", "C"};

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "I";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "I";

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "A";

        Class* k = new Class();
        k->className = "K";
        k->directAncestors << "A";
        Method* method3K = new Method();
        method3K->returnType = "Void";
        method3K->methodName = "method3";
        method3K->parameters = {p1};

        Class* f = new Class();
        f->className = "F";
        f->directAncestors = {"D", "E", "K", "G", "H"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("I", i);
        classes.insert("K", k);
        classes.insert("F", f);

        MethodMap expectedResult;
        Method* expectedMethod1 = new Method();
        expectedMethod1->returnType = "Void";
        expectedMethod1->methodName = "method1";
        expectedMethod1->parameters = {p1};
        Method* expectedMethod2 = new Method();
        expectedMethod2->returnType = "Void";
        expectedMethod2->methodName = "method2";
        expectedMethod2->parameters = {p1};
        Method* expectedMethod3 = new Method();
        expectedMethod3->returnType = "Void";
        expectedMethod3->methodName = "method3";
        expectedMethod3->parameters = {p1};
        expectedResult["D"] = {expectedMethod1};
        expectedResult["C"] = {expectedMethod2};
        expectedResult["K"] = {expectedMethod3};

        QTest::newRow("MultipleMethodsInMultipleIntermediates") << inheritanceMatrix << classes << "F" << "A" << "Null" << QList<QString>{"D", "E", "K", "G", "H", "B", "C", "I"} << expectedResult;
    }

    // Тест 19: переопределение различных методов в цепочке наследования
    {
        InheritanceMatrix inheritanceMatrix = {
            {"R", {"P", "O", "K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A"}},
            {"P", {"O", "K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A"}},
            {"O", {"K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A"}},
            {"K", {"I", "J", "H", "F", "G", "E", "C", "D", "B", "A"}},
            {"I", {"H", "F", "G", "E", "C", "D", "B", "A"}},
            {"J", {"H", "F", "G", "E", "C", "D", "B", "A"}},
            {"H", {"F", "G", "E", "C", "D", "B", "A"}},
            {"F", {"E", "C", "D", "B", "A"}},
            {"G", {"E", "C", "D", "B", "A"}},
            {"E", {"C", "D", "B", "A"}},
            {"C", {"B", "A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Method* method1 = new Method();
        method1->returnType = "Void";
        method1->methodName = "method1";
        Parameter p1;
        p1.type = "int";
        p1.name = "count";
        p1.isConst = false;
        p1.isReference = false;
        p1.isPointer = false;
        method1->parameters = {p1};
        Method* method2 = new Method();
        method2->returnType = "Void";
        method2->methodName = "method2";
        method2->parameters = {p1};
        Method* method3 = new Method();
        method3->returnType = "Void";
        method3->methodName = "method3";
        method3->parameters = {p1};
        Method* method4 = new Method();
        method4->returnType = "Void";
        method4->methodName = "method4";
        method4->parameters = {p1};
        a->methods.append(method1);
        a->methods.append(method2);
        a->methods.append(method3);
        a->methods.append(method4);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Method* method2C = new Method();
        method2C->returnType = "Void";
        method2C->methodName = "method2";
        method2C->parameters = {p1};
        c->methods.append(method2C);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors = {"C", "D"};

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "E";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";
        Method* method1G = new Method();
        method1G->returnType = "Void";
        method1G->methodName = "method1";
        method1G->parameters = {p1};
        g->methods.append(method1G);

        Class* h = new Class();
        h->className = "H";
        h->directAncestors = {"F", "G"};

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "H";

        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "H";

        Class* k = new Class();
        k->className = "K";
        k->directAncestors = {"I", "J"};
        Method* method3K = new Method();
        method3K->returnType = "Void";
        method3K->methodName = "method3";
        method3K->parameters = {p1};
        k->methods.append(method3K);

        Class* o = new Class();
        o->className = "O";
        o->directAncestors << "K";
        Method* method4O = new Method();
        method4O->returnType = "Void";
        method4O->methodName = "method4";
        method4O->parameters = {p1};
        o->methods.append(method4O);

        Class* p = new Class();
        p->className = "P";
        p->directAncestors << "O";

        Class* r = new Class();
        r->className = "R";
        r->directAncestors << "P";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("I", i);
        classes.insert("J", j);
        classes.insert("K", k);
        classes.insert("O", o);
        classes.insert("P", p);
        classes.insert("R", r);

        MethodMap expectedResult;
        Method* expectedMethod4 = new Method();
        expectedMethod4->returnType = "Void";
        expectedMethod4->methodName = "method4";
        expectedMethod4->parameters = {p1};
        expectedResult["O"] = {expectedMethod4};

        QTest::newRow("MultipleMethodsInInheritanceChain") << inheritanceMatrix << classes << "R" << "A" << "K" << QList<QString>{"O", "P", "K", "I", "J", "H", "G", "F", "E", "C", "D", "B"} << expectedResult;
    }

    // Тест 20: перегрузка метода top по возвращаемому значению
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Method* method = new Method();
        method->returnType = "int";
        method->methodName = "method";
        Parameter p1;
        p1.type = "int";
        p1.name = "a";
        p1.isConst = false;
        p1.isReference = false;
        p1.isPointer = false;
        Parameter p2;
        p2.type = "int";
        p2.name = "b";
        p2.isConst = false;
        p2.isReference = false;
        p2.isPointer = false;
        method->parameters = {p1, p2};
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* methodC = new Method();
        methodC->returnType = "float";
        methodC->methodName = "method";
        methodC->parameters = {p1, p2};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors = {"B", "C"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("OverloadByReturnValue") << inheritanceMatrix << classes << "D" << "A" << "" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Тест 21: перегрузка метода top по входному параметру
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Method* method = new Method();
        method->returnType = "int";
        method->methodName = "method";
        Parameter p1;
        p1.type = "int";
        p1.name = "a";
        p1.isConst = false;
        p1.isReference = false;
        p1.isPointer = false;
        Parameter p2;
        p2.type = "int";
        p2.name = "b";
        p2.isConst = false;
        p2.isReference = false;
        p2.isPointer = false;
        method->parameters = {p1, p2};
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* methodC = new Method();
        methodC->returnType = "int";
        methodC->methodName = "method";
        Parameter p2Overload;
        p2Overload.type = "float";
        p2Overload.name = "b";
        p2Overload.isConst = false;
        p2Overload.isReference = false;
        p2Overload.isPointer = false;
        methodC->parameters = {p1, p2Overload};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors = {"B", "C"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("OverloadByInputParameter") << inheritanceMatrix << classes << "D" << "A" << "" << QList<QString>{"B", "C"} << expectedResult;
    }

    // Test 22: Overload by new parameter
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Method* method = new Method();
        method->returnType = "int";
        method->methodName = "method";
        Parameter p1;
        p1.type = "int";
        p1.name = "a";
        p1.isConst = false;
        p1.isReference = false;
        p1.isPointer = false;
        Parameter p2;
        p2.type = "int";
        p2.name = "b";
        p2.isConst = false;
        p2.isReference = false;
        p2.isPointer = false;
        method->parameters = {p1, p2};
        a->methods.append(method);

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Method* methodC = new Method();
        methodC->returnType = "int";
        methodC->methodName = "method";
        Parameter p3;
        p3.type = "int";
        p3.name = "c";
        p3.isConst = false;
        p3.isReference = false;
        p3.isPointer = false;
        methodC->parameters = {p1, p2, p3};
        c->methods.append(methodC);

        Class* d = new Class();
        d->className = "D";
        d->directAncestors = {"B", "C"};

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        MethodMap expectedResult;

        QTest::newRow("OverloadByNewParameter") << inheritanceMatrix << classes << "D" << "A" << "" << QList<QString>{"B", "C"} << expectedResult;
    }
}

void TestCollectOverridden::testCollectOverridden()
{
    QFETCH(InheritanceMatrix, inheritanceMatrix);
    QFETCH(ClassMap, classes);
    QFETCH(QString, bottomClass);
    QFETCH(QString, topClass);
    QFETCH(QString, mergePoint);
    QFETCH(QList<QString>, intermediates);
    QFETCH(MethodMap, expectedResult);

    MethodMap result = collectOverriddenMethods(bottomClass, topClass, mergePoint, intermediates, classes, inheritanceMatrix);

    // Проверка размера массива
    QVERIFY2(result.size() == expectedResult.size(), qPrintable(QString("Expected result size %1, but got %2").arg(expectedResult.size()).arg(result.size())));

    // Проверка ключей
    for (const QString& key : expectedResult.keys()) {
        QVERIFY2(result.contains(key), qPrintable(QString("Result does not contain expected key: %1").arg(key)));
        QVERIFY2(result[key].size() == expectedResult[key].size(), qPrintable(QString("For key %1, expected %2 methods, but got %3").arg(key).arg(expectedResult[key].size()).arg(result[key].size())));

        // Проверка названий методов
        for (int i = 0; i < expectedResult[key].size(); ++i) {
            QVERIFY2(result[key][i]->methodName == expectedResult[key][i]->methodName,
                     qPrintable(QString("For key %1, expected method %2, but got %3").arg(key).arg(expectedResult[key][i]->methodName).arg(result[key][i]->methodName)));
        }
    }

    // Очистка памяти
    for (Class* cls : classes) {
        for (Method* method : cls->methods) {
            delete method;
        }
        delete cls;
    }
    for (const QString& key : expectedResult.keys()) {
        for (Method* method : expectedResult[key]) {
            delete method;
        }
    }
}
