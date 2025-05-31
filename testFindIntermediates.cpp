#include "testfindintermediates.h"
#include "findintermediates.h"
#include "class.h"

// Алиасы для работы QFetch
using InheritanceMatrix = QMap<QString, QSet<QString>>;
using ClassMap = QMap<QString, Class*>;

void TestFindIntermediates::testFindIntermediates_data()
{
    QTest::addColumn<InheritanceMatrix>("inheritanceMatrix");
    QTest::addColumn<ClassMap>("classes");
    QTest::addColumn<QString>("top");
    QTest::addColumn<QString>("bottom");
    QTest::addColumn<QList<QString>>("expected");

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

        QTest::newRow("TwoClassesOnSameLevel") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{"B", "C"};
    }

    // Тест 2: отсутствие классов между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A";

        classes.insert("A", a);
        classes.insert("D", d);

        QTest::newRow("NoClassesBetween") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{};
    }

    // Тест 3: один класс между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
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

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);

        QTest::newRow("OneClassBetween") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{"B"};
    }

    // Тест 4: прямая ветвь наследования между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"E", "B", "C", "A"}},
            {"E", {"B", "C", "A"}},
            {"B", {"C", "A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "E";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        QTest::newRow("StraightInheritanceChain") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{"E", "B", "C"};
    }

    // Тест 5: не связанные с top предки bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A", "B", "C", "E"}},
            {"B", {"E"}},
            {"C", {"E"}},
            {"A", {}},
            {"E", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "E";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "E";

        Class* e = new Class();
        e->className = "E";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A" << "B" << "C";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);

        QTest::newRow("BottomAncestorsNotConnectedToTop") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{};
    }

    // Тест 6: полная связь bottom с каждым классом
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
        d->directAncestors << "A" << "B" << "C";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);

        QTest::newRow("FullConnectionToBottom") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{"B", "C"};
    }

    // Тест 7: прямой предок bottom - точка слияния между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"I", {"H", "G", "E", "A"}},
            {"H", {"G", "E", "A"}},
            {"G", {"A"}},
            {"E", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "A";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "G" << "E";

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "H";

        classes.insert("A", a);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("H", h);
        classes.insert("I", i);

        QTest::newRow("DirectAncestorMergePoint") << inheritanceMatrix << classes << "A" << "I" << QList<QString>{"H", "G", "E"};
    }

    // Тест 8: прямой потомок - точка слияния между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"I", {"G", "E", "B", "A"}},
            {"E", {"B", "A"}},
            {"G", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "B";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B";

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "G" << "E";

        classes.insert("A", a);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("B", b);
        classes.insert("I", i);

        QTest::newRow("DirectDescendantMergePoint") << inheritanceMatrix << classes << "A" << "I" << QList<QString>{"G", "E", "B"};
    }

    // Тест 9: два ромба, связанные друг с другом, между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"E", "D", "B", "C", "A"}},
            {"E", {"B", "A"}},
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

        QTest::newRow("TwoDiamondsConnected") << inheritanceMatrix << classes << "A" << "F" << QList<QString>{"E", "D", "B", "C"};
    }

    // Тест 10: длинная цепочка наследований между bottom и top
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

        QTest::newRow("LongInheritanceChain") << inheritanceMatrix << classes << "A" << "R" << QList<QString>{"P", "O", "K", "I", "J", "H", "F", "G", "E", "C", "D", "B"};
    }

    // Тест 11: ромб из трех классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A", "B"}},
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
        d->directAncestors << "A" << "B";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);

        QTest::newRow("ThreeClassRhombus") << inheritanceMatrix << classes << "A" << "D" << QList<QString>{"B"};
    }

    // Тест 12: разная глубина наследования с обхнодными путями нескольких точек слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"A", "G", "C", "F", "H", "D", "B", "X"}},
            {"G", {"C", "F", "H", "D", "B", "X", "A"}},
            {"C", {"H", "X", "A"}},
            {"F", {"D", "B", "X", "A"}},
            {"H", {"X", "A"}},
            {"D", {"B", "X", "A"}},
            {"B", {"X", "A"}},
            {"X", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "X" << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "X";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "D";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "H";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "C" << "F";

        Class* x = new Class();
        x->className = "X";
        x->directAncestors << "A";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A" << "G";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("H", h);
        classes.insert("F", f);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("X", x);
        classes.insert("E", e);

        QTest::newRow("DifferentDepthsWithBypass") << inheritanceMatrix << classes << "A" << "E" << QList<QString>{"G", "C", "F", "H", "D", "B", "X"};
    }

    // Тест 13: несколько ромбов с общим bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "G", "F", "A", "H"}},
            {"D", {"F"}},
            {"B", {"F", "A"}},
            {"C", {"A", "H"}},
            {"G", {"H"}},
            {"F", {}},
            {"A", {}},
            {"H", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* f = new Class();
        f->className = "F";

        Class* h = new Class();
        h->className = "H";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "F" << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "F";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A" << "H";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "H";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "B" << "C" << "G";

        classes.insert("F", f);
        classes.insert("A", a);
        classes.insert("H", h);
        classes.insert("D", d);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("E", e);

        QTest::newRow("MultipleRhombusesCommonBottom") << inheritanceMatrix << classes << "A" << "E" << QList<QString>{"B", "C"};
    }

    // Тест 14: несколько ромбов с общим bottom и top в одном из них
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "G", "F", "A", "H", "X"}},
            {"D", {"F", "X"}},
            {"B", {"F", "A", "X"}},
            {"C", {"A", "H", "X"}},
            {"G", {"H", "X"}},
            {"F", {"X"}},
            {"A", {"X"}},
            {"H", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "X";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "F" << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "F";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A" << "H";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "H";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "B" << "C" << "G";

        classes.insert("F", f);
        classes.insert("A", a);
        classes.insert("H", h);
        classes.insert("D", d);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("X", x);

        QTest::newRow("MultipleRhombusesCommonBottomAndTop") << inheritanceMatrix << classes << "A" << "E" << QList<QString>{"B", "C"};
    }

    // Тест 15: несколько ромбов с общим bottom и top в точке их объединения
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "G", "F", "A", "H", "X"}},
            {"D", {"F", "X"}},
            {"B", {"F", "A", "X"}},
            {"C", {"A", "H", "X"}},
            {"G", {"H", "X"}},
            {"F", {"X"}},
            {"A", {"X"}},
            {"H", {"X"}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "X";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "F" << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "F";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A" << "H";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "H";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "B" << "C" << "G";

        classes.insert("F", f);
        classes.insert("A", a);
        classes.insert("H", h);
        classes.insert("D", d);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("X", x);

        QTest::newRow("MultipleRhombusesUnionPoint") << inheritanceMatrix << classes << "X" << "E" << QList<QString>{"D", "B", "C", "G", "F", "A", "H"};
    }

    // Тест 16: не все ромбы объединяются в одной точке
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"D", "B", "C", "G", "J", "K", "F", "A", "H", "I", "X"}},
            {"D", {"F", "X"}},
            {"B", {"F", "A", "X"}},
            {"C", {"A", "H", "X"}},
            {"G", {"H"}},
            {"J", {"I"}},
            {"K", {"I"}},
            {"F", {"X"}},
            {"A", {"X"}},
            {"H", {}},
            {"I", {}},
            {"X", {}}
        };

        ClassMap classes;
        Class* x = new Class();
        x->className = "X";

        Class* i = new Class();
        i->className = "I";

        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "I";

        Class* k = new Class();
        k->className = "K";
        k->directAncestors << "I";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "X";

        Class* h = new Class();
        h->className = "H";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "F" << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "F";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A" << "H";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "H";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "D" << "B" << "C" << "G" << "J" << "K";

        classes.insert("F", f);
        classes.insert("A", a);
        classes.insert("H", h);
        classes.insert("D", d);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("X", x);
        classes.insert("I", i);
        classes.insert("J", j);
        classes.insert("K", k);

        QTest::newRow("NotAllRhombusesUnite") << inheritanceMatrix << classes << "X" << "E" << QList<QString>{"D", "B", "C", "F", "A"};
    }

    // Тест 17: цепочка наследования с разветвлением одного из ромбов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"R", {"S", "T", "H", "K", "J", "G", "F", "B", "D", "A", "X", "Y"}},
            {"S", {"H", "K", "J", "G", "F", "B", "D", "A", "X", "Y"}},
            {"T", {"H", "K", "J", "G", "F", "B", "D", "A", "X", "Y"}},
            {"H", {"K", "J", "G", "F", "B", "D", "A", "X", "Y"}},
            {"K", {"J", "G", "F", "B", "D", "A", "X", "Y"}},
            {"J", {"G", "F", "B", "D", "A", "X", "Y"}},
            {"G", {"B", "D", "X", "Y"}},
            {"F", {"D", "A", "X", "Y"}},
            {"D", {"X", "Y"}},
            {"X", {"Y"}},
            {"B", {}},
            {"A", {}},
            {"Y", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";

        Class* y = new Class();
        y->className = "Y";

        Class* x = new Class();
        x->className = "X";
        x->directAncestors << "Y";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "X";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "B" << "D";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "D" << "A";

        Class* j = new Class();
        j->className = "J";
        j->directAncestors << "G" << "F";

        Class* k = new Class();
        k->className = "K";
        k->directAncestors << "J";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "K";

        Class* s = new Class();
        s->className = "S";
        s->directAncestors << "H";

        Class* t = new Class();
        t->className = "T";
        t->directAncestors << "H";

        Class* r = new Class();
        r->className = "R";
        r->directAncestors << "S" << "T";

        classes.insert("Y", y);
        classes.insert("X", x);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("A", a);
        classes.insert("G", g);
        classes.insert("F", f);
        classes.insert("J", j);
        classes.insert("K", k);
        classes.insert("H", h);
        classes.insert("S", s);
        classes.insert("T", t);
        classes.insert("R", r);

        QTest::newRow("ChainWithRhombusBranching") << inheritanceMatrix << classes << "A" << "R" << QList<QString>{"S", "T", "H", "K", "J", "G", "F", "D"};
    }
}

void TestFindIntermediates::testFindIntermediates()
{
    QFETCH(InheritanceMatrix, inheritanceMatrix);
    QFETCH(ClassMap, classes);
    QFETCH(QString, top);
    QFETCH(QString, bottom);
    QFETCH(QList<QString>, expected);

    QList<QString> result = findIntermediates(top, bottom, inheritanceMatrix, classes);
    QCOMPARE(result, expected);

    qDeleteAll(classes);
}
