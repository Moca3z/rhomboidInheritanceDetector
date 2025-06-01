#include "testdetectrhomboidinheritance.h"
#include "detectrhomboidinheritance.h"
#include "class.h"

// Добавляем алиас
using ClassMap = QMap<QString, Class*>;

void TestDetectRhomboid::testDetectRhomboidInheritance_data()
{
    QTest::addColumn<ClassMap>("classes");
    QTest::addColumn<QSet<Rhombus>>("expected");

    // Тест 1: пустой граф
    {
        ClassMap classes;
        QSet<Rhombus> expected;
        QTest::newRow("Empty Graph") << classes << expected;
    }

    // Тест 2: один класс в графе
    {
        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        classes.insert("A", a);
        QSet<Rhombus> expected;
        QTest::newRow("Single Class") << classes << expected;
    }

    // Тест 3: два класса в графе с связью наследования
    {
        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        classes.insert("A", a);
        classes.insert("B", b);
        QSet<Rhombus> expected;
        QTest::newRow("Two Classes With Inheritance") << classes << expected;
    }

    // Тест 4: две несвязанные  вершины графа
    {
        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        QSet<Rhombus> expected;
        QTest::newRow("Two Unconnected Classes") << classes << expected;
    }

    // Тест 5: один промежуточный класс
    {
        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        QSet<Rhombus> expected;
        QTest::newRow("One Intermediate Class") << classes << expected;
    }

    // Тест 6: прямая цепочка наследований
    {
        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "C";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        QSet<Rhombus> expected;
        QTest::newRow("Straight Inheritance Chain") << classes << expected;
    }

    // Тест 7: прямая цепочка наследований с переопределением метода
    {
        ClassMap classes;
        Method* method = new Method();
        method->returnType = "void";
        method->methodName = "methodA";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(method);
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        c->methods.append(method);
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "C";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        QSet<Rhombus> expected;
        QTest::newRow("Straight Chain With Method Override") << classes << expected;
    }

    // Тест 8: разветвление прямых цепочек наследования разной длины с переопределением методов
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodB = new Method();
        methodB->returnType = "void";
        methodB->methodName = "methodB";
        Method* methodC = new Method();
        methodC->returnType = "void";
        methodC->methodName = "methodC";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        Class* b = new Class();
        b->className = "B";
        b->methods.append(methodB);
        Class* c = new Class();
        c->className = "C";
        c->methods.append(methodC);
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A";
        d->methods.append(methodA);
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B";
        e->methods.append(methodB);
        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "C";
        f->methods.append(methodC);
        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "D";
        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "E";
        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "G";
        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "F" << "H" << "I";
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
        QSet<Rhombus> expected;
        QTest::newRow("Branching Chains With Method Override") << classes << expected;
    }

    // Тест 9: ромб из трех классов без переопределения методов
    {
        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A" << "B";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        QSet<Rhombus> expected;
        QTest::newRow("Rhombus Three Classes No Override") << classes << expected;
    }

    // Тест 10: ромб из трех классов с переопределением методов
    {
        ClassMap classes;
        Method* method = new Method();
        method->returnType = "void";
        method->methodName = "methodA";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(method);
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        b->methods.append(method);
        Class* d = new Class();
        d->className = "D";
        Class* g = new Class();
        g->className = "G";
        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "G";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A" << "B" << "D" << "F";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("F", f);
        classes.insert("G", g);
        Rhombus expectedRhombus;
        expectedRhombus.bottom = "C";
        expectedRhombus.top = "A";
        expectedRhombus.intermediates = QList<QString>{"B"};
        expectedRhombus.overriddenMethods.insert("B", QList<Method*>{method});
        QSet<Rhombus> expected = {expectedRhombus};
        QTest::newRow("Rhombus Three Classes With Override") << classes << expected;
    }

    // Тест 11: один ромб с более чем одним промежуточным классом и с переопределением методов в bottom ромба
    {
        ClassMap classes;
        Method* method = new Method();
        method->returnType = "void";
        method->methodName = "methodA";
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
        d->methods.append(method);
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        QSet<Rhombus> expected;
        QTest::newRow("Rhombus Multiple Intermediates Bottom Override") << classes << expected;
    }

    // Тест 12: один ромб с более чем одним промежуточным классом и с переопределением методов
    {
        ClassMap classes;
        Method* method = new Method();
        method->returnType = "void";
        method->methodName = "methodA";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(method);
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        c->methods.append(method);
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        Rhombus expectedRhombus;
        expectedRhombus.bottom = "D";
        expectedRhombus.top = "A";
        expectedRhombus.intermediates = QList<QString>{"B", "C"};
        expectedRhombus.overriddenMethods.insert("C", QList<Method*>{method});
        QSet<Rhombus> expected = {expectedRhombus};
        QTest::newRow("Rhombus Multiple Intermediates") << classes << expected;
    }

    // Тест 13: несколько ромбов с более чем одним промежуточным классом и переопределением методов
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodG = new Method();
        methodG->returnType = "void";
        methodG->methodName = "methodG";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        Class* m = new Class();
        m->className = "M";
        Class* g = new Class();
        g->className = "G";
        g->methods.append(methodG);
        Class* k = new Class();
        k->className = "K";
        Class* h = new Class();
        h->className = "H";
        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "M";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A" << "J";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        c->methods.append(methodA);
        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "G";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "G" << "K";
        e->methods.append(methodG);
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "C" << "B" << "F" << "E" << "H";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("J", j);
        classes.insert("K", k);
        Rhombus rhombus1;
        rhombus1.bottom = "D";
        rhombus1.top = "A";
        rhombus1.intermediates = QList<QString>{"C", "B"};
        rhombus1.overriddenMethods.insert("C", QList<Method*>{methodA});
        Rhombus rhombus2;
        rhombus2.bottom = "D";
        rhombus2.top = "G";
        rhombus2.intermediates = QList<QString>{"F", "E"};
        rhombus2.overriddenMethods.insert("E", QList<Method*>{methodG});
        QSet<Rhombus> expected = {rhombus1, rhombus2};
        QTest::newRow("Multiple Rhombuses") << classes << expected;
    }

    // Тест 14: ромб с точкой слияния в виде единственного потомка top
    {
        ClassMap classes;
        Method* method = new Method();
        method->returnType = "void";
        method->methodName = "methodA";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(method);
        Class* f = new Class();
        f->className = "F";
        Class* g = new Class();
        g->className = "G";
        Class* h = new Class();
        h->className = "H";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "F" << "B";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "G";
        d->methods.append(method);
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D" << "H";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        Rhombus expectedRhombus;
        expectedRhombus.bottom = "E";
        expectedRhombus.top = "A";
        expectedRhombus.intermediates = QList<QString>{"C", "D", "B"};
        expectedRhombus.overriddenMethods.insert("D", QList<Method*>{method});
        QSet<Rhombus> expected = {expectedRhombus};
        QTest::newRow("Rhombus Single Descendant Top") << classes << expected;
    }

    // Тест 15: ромб с точкой слияния в виде единственного потомка top и с переопределением в нем метода
    {
        ClassMap classes;
        Method* method = new Method();
        method->returnType = "void";
        method->methodName = "methodA";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(method);
        Class* f = new Class();
        f->className = "F";
        Class* g = new Class();
        g->className = "G";
        Class* h = new Class();
        h->className = "H";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        b->methods.append(method);
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "F" << "B";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "G";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D" << "H";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        QSet<Rhombus> expected;
        QTest::newRow("Rhombus Single Descendant Top With Override") << classes << expected;
    }

    // Тест 16: ромб с точкой слияния в виде единственного потомка top и с переопределением его метода в одном из промежуточных классов
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodB = new Method();
        methodB->returnType = "void";
        methodB->methodName = "methodB";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        Class* f = new Class();
        f->className = "F";
        Class* g = new Class();
        g->className = "G";
        Class* h = new Class();
        h->className = "H";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        b->methods.append(methodB);
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "F" << "B";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "G";
        d->methods.append(methodA);
        d->methods.append(methodB);
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D" << "H";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        Rhombus rhombus1;
        rhombus1.bottom = "E";
        rhombus1.top = "A";
        rhombus1.intermediates = QList<QString>{"C", "D", "B"};
        rhombus1.overriddenMethods.insert("D", QList<Method*>{methodA});
        Rhombus rhombus2;
        rhombus2.bottom = "E";
        rhombus2.top = "B";
        rhombus2.intermediates = QList<QString>{"C", "D"};
        rhombus2.overriddenMethods.insert("D", QList<Method*>{methodB});
        QSet<Rhombus> expected = {rhombus1, rhombus2};
        QTest::newRow("Rhombus Single Descendant Top Intermediate Override") << classes << expected;
    }

    // Тест 17: ромб с точкой слияния в виде единственного предка bottom и с переопределением метода top
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodB = new Method();
        methodB->returnType = "void";
        methodB->methodName = "methodB";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        a->methods.append(methodB);
        Class* g = new Class();
        g->className = "G";
        Class* h = new Class();
        h->className = "H";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";
        b->methods.append(methodB);
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "G" << "H";
        e->methods.append(methodA);
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("G", g);
        classes.insert("H", h);
        Rhombus expectedRhombus;
        expectedRhombus.bottom = "D";
        expectedRhombus.top = "A";
        expectedRhombus.intermediates = QList<QString>{"B", "C"};
        expectedRhombus.overriddenMethods.insert("B", QList<Method*>{methodB});
        QSet<Rhombus> expected = {expectedRhombus};
        QTest::newRow("Rhombus Single Ancestor Bottom") << classes << expected;
    }

    // Тест 18: ромб с разной глубиной наследования промежуточных классов
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodE = new Method();
        methodE->returnType = "void";
        methodE->methodName = "methodE";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        Class* j = new Class();
        j->className = "J";
        Class* k = new Class();
        k->className = "K";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A";
        e->methods.append(methodE);
        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "A";
        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "G";
        b->methods.append(methodA);
        b->methods.append(methodE);
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "F";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C" << "J" << "K";
        d->methods.append(methodE);
        classes.insert("A", a);
        classes.insert("J", j);
        classes.insert("K", k);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("C", c);
        classes.insert("B", b);
        classes.insert("D", d);
        Rhombus expectedRhombus;
        expectedRhombus.bottom = "D";
        expectedRhombus.top = "A";
        expectedRhombus.intermediates = QList<QString>{"B", "C", "G", "F", "E"};
        expectedRhombus.overriddenMethods.insert("B", QList<Method*>{methodA});
        QSet<Rhombus> expected = {expectedRhombus};
        QTest::newRow("Rhombus Different Inheritance Depth") << classes << expected;
    }

    // Тест 19: несколько точек слияния с переопределение методов класса top
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodB = new Method();
        methodB->returnType = "void";
        methodB->methodName = "methodB";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        a->methods.append(methodB);
        Class* m = new Class();
        m->className = "M";
        Class* l = new Class();
        l->className = "L";
        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "A";
        j->methods.append(methodA);
        Class* k = new Class();
        k->className = "K";
        k->directAncestors << "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "J" << "K";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D";
        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";
        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "E";
        h->methods.append(methodB);
        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "H" << "G" << "M" << "L";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("J", j);
        classes.insert("K", k);
        classes.insert("M", m);
        classes.insert("L", l);
        Rhombus rhombus1;
        rhombus1.bottom = "B";
        rhombus1.top = "A";
        rhombus1.intermediates = QList<QString>{"J", "K"};
        rhombus1.overriddenMethods.insert("J", QList<Method*>{methodA});
        Rhombus rhombus2;
        rhombus2.bottom = "F";
        rhombus2.top = "A";
        rhombus2.intermediates = QList<QString>{"H", "G", "E", "C", "D", "B", "J", "K"};
        rhombus2.overriddenMethods.insert("H", QList<Method*>{methodB});
        QSet<Rhombus> expected = {rhombus1, rhombus2};
        QTest::newRow("Multiple Merge Points") << classes << expected;
    }

    // Тест 20: цепочка наследования с несколькими точками слияния и с переопределением в различных частях ромбов
    {
        ClassMap classes;
        Method* methodA = new Method();
        methodA->returnType = "void";
        methodA->methodName = "methodA";
        Method* methodJ = new Method();
        methodJ->returnType = "void";
        methodJ->methodName = "methodJ";
        Method* methodE = new Method();
        methodE->returnType = "void";
        methodE->methodName = "methodE";
        Class* a = new Class();
        a->className = "A";
        a->methods.append(methodA);
        Class* l = new Class();
        l->className = "L";
        Class* p = new Class();
        p->className = "P";
        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "A";
        j->methods.append(methodA);
        j->methods.append(methodJ);
        Class* k = new Class();
        k->className = "K";
        k->directAncestors << "A";
        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "J" << "K";
        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";
        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";
        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D";
        e->methods.append(methodE);
        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";
        g->methods.append(methodJ);
        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "E";
        h->methods.append(methodA);
        Class* m = new Class();
        m->className = "M";
        m->directAncestors << "E";
        m->methods.append(methodE);
        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "H" << "G" << "M";
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("J", j);
        classes.insert("K", k);
        classes.insert("M", m);
        classes.insert("L", l);
        classes.insert("P", p);
        Rhombus rhombus1;
        rhombus1.bottom = "B";
        rhombus1.top = "A";
        rhombus1.intermediates = QList<QString>{"J", "K"};
        rhombus1.overriddenMethods.insert("J", QList<Method*>{methodA});
        Rhombus rhombus2;
        rhombus2.bottom = "F";
        rhombus2.top = "A";
        rhombus2.intermediates = QList<QString>{"H", "G", "M", "E", "C", "D", "B", "J", "K"};
        rhombus2.overriddenMethods.insert("H", QList<Method*>{methodA});
        Rhombus rhombus3;
        rhombus3.bottom = "F";
        rhombus3.top = "J";
        rhombus3.intermediates = QList<QString>{"H", "G", "M", "E", "C", "D", "B"};
        rhombus3.overriddenMethods.insert("G", QList<Method*>{methodJ});
        Rhombus rhombus4;
        rhombus4.bottom = "F";
        rhombus4.top = "E";
        rhombus4.intermediates = QList<QString>{"H", "G", "M"};
        rhombus4.overriddenMethods.insert("M", QList<Method*>{methodE});
        QSet<Rhombus> expected = {rhombus1, rhombus2, rhombus3, rhombus4};
        QTest::newRow("Chain With Multiple Merge Points") << classes << expected;
    }
}

void TestDetectRhomboid::testDetectRhomboidInheritance()
{
    QFETCH(ClassMap, classes);
    QFETCH(QSet<Rhombus>, expected);

    QSet<Rhombus> result = detectRhomboidInheritance(classes);

    QCOMPARE(result, expected);

    qDeleteAll(classes);
}
