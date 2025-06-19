/*!
 * \file
 * \brief Файл содержит реализацию тестов для функции detectRhomboidInheritance.
 */
#include "testdetectrhomboidinheritance.h"
#include "detectrhomboidinheritance.h"
#include "class.h"
#include <QDebug>

using ClassMap = QMap<QString, Class*>;
using IntermediatesMap = QMultiMap<int, QString>;

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
        classes.insert("A", new Class("A"));
        QSet<Rhombus> expected;
        QTest::newRow("Single Class") << classes << expected;
    }

    // Тест 3: два класса в графе с связью наследования
    {
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        QSet<Rhombus> expected;
        QTest::newRow("Two Classes With Inheritance") << classes << expected;
    }

    // Тест 4: две несвязанные вершины графа
    {
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C"));
        QSet<Rhombus> expected;
        QTest::newRow("Two Unconnected Classes") << classes << expected;
    }

    // Тест 5: один промежуточный класс
    {
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}));
        QSet<Rhombus> expected;
        QTest::newRow("One Intermediate Class") << classes << expected;
    }

    // Тест 6: прямая цепочка наследований
    {
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"C"}));
        classes.insert("E", new Class("E", {"D"}));
        QSet<Rhombus> expected;
        QTest::newRow("Straight Inheritance Chain") << classes << expected;
    }

    // Тест 7: прямая цепочка наследований с переопределением метода
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}, {new Method("void", "methodA")}));
        classes.insert("D", new Class("D", {"C"}));
        classes.insert("E", new Class("E", {"D"}));
        QSet<Rhombus> expected;
        QTest::newRow("Straight Chain With Method Override") << classes << expected;
    }

    // Тест 8: разветвление прямых цепочек наследования разной длины с переопределением методов
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("B", new Class("B", {}, {new Method("void", "methodB")}));
        classes.insert("C", new Class("C", {}, {new Method("void", "methodC")}));
        classes.insert("D", new Class("D", {"A"}, {new Method("void", "methodA")}));
        classes.insert("E", new Class("E", {"B"}, {new Method("void", "methodB")}));
        classes.insert("F", new Class("F", {"C"}, {new Method("void", "methodC")}));
        classes.insert("G", new Class("G", {"D"}));
        classes.insert("H", new Class("H", {"E"}));
        classes.insert("I", new Class("I", {"G"}));
        classes.insert("J", new Class("J", {"F", "H", "I"}));
        QSet<Rhombus> expected;
        QTest::newRow("Branching Chains With Method Override") << classes << expected;
    }

    // Тест 9: ромб из трех классов без переопределения методов
    {
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A", "B"}));
        QSet<Rhombus> expected;
        QTest::newRow("Rhombus Three Classes No Override") << classes << expected;
    }

    // Тест 10: ромб из трех классов с переопределением методов
    {
        ClassMap classes;

        Parameter p1("int", "a"), p2("int", "b");

        classes.insert("A", new Class("A", {}, {new Method("int", "methodA", {p1, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "methodA", {p1, p2})}));
        classes.insert("D", new Class("D"));
        classes.insert("G", new Class("G"));
        classes.insert("F", new Class("F", {"G"}));
        classes.insert("C", new Class("C", {"A", "B", "D", "F"}));

        QSet<Rhombus> expected = {Rhombus("C", "A", {{1, "B"}}, {{"B", {new Method("int", "methodA", {p1, p2})}}})};
        QTest::newRow("Rhombus Three Classes With Override") << classes << expected;
    }

    // Тест 11: один ромб с более чем одним промежуточным классом и с переопределением методов в bottom ромба
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}, {new Method("void", "methodA")}));
        QSet<Rhombus> expected;
        QTest::newRow("Rhombus Multiple Intermediates Bottom Override") << classes << expected;
    }

    // Тест 12: один ромб с более чем одним промежуточным классом и с переопределением методов
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}, {new Method("void", "methodA")}));
        classes.insert("D", new Class("D", {"B", "C"}));
        QSet<Rhombus> expected = {Rhombus("D", "A", {{1, "B"}, {1, "C"}}, {{"C", {new Method("void", "methodA")}}})};
        QTest::newRow("Rhombus Multiple Intermediates") << classes << expected;
    }

    // Тест 13: несколько ромбов с более чем одним промежуточным классом и переопределением методов
    {
        ClassMap classes;

        Parameter p1("int", "a"), p2("int", "b");

        classes.insert("A", new Class("A", {}, {new Method("int", "methodA", {p1, p2})}));
        classes.insert("M", new Class("M"));
        classes.insert("G", new Class("G", {}, {new Method("void", "methodG")}));
        classes.insert("K", new Class("K"));
        classes.insert("H", new Class("H"));
        classes.insert("J", new Class("J", {"M"}));
        classes.insert("B", new Class("B", {"A", "J"}, {new Method("int", "methodA", {p1, p2})}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("F", new Class("F", {"G"}));
        classes.insert("E", new Class("E", {"G", "K"}, {new Method("void", "methodG")}));
        classes.insert("D", new Class("D", {"C", "B", "F", "E", "H"}));
        QSet<Rhombus> expected = {
            Rhombus("D", "A", {{1, "C"}, {1, "B"}}, {{"B", {new Method("int", "methodA", {p1, p2})}}}),
            Rhombus("D", "G", {{1, "F"}, {1, "E"}}, {{"E", {new Method("void", "methodG")}}})
        };
        QTest::newRow("Multiple Rhombuses") << classes << expected;
    }

    // Тест 14: ромб с точкой слияния в виде единственного потомка top
    {
        ClassMap classes;

        Parameter p1("QString", "str"), p2("int", "b");

        classes.insert("A", new Class("A", {}, {new Method("QString", "methodA", {p1, p2})}));
        classes.insert("F", new Class("F"));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"F", "B"}));
        classes.insert("D", new Class("D", {"B", "G"}, {new Method("QString", "methodA", {p1, p2})}));
        classes.insert("E", new Class("E", {"C", "D", "H"}));
        QSet<Rhombus> expected = {Rhombus("E", "A", {{1, "C"}, {1, "D"}, {2, "B"}}, {{"D", {new Method("QString", "methodA", {p1, p2})}}})};
        QTest::newRow("Rhombus Single Descendant Top") << classes << expected;
    }

    // Тест 15: ромб с точкой слияния в виде единственного потомка top и с переопределением в нем метода
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("F", new Class("F"));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}, {new Method("void", "methodA")}));
        classes.insert("C", new Class("C", {"F", "B"}));
        classes.insert("D", new Class("D", {"B", "G"}));
        classes.insert("E", new Class("E", {"C", "D", "H"}));
        QSet<Rhombus> expected;
        QTest::newRow("Rhombus Single Descendant Top With Override") << classes << expected;
    }

    // Тест 16: ромб с точкой слияния в виде единственного потомка top и с переопределением его метода в одном из промежуточных классов
    {
        ClassMap classes;

        Parameter p1("int", "a"), p2("int", "b");

        classes.insert("A", new Class("A", {}, {new Method("int", "methodA", {p1, p2})}));
        classes.insert("F", new Class("F"));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}, {new Method("void", "methodB")}));
        classes.insert("C", new Class("C", {"F", "B"}));
        classes.insert("D", new Class("D", {"B", "G"}, {new Method("int", "methodA", {p1, p2}), new Method("void", "methodB")}));
        classes.insert("E", new Class("E", {"C", "D", "H"}));
        QSet<Rhombus> expected = {
            Rhombus("E", "A", {{1, "C"}, {1, "D"}, {2, "B"}}, {{"D", {new Method("int", "methodA", {p1, p2})}}}),
            Rhombus("E", "B", {{1, "C"}, {1, "D"}}, {{"D", {new Method("void", "methodB")}}})
        };
        QTest::newRow("Rhombus Single Descendant Top Intermediate Override") << classes << expected;
    }

    // Тест 17: ромб с точкой слияния в виде единственного предка bottom и с переопределением метода top
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA"), new Method("void", "methodB")}));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}, {new Method("void", "methodB")}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D", "G", "H"}, {new Method("void", "methodA")}));
        QSet<Rhombus> expected = {Rhombus("D", "A", {{1, "B"}, {1, "C"}}, {{"B", {new Method("void", "methodB")}}})};
        QTest::newRow("Rhombus Single Ancestor Bottom") << classes << expected;
    }

    // Тест 18: ромб с разной глубиной наследования промежуточных классов
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("J", new Class("J"));
        classes.insert("K", new Class("K"));
        classes.insert("E", new Class("E", {"A"}, {new Method("void", "methodE")}));
        classes.insert("F", new Class("F", {"A"}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("B", new Class("B", {"G"}, {new Method("void", "methodA"), new Method("void", "methodE")}));
        classes.insert("C", new Class("C", {"F"}));
        classes.insert("D", new Class("D", {"B", "C", "J", "K"}, {new Method("void", "methodE")}));
        QSet<Rhombus> expected = {Rhombus("D", "A", {{1, "B"}, {1, "C"}, {2, "G"}, {2, "F"}, {3, "E"}}, {{"B", {new Method("void", "methodA")}}})};
        QTest::newRow("Rhombus Different Inheritance Depth") << classes << expected;
    }

    // Тест 19: несколько точек слияния с переопределением методов класса top
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA"), new Method("void", "methodB")}));
        classes.insert("M", new Class("M"));
        classes.insert("L", new Class("L"));
        classes.insert("J", new Class("J", {"A"}, {new Method("void", "methodA")}));
        classes.insert("K", new Class("K", {"A"}));
        classes.insert("B", new Class("B", {"J", "K"}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("H", new Class("H", {"E"}, {new Method("void", "methodB")}));
        classes.insert("F", new Class("F", {"H", "G", "M", "L"}));
        QSet<Rhombus> expected = {
            Rhombus("B", "A", {{1, "J"}, {1, "K"}}, {{"J", {new Method("void", "methodA")}}}),
            Rhombus("F", "A", {{1, "H"}, {1, "G"}, {2, "E"}, {3, "C"}, {3, "D"}, {4, "B"}, {5, "J"}, {5, "K"}}, {{"H", {new Method("void", "methodB")}}})
        };
        QTest::newRow("Multiple Merge Points") << classes << expected;
    }

    // Тест 20: цепочка наследования с несколькими точками слияния и с переопределением в различных частях ромбов
    {
        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("void", "methodA")}));
        classes.insert("L", new Class("L"));
        classes.insert("P", new Class("P"));
        classes.insert("J", new Class("J", {"A"}, {new Method("void", "methodA"), new Method("void", "methodJ")}));
        classes.insert("K", new Class("K", {"A"}));
        classes.insert("B", new Class("B", {"J", "K"}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}, {new Method("void", "methodE")}));
        classes.insert("G", new Class("G", {"E"}, {new Method("void", "methodJ")}));
        classes.insert("H", new Class("H", {"E"}, {new Method("void", "methodA")}));
        classes.insert("M", new Class("M", {"E"}, {new Method("void", "methodE")}));
        classes.insert("F", new Class("F", {"H", "G", "M"}));
        QSet<Rhombus> expected = {
            Rhombus("B", "A", {{1, "J"}, {1, "K"}}, {{"J", {new Method("void", "methodA")}}}),
            Rhombus("F", "A", {{1, "H"}, {1, "G"}, {1, "M"}, {2, "E"}, {3, "C"}, {3, "D"}, {4, "B"}, {5, "J"}, {5, "K"}}, {{"H", {new Method("void", "methodA")}}}),
            Rhombus("F", "J", {{1, "H"}, {1, "G"}, {1, "M"}, {2, "E"}, {3, "C"}, {3, "D"}, {4, "B"}}, {{"G", {new Method("void", "methodJ")}}, {"H", {new Method("void", "methodA")}}}),
            Rhombus("F", "E", {{1, "H"}, {1, "G"}, {1, "M"}}, {{"M", {new Method("void", "methodE")}}})
        };
        QTest::newRow("Chain With Multiple Merge Points") << classes << expected;
    }

    // Тест 21: ромб с точкой слияния и перегрузкой метода по константному указателю
    {
        ClassMap classes;

        Parameter p1("int", "a", false, false, false, true), p2("int", "b"), p3("int", "a", true, false, false, true);

        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}, {new Method("int", "method1", {p1, p2})}));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method1", {p3, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method1", {p1, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D", "G", "H"}, {new Method("void", "methodA")}));
        QSet<Rhombus> expected = {Rhombus("D", "A", {{1, "B"}, {1, "C"}}, {{"C", {new Method("int", "method1", {p1, p2})}}})};
        QTest::newRow("Rhombus Overload Const Pointer") << classes << expected;
    }

    // Тест 22: ромб с точкой слияния и переопределениями различных массивов
    {
        ClassMap classes;

        Parameter p1("int", "a", false, false, false, true), p2("int", "b"), p3("int", "a", false, false, false, false, true, true, {-1, 3}),
            p4("int", "a", false, false, false, false, true, false, {-1}), p5("int", "a", false, false, false, false, true, false, {-1, 3});

        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}, {new Method("int", "method1", {p1, p2}), new Method("int", "method2", {p3, p2})}));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method1", {p1, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method1", {p4, p2})}));
        classes.insert("F", new Class("F", {"A"}, {new Method("int", "method2", {p5, p2})}));
        classes.insert("D", new Class("D", {"B", "C", "F"}));
        classes.insert("E", new Class("E", {"D", "G", "H"}, {new Method("void", "methodA")}));

        QMap<QString, QList<Method*>> overriddenMethods;
        overriddenMethods["B"] = {new Method("int", "method1", {p1, p2})};

        QSet<Rhombus> expected = {Rhombus("D", "A", {{1, "B"}, {1, "C"}, {1, "F"}}, overriddenMethods)};

        QTest::newRow("Rhombus Override Multidimensional Arrays") << classes << expected;
    }

    // Тест 23: ромб с точкой слияния и перегрузками с переопределениями методов по количествау параметров и их типам
    {
        ClassMap classes;

        Parameter p1("int", "a"), p2("int", "b"), p3("float", "a"), p4("float", "b"), p5("int", "c"), p6("float", "first"), p7("float", "second");

        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}, {new Method("int", "method1", {p1, p2}), new Method("float", "method2", {p3, p4})}));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method1", {p1, p2, p5})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method1", {p3, p2})}));
        classes.insert("D", new Class("D", {"B", "C", "F"}));
        classes.insert("F", new Class("F", {"A"}, {new Method("float", "method2", {p6, p7})}));
        classes.insert("E", new Class("E", {"D", "G", "H"}, {new Method("void", "methodA")}));

        QSet<Rhombus> expected = {Rhombus("D", "A", {{1, "B"}, {1, "C"}, {1, "F"}}, {{"F", {new Method("float", "method2", {p6, p7})}}})};

        QTest::newRow("Rhombus Overload By Parameters Amounts And Types") << classes << expected;
    }

}

void TestDetectRhomboid::testDetectRhomboidInheritance()
{
    QFETCH(ClassMap, classes);
    QFETCH(QSet<Rhombus>, expected);

    QSet<Rhombus> result = detectRhomboidInheritance(classes);

    if (result != expected) {
        QSet<Rhombus> extraInActual = result - expected; // Элементы, присутствующие только в result
        QSet<Rhombus> extraInExpected = expected - result; // Элементы, присутствующие только в expected

        qDebug() << "Тест провалился:" << "Фактический набор Rhombus не совпадает с ожидаемым";

        if (!extraInActual.isEmpty()) {
            qDebug() << "Дополнительные Rhombus в Actual (result), которых нет в ожидаемом (expected) наборе:";
            for (const Rhombus& r : extraInActual) {
                qDebug() << "  - Bottom:" << r.bottom << ", Top:" << r.top;
                qDebug() << "    Intermediates:" << r.intermediates;
                qDebug() << "    Overridden Methods:" << r.overriddenMethods;
            }
        } else {
            qDebug() << "Пустой узел: нет дополнительных Rhombus в Actual";
        }

        if (!extraInExpected.isEmpty()) {
            qDebug() << "Пропущенные Rhombus в Actual (result), которых нет в ожидаемом (expected) наборе:";
            for (const Rhombus& r : extraInExpected) {
                qDebug() << "  - Bottom:" << r.bottom << ", Top:" << r.top;
                qDebug() << "    Intermediates:" << r.intermediates;
                qDebug() << "    Overridden Methods:" << r.overriddenMethods;
            }
        } else {
            qDebug() << "Пустой узел: нет пропущенных Rhombus в Actual";
        }
    }

    QCOMPARE(result, expected);

    qDeleteAll(classes);
}
