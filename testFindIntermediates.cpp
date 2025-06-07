#include "testfindintermediates.h"
#include "findintermediates.h"
#include "class.h"

// Алиасы для работы QFetch
using InheritanceMatrix = QMap<QString, QSet<QString>>;
using ClassMap = QMap<QString, Class*>;
using IntermediatesMap = QMultiMap<int, QString>;

void TestFindIntermediates::testFindIntermediates_data()
{
    QTest::addColumn<InheritanceMatrix>("inheritanceMatrix");
    QTest::addColumn<ClassMap>("classes");
    QTest::addColumn<QString>("top");
    QTest::addColumn<QString>("bottom");
    QTest::addColumn<IntermediatesMap>("expected");

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

        IntermediatesMap expected;
        expected.insert(1, "B");
        expected.insert(1, "C");
        QTest::newRow("TwoClassesOnSameLevel") << inheritanceMatrix << classes << "A" << "D" << expected;
    }

    // Тест 2: отсутствие классов между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A", "B"}},
            {"A", {}},
            {"B", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A" << "B";

        classes.insert("A", a);
        classes.insert("D", d);
        classes.insert("B", b);

        QTest::newRow("NoClassesBetween") << inheritanceMatrix << classes << "A" << "D" << IntermediatesMap();
    }

    // Тест 3: один класс между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"C", "B", "E", "A", "F"}},
            {"B", {"A", "F"}},
            {"A", {}},
            {"C", {}},
            {"E", {}},
            {"F", {}}
        };

        ClassMap classes;
        Class* f = new Class();
        f->className = "F";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "F";

        Class* c = new Class();
        c->className = "C";

        Class* e = new Class();
        e->className = "E";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C" << "E";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("C", c);
        classes.insert("E", e);
        classes.insert("F", f);

        IntermediatesMap expected;
        expected.insert(1, "B");
        QTest::newRow("OneClassBetween") << inheritanceMatrix << classes << "A" << "D" << expected;
    }

    // Тест 4: прямая ветвь наследования между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"E", "B", "C", "A", "F", "H", "G"}},
            {"E", {"B", "C", "A", "F", "H", "G"}},
            {"B", {"C", "A"}},
            {"C", {"A"}},
            {"A", {}},
            {"F", {"H"}},
            {"H", {}},
            {"G", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* h = new Class();
        h->className = "H";

        Class* g = new Class();
        g->className = "G";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B" << "F" << "G";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "E";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "H";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("H", h);
        classes.insert("G", g);

        IntermediatesMap expected;
        expected.insert(1, "E");
        expected.insert(2, "B");
        expected.insert(3, "C");
        QTest::newRow("StraightInheritanceChain") << inheritanceMatrix << classes << "A" << "D" << expected;
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

        QTest::newRow("BottomAncestorsNotConnectedToTop") << inheritanceMatrix << classes << "A" << "D" << IntermediatesMap();
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

        IntermediatesMap expected;
        expected.insert(1, "B");
        expected.insert(1, "C");
        QTest::newRow("FullConnectionToBottom") << inheritanceMatrix << classes << "A" << "D" << expected;
    }

    // Тест 7: прямой предок bottom - точка слияния между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"I", {"H", "G", "E", "A", "B", "C"}},
            {"H", {"G", "E", "A", "B", "C"}},
            {"G", {"A"}},
            {"E", {"A"}},
            {"A", {}},
            {"B", {}},
            {"C", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";

        Class* c = new Class();
        c->className = "C";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "A";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "A";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "G" << "E" << "B" << "C";

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "H";

        classes.insert("A", a);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("H", h);
        classes.insert("I", i);
        classes.insert("B", h);
        classes.insert("C", i);

        IntermediatesMap expected;
        expected.insert(1, "H");
        expected.insert(2, "G");
        expected.insert(2, "E");
        QTest::newRow("DirectAncestorMergePoint") << inheritanceMatrix << classes << "A" << "I" << expected;
    }

    // Тест 8: прямой потомок - точка слияния между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"I", {"G", "E", "B", "A", "C", "F", "X"}},
            {"E", {"B", "A", "F", "X"}},
            {"G", {"B", "A", "C", "X"}},
            {"B", {"A", "X"}},
            {"A", {"X"}},
            {"C", {}},
            {"F", {}},
            {"X", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "X";

        Class* c = new Class();
        c->className = "C";

        Class* f = new Class();
        f->className = "F";

        Class* x = new Class();
        x->className = "X";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "B" << "C";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B" << "F";

        Class* i = new Class();
        i->className = "I";
        i->directAncestors << "G" << "E";

        classes.insert("A", a);
        classes.insert("G", g);
        classes.insert("E", e);
        classes.insert("B", b);
        classes.insert("I", i);
        classes.insert("C", c);
        classes.insert("F", f);
        classes.insert("X", x);

        IntermediatesMap expected;
        expected.insert(1, "G");
        expected.insert(1, "E");
        expected.insert(2, "B");
        QTest::newRow("DirectDescendantMergePoint") << inheritanceMatrix << classes << "A" << "I" << expected;
    }

    // Тест 9: два ромба, связанные друг с другом, между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"F", {"E", "D", "B", "C", "A", "I", "J", "G", "H"}},
            {"E", {"B", "A", "G"}},
            {"D", {"B", "C", "A", "H"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}},
            {"G", {}},
            {"I", {}},
            {"H", {}},
            {"J", {}}
        };

        ClassMap classes;
        Class* a = new Class();
        a->className = "A";

        Class* i = new Class();
        i->className = "I";

        Class* j = new Class();
        j->className = "J";

        Class* g = new Class();
        g->className = "G";

        Class* h = new Class();
        h->className = "H";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "A";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "C" << "H";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "B" << "G";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "E" << "D" << "I" << "J";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("C", c);
        classes.insert("D", d);
        classes.insert("E", e);
        classes.insert("F", f);
        classes.insert("I", i);
        classes.insert("G", g);
        classes.insert("H", h);
        classes.insert("J", j);

        IntermediatesMap expected;
        expected.insert(1, "E");
        expected.insert(1, "D");
        expected.insert(2, "B");
        expected.insert(2, "C");
        QTest::newRow("TwoDiamondsConnected") << inheritanceMatrix << classes << "A" << "F" << expected;
    }

    // Тест 10: длинная цепочка наследований между bottom и top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"R", {"P", "O", "K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A", "M", "L", "X", "Q", "Y", "Z"}},
            {"P", {"K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A", "X", "Q", "Y", "Z"}},
            {"O", {"K", "I", "J", "H", "F", "G", "E", "C", "D", "B", "A", "X", "Q", "Y", "Z"}},
            {"K", {"I", "J", "H", "F", "G", "E", "C", "D", "B", "A", "X", "Q", "Y", "Z"}},
            {"I", {"H", "F", "G", "E", "C", "D", "B", "A", "X", "Q", "Y", "Z"}},
            {"J", {"H", "F", "G", "E", "C", "D", "B", "A", "X", "Q", "Y", "Z"}},
            {"H", {"F", "G", "E", "C", "D", "B", "A", "X", "Q", "Y", "Z"}},
            {"F", {"E", "C", "D", "B", "A", "Q", "Y", "Z"}},
            {"G", {"E", "C", "D", "B", "A", "Q", "Y", "Z"}},
            {"E", {"C", "D", "B", "A", "Q", "Y", "Z"}},
            {"C", {"B", "A", "Z"}},
            {"D", {"B", "A", "Y", "Z"}},
            {"B", {"A", "Z"}},
            {"A", {"Z"}},
            {"M", {}},
            {"L", {}},
            {"X", {}},
            {"Q", {}},
            {"Y", {}},
            {"Z", {}}
        };

        ClassMap classes;

        Class* m = new Class();
        m->className = "M";

        Class* l = new Class();
        l->className = "L";

        Class* x = new Class();
        x->className = "X";

        Class* q = new Class();
        q->className = "Q";

        Class* y = new Class();
        y->className = "Y";

        Class* z = new Class();
        z->className = "Z";

        Class* a = new Class();
        a->className = "A";
        a->directAncestors << "Y";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A";

        Class* c = new Class();
        c->className = "C";
        c->directAncestors << "B";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "B" << "Y";

        Class* e = new Class();
        e->className = "E";
        e->directAncestors << "C" << "D" << "Q";

        Class* f = new Class();
        f->className = "F";
        f->directAncestors << "E";

        Class* g = new Class();
        g->className = "G";
        g->directAncestors << "E";

        Class* h = new Class();
        h->className = "H";
        h->directAncestors << "F" << "G" << "X";

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
        r->directAncestors << "P" << "O" << "M" << "L";

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
        classes.insert("M", m);
        classes.insert("L", l);
        classes.insert("X", x);
        classes.insert("Q", q);
        classes.insert("Y", y);
        classes.insert("Z", z);

        IntermediatesMap expected;
        expected.insert(1, "P");
        expected.insert(1, "O");
        expected.insert(2, "K");
        expected.insert(3, "I");
        expected.insert(3, "J");
        expected.insert(4, "H");
        expected.insert(5, "F");
        expected.insert(5, "G");
        expected.insert(6, "E");
        expected.insert(7, "C");
        expected.insert(7, "D");
        expected.insert(8, "B");
        QTest::newRow("LongInheritanceChain") << inheritanceMatrix << classes << "A" << "R" << expected;
    }

    // Тест 11: ромб из трех классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A", "B", "C", "E"}},
            {"B", {"A", "C"}},
            {"A", {}},
            {"C", {}},
            {"E", {}}
        };

        ClassMap classes;

        Class* c = new Class();
        c->className = "C";

        Class* e = new Class();
        e->className = "E";

        Class* a = new Class();
        a->className = "A";

        Class* b = new Class();
        b->className = "B";
        b->directAncestors << "A" << "C";

        Class* d = new Class();
        d->className = "D";
        d->directAncestors << "A" << "B" << "E";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("C", c);
        classes.insert("E", e);

        IntermediatesMap expected;
        expected.insert(1, "B");
        QTest::newRow("ThreeClassRhombus") << inheritanceMatrix << classes << "A" << "D" << expected;
    }

    // Тест 12: разная глубина наследования с обходными путями нескольких точек слияния
    {
        InheritanceMatrix inheritanceMatrix = {
            {"E", {"A", "G", "C", "F", "H", "D", "B", "X", "N", "O", "L", "M"}},
            {"G", {"C", "F", "H", "D", "B", "X", "A", "L", "M"}},
            {"C", {"H", "X", "A"}},
            {"F", {"D", "B", "X", "A", "L", "M"}},
            {"H", {"X", "A"}},
            {"D", {"B", "X", "A"}},
            {"B", {"X", "A"}},
            {"X", {"A"}},
            {"A", {}},
            {"N", {}},
            {"O", {}},
            {"L", {}},
            {"M", {}}
        };

        ClassMap classes;

        Class* n = new Class();
        n->className = "N";

        Class* o = new Class();
        o->className = "O";

        Class* l = new Class();
        l->className = "L";

        Class* m = new Class();
        m->className = "M";

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
        f->directAncestors << "D" << "L" << "M";

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
        e->directAncestors << "A" << "G" << "N" << "O";

        classes.insert("A", a);
        classes.insert("B", b);
        classes.insert("D", d);
        classes.insert("H", h);
        classes.insert("F", f);
        classes.insert("C", c);
        classes.insert("G", g);
        classes.insert("X", x);
        classes.insert("E", e);
        classes.insert("N", c);
        classes.insert("O", g);
        classes.insert("L", x);
        classes.insert("M", e);

        IntermediatesMap expected;
        expected.insert(1, "G");
        expected.insert(2, "C");
        expected.insert(2, "F");
        expected.insert(3, "H");
        expected.insert(3, "D");
        expected.insert(4, "B");
        expected.insert(4, "X");
        QTest::newRow("DifferentDepthsWithBypass") << inheritanceMatrix << classes << "A" << "E" << expected;
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

        IntermediatesMap expected;
        expected.insert(1, "B");
        expected.insert(1, "C");
        QTest::newRow("MultipleRhombusesCommonBottom") << inheritanceMatrix << classes << "A" << "E" << expected;
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

        IntermediatesMap expected;
        expected.insert(1, "B");
        expected.insert(1, "C");
        QTest::newRow("MultipleRhombusesCommonBottomAndTop") << inheritanceMatrix << classes << "A" << "E" << expected;
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

        IntermediatesMap expected;
        expected.insert(1, "D");
        expected.insert(1, "B");
        expected.insert(1, "C");
        expected.insert(1, "G");
        expected.insert(2, "F");
        expected.insert(2, "A");
        expected.insert(2, "H");
        QTest::newRow("MultipleRhombusesUnionPoint") << inheritanceMatrix << classes << "X" << "E" << expected;
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

        IntermediatesMap expected;
        expected.insert(1, "D");
        expected.insert(1, "B");
        expected.insert(1, "C");
        expected.insert(2, "F");
        expected.insert(2, "A");
        QTest::newRow("NotAllRhombusesUnite") << inheritanceMatrix << classes << "X" << "E" << expected;
    }
}

void TestFindIntermediates::testFindIntermediates()
{
    QFETCH(InheritanceMatrix, inheritanceMatrix);
    QFETCH(ClassMap, classes);
    QFETCH(QString, top);
    QFETCH(QString, bottom);
    QFETCH(IntermediatesMap, expected);

    IntermediatesMap result = findIntermediates(top, bottom, inheritanceMatrix, classes);
    QCOMPARE(result, expected);

    qDeleteAll(classes);
}
