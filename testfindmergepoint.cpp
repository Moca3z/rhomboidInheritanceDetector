#include "testfindmergepoint.h"
#include "findmergepoint.h"
#include "class.h"

// Алиасы для работы QFetch
using InheritanceMatrix = QMap<QString, QSet<QString>>;
using ClassMap = QMap<QString, Class*>;


void TestFindMergePoint::testFindMergePoint_data()
{
    QTest::addColumn<QString>("bottom");
    QTest::addColumn<QString>("top");
    QTest::addColumn<QList<QString>>("intermediates");
    QTest::addColumn<InheritanceMatrix>("inheritanceMatrix");
    QTest::addColumn<ClassMap>("classes");
    QTest::addColumn<QString>("expected");

    // Тест 1: 2 класса на одном уровне между bottom и top
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

        QTest::newRow("TwoClassesSameLevel") << "D" << "A" << QList<QString>{"B", "C"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 2: ромб из трех классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"C", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B" << "A";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);

        QTest::newRow("ThreeClassDiamond") << "C" << "A" << QList<QString>{"B"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 3: полная связь bottom с каждым классом
    {
        InheritanceMatrix inheritanceMatrix = {
            {"C", {"B", "D", "A"}},
            {"B", {"A"}},
            {"D", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B" << "A" << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        QTest::newRow("FullConnectionToBottom") << "C" << "A" << QList<QString>{"B", "D"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 4: дополнительное наследование для одного из промежуточных классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"B", "A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B" << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        QTest::newRow("AdditionalInheritanceIntermediate") << "D" << "A" << QList<QString>{"B", "C"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 5: точка слияния - прямой предок bottom
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

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

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

        QTest::newRow("MergePointDirectAncestorBottom") << "E" << "A" << QList<QString>{"D", "B", "C"} << inheritanceMatrix << classes << "D";
    }

    // Тест 6: точка слияния - единственный потомок top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

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
        classes.insert("X", x);

        QTest::newRow("MergePointDirectDescendantTop") << "D" << "X" << QList<QString>{"B", "C", "A"} << inheritanceMatrix << classes << "A";
    }

    // Тест 7: точка слияния - единственный потомок top с множеством промежуточных классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "E", "F", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"E", {"A", "X"}},
            {"F", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C" << "E" << "F";

        classes.insert("X", x);
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("D", d);

        QTest::newRow("MergePointDirectDescendantTopMultipleIntermediates") << "D" << "X" << QList<QString>{"B", "C", "E", "F", "A"} << inheritanceMatrix << classes << "A";
    }

    // Тест 8: обходной путь через потенциальную точку слияния в виде прямого потомка top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C" << "X";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("X", x);

        QTest::newRow("BypassPotentialMergePointDirectDescendantTop") << "D" << "X" << QList<QString>{"B", "C", "A"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 9: обходной путь через потенциальную точку слияния для прямого предка bottom
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

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A" << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        QTest::newRow("BypassPotentialMergePointDirectAncestorBottom") << "E" << "A" << QList<QString>{"D", "B", "C"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 10: две точки слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A", "X"}},
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

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
        classes.insert("X", x);

        QTest::newRow("TwoMergePoints") << "E" << "X" << QList<QString>{"D", "B", "C", "A"} << inheritanceMatrix << classes << "D";
    }

    // Тест 11: обход первой точки слияния во вторую точку слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A", "X"}},
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A" << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("X", x);

        QTest::newRow("BypassFirstMergePointToSecond") << "E" << "X" << QList<QString>{"D", "B", "C", "A"} << inheritanceMatrix << classes << "A";
    }

    // Тест 12: обход первой точки слияния в промежуточный класс
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A", "X"}},
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "B";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("X", x);

        QTest::newRow("BypassFirstMergePointToIntermediate") << "E" << "X" << QList<QString>{"D", "B", "C", "A"} << inheritanceMatrix << classes << "A";
    }

    // Тест 13: обход обеих точек слияния одновременно
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A", "X"}},
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "X";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("X", x);

        QTest::newRow("BypassBothMergePointsSimultaneously") << "E" << "X" << QList<QString>{"D", "B", "C", "A"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 14: обход обеих точек слияния при помощи нескольких путей
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "A", "X"}},
            {"D", {"B", "C", "A", "X"}},
            {"B", {"A", "X"}},
            {"C", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A" << "D" << "X";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("X", x);

        QTest::newRow("BypassBothMergePointsMultiplePaths") << "E" << "X" << QList<QString>{"D", "B", "C", "A"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 15: разная глубина наследования в одной из веток промежуточных классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"C", "G", "F", "D", "B", "A"}},
            {"G", {"F", "D", "B", "A"}},
            {"F", {"D", "B", "A"}},
            {"C", {"A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "D";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "F";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "G";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("F", f);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("E", e);

        QTest::newRow("DifferentInheritanceDepthOneBranch") << "E" << "A" << QList<QString>{"C", "G", "F", "D", "B"} << inheritanceMatrix << classes << "Null";
    }

    // Тест 16: разная глубина наследования с точкой слияния перед промежуточными классами
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"G", "C", "F", "D", "B", "A"}},
            {"G", {"C", "F", "D", "B", "A"}},
            {"F", {"D", "B", "A"}},
            {"C", {"A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "D";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "C" << "F";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "G";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("F", f);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("E", e);

        QTest::newRow("DifferentInheritanceDepthAfterMergePoint") << "E" << "A" << QList<QString>{"G", "C", "F", "D", "B"} << inheritanceMatrix << classes << "G";
    }

    // Тест 17: разная глубина наследования с двумя точками слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"G", "F", "C", "D", "B", "H", "A", "X"}},
            {"G", {"F", "C", "D", "B", "H", "A", "X"}},
            {"F", {"D", "B", "A", "X"}},
            {"C", {"H", "A", "X"}},
            {"D", {"B", "A", "X"}},
            {"B", {"A", "X"}},
            {"H", {"A", "X"}},
            {"A", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "H";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "D";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "F" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "G";

        classes.insert("X", x);
        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("H", h);
        classes.insert("D", d);
        classes.insert("C", c);
        classes.insert("F", f);
        classes.insert("G", g);
        classes.insert("E", e);

        QTest::newRow("DifferentInheritanceDepthTwoMergePoints") << "E" << "X" << QList<QString>{"G", "F", "C", "D", "H", "B", "A"} << inheritanceMatrix << classes << "G";
    }

    // Тест 18: отсутствие intermediates
    {
        InheritanceMatrix inheritanceMatrix = {
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        classes.insert("A", a);
        classes.insert("B", b);

        QTest::newRow("NoIntermediates") << "B" << "A" << QList<QString>{} << inheritanceMatrix << classes << "Null";
    }

    // Тест 19: только один класс intermediates
    {
        InheritanceMatrix inheritanceMatrix = {
            {"C", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

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

        QTest::newRow("SingleIntermediate") << "B" << "A" << QList<QString>{"B"} << inheritanceMatrix << classes << "B";
    }

    // Тест 20: прямая ветвь наследования
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "C", "B", "A"}},
            {"D", {"C", "B", "A"}},
            {"C", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

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

        QTest::newRow("StraightInheritanceBranch") << "E" << "A" << QList<QString>{"D", "C", "B"} << inheritanceMatrix << classes << "D";
    }

    // Тест 21: два ромба, связанные друг с другом
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"E", "D", "B", "C", "A"}},
            {"E", {"B", "A"}},
            {"D", {"B", "C", "A"}},
            {"C", {"A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "E" << "D";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);

        QTest::newRow("TwoConnectedDiamonds") << "F" << "A" << QList<QString>{"E", "D", "B", "C"} << inheritanceMatrix << classes << "Null";
    }

    // Test case 22: testChainOfMergePoints
    {
        InheritanceMatrix inheritanceMatrix = {
            {"R", {"P", "O", "K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A"}},
            {"P", {"K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A"}},
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
        e->directAncestors << "C" << "D";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "E";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "F" << "G";

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "H";

        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "H";

        Class* k = new Class();
        k->className = "K";
        k->directAncestors << "I" << "J";

        Class* p = new Class();
        p->className = "P";
        p->directAncestors << "K";

        Class* o = new Class();
        o->className = "O";
        o->directAncestors << "K";

        Class* r = new Class();
        r->className = "R";
        r->directAncestors << "P" << "O";

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
        classes.insert("P", p);
        classes.insert("O", o);
        classes.insert("R", r);

        QTest::newRow("ChainOfMergePoints") << "R" << "A" << QList<QString>{"P", "O", "K", "I", "J", "H", "F", "G", "E", "C", "D", "B"} << inheritanceMatrix << classes << "K";
    }
}

void TestFindMergePoint::testFindMergePoint()
{
    QFETCH(QString, bottom);
    QFETCH(QString, top);
    QFETCH(QList<QString>, intermediates);
    QFETCH(InheritanceMatrix, inheritanceMatrix);
    QFETCH(ClassMap, classes);
    QFETCH(QString, expected);

    QString result = findMergePoint(bottom, top, intermediates, inheritanceMatrix, classes);
    QCOMPARE(result, expected);

    qDeleteAll(classes);
}



