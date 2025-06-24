/*!
 * \file
 * \brief Файл содержит реализацию тестов для функции findMergePoint.
 */
#include "testfindmergepoint.h"
#include "findmergepoint.h"
#include "class.h"

// Алиасы для работы QFetch
using InheritanceMatrix = QMap<QString, QSet<QString>>;
using ClassMap = QMap<QString, Class*>;
using IntermediatesMap = QMultiMap<int, QString>;

void TestFindMergePoint::testFindMergePoint_data()
{
    QTest::addColumn<QString>("bottom");
    QTest::addColumn<QString>("top");
    QTest::addColumn<IntermediatesMap>("intermediates");
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");
        QTest::newRow("TwoClassesSameLevel") << "D" << "A" << intermediates << inheritanceMatrix << classes << "";
    }

    // Тест 2: ромб из трех классов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"C", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B", "A"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        QTest::newRow("ThreeClassDiamond") << "C" << "A" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("D", new Class("D", {"A"}));
        classes.insert("C", new Class("C", {"B", "A", "D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "D");
        QTest::newRow("FullConnectionToBottom") << "C" << "A" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B", "A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");
        QTest::newRow("AdditionalInheritanceIntermediate") << "D" << "A" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        QTest::newRow("MergePointDirectAncestorBottom") << "E" << "A" << intermediates << inheritanceMatrix << classes << "D";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");
        intermediates.insert(2, "A");
        QTest::newRow("MergePointDirectDescendantTop") << "D" << "X" << intermediates << inheritanceMatrix << classes << "A";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("E", new Class("E", {"A"}));
        classes.insert("F", new Class("F", {"A"}));
        classes.insert("D", new Class("D", {"B", "C", "E", "F"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");
        intermediates.insert(1, "E");
        intermediates.insert(1, "F");
        intermediates.insert(2, "A");
        QTest::newRow("MergePointDirectDescendantTopMultipleIntermediates") << "D" << "X" << intermediates << inheritanceMatrix << classes << "A";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C", "X"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");
        intermediates.insert(2, "A");
        QTest::newRow("BypassPotentialMergePointDirectDescendantTop") << "D" << "X" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"A", "D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        QTest::newRow("BypassPotentialMergePointDirectAncestorBottom") << "E" << "A" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        intermediates.insert(3, "A");
        QTest::newRow("TwoMergePoints") << "E" << "X" << intermediates << inheritanceMatrix << classes << "D";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"A", "D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        intermediates.insert(1, "A");
        QTest::newRow("BypassFirstMergePointToSecond") << "E" << "X" << intermediates << inheritanceMatrix << classes << "A";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D", "B"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(1, "B");
        intermediates.insert(2, "C");
        intermediates.insert(2, "A");
        QTest::newRow("BypassFirstMergePointToIntermediate") << "E" << "X" << intermediates << inheritanceMatrix << classes << "A";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D", "X"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        intermediates.insert(3, "A");
        QTest::newRow("BypassBothMergePointsSimultaneously") << "E" << "X" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"A", "D", "X"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        intermediates.insert(1, "A");
        QTest::newRow("BypassBothMergePointsMultiplePaths") << "E" << "X" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("F", new Class("F", {"D"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("G", new Class("G", {"F"}));
        classes.insert("E", new Class("E", {"C", "G"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "C");
        intermediates.insert(1, "G");
        intermediates.insert(2, "F");
        intermediates.insert(3, "D");
        intermediates.insert(4, "B");
        QTest::newRow("DifferentInheritanceDepthOneBranch") << "E" << "A" << intermediates << inheritanceMatrix << classes << "";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("F", new Class("F", {"D"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("G", new Class("G", {"C", "F"}));
        classes.insert("E", new Class("E", {"G"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "G");
        intermediates.insert(2, "C");
        intermediates.insert(2, "F");
        intermediates.insert(3, "D");
        intermediates.insert(4, "B");
        QTest::newRow("DifferentInheritanceDepthAfterMergePoint") << "E" << "A" << intermediates << inheritanceMatrix << classes << "G";
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("H", new Class("H", {"A"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("C", new Class("C", {"H"}));
        classes.insert("F", new Class("F", {"D"}));
        classes.insert("G", new Class("G", {"F", "C"}));
        classes.insert("E", new Class("E", {"G"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "G");
        intermediates.insert(2, "F");
        intermediates.insert(2, "C");
        intermediates.insert(3, "D");
        intermediates.insert(3, "H");
        intermediates.insert(4, "B");
        intermediates.insert(4, "A");
        QTest::newRow("DifferentInheritanceDepthTwoMergePoints") << "E" << "X" << intermediates << inheritanceMatrix << classes << "G";
    }

    // Тест 18: отсутствие intermediates
    {
        InheritanceMatrix inheritanceMatrix = {
            {"B", {"A"}},
            {"A", {}}
        };
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        QTest::newRow("NoIntermediates") << "B" << "A" << IntermediatesMap() << inheritanceMatrix << classes << "";
    }

    // Тест 19: только один класс intermediates
    {
        InheritanceMatrix inheritanceMatrix = {
            {"C", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        QTest::newRow("SingleIntermediate") << "B" << "A" << intermediates << inheritanceMatrix << classes << "B";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"C"}));
        classes.insert("E", new Class("E", {"D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "C");
        intermediates.insert(3, "B");
        QTest::newRow("StraightInheritanceBranch") << "E" << "A" << intermediates << inheritanceMatrix << classes << "D";
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"B"}));
        classes.insert("F", new Class("F", {"E", "D"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "E");
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        QTest::newRow("TwoConnectedDiamonds") << "F" << "A" << intermediates << inheritanceMatrix << classes << "";
    }

    // Тест 22: цепочка точек слияния
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}));
        classes.insert("F", new Class("F", {"E"}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("H", new Class("H", {"F", "G"}));
        classes.insert("I", new Class("I", {"H"}));
        classes.insert("J", new Class("J", {"H"}));
        classes.insert("K", new Class("K", {"I", "J"}));
        classes.insert("P", new Class("P", {"K"}));
        classes.insert("O", new Class("O", {"K"}));
        classes.insert("R", new Class("R", {"P", "O"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "P");
        intermediates.insert(1, "O");
        intermediates.insert(2, "K");
        intermediates.insert(3, "I");
        intermediates.insert(3, "J");
        intermediates.insert(4, "H");
        intermediates.insert(5, "F");
        intermediates.insert(5, "G");
        intermediates.insert(6, "E");
        intermediates.insert(7, "C");
        intermediates.insert(7, "D");
        intermediates.insert(8, "B");
        QTest::newRow("ChainOfMergePoints") << "R" << "A" << intermediates << inheritanceMatrix << classes << "K";
    }

    // Тест 23: цепочка связанных друг с другом ромбов
    {
        InheritanceMatrix inheritanceMatrix = {
            {"O", {"P", "K", "I", "J", "H", "F", "G", "E", "D", "C", "B", "A"}},
            {"P", {"I", "J", "F", "G", "E", "C", "B", "A"}},
            {"K", {"J", "H", "G", "E", "D", "C", "B", "A"}},
            {"J", {"G", "E", "C", "B", "A"}},
            {"I", {"F", "G", "C", "A"}},
            {"H", {"E", "D", "B", "A"}},
            {"F", {"C", "A"}},
            {"G", {"C", "A"}},
            {"E", {"B", "A"}},
            {"C", {"A"}},
            {"D", {"B", "A"}},
            {"B", {"A"}},
            {"A", {}}
        };
        ClassMap classes;
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"B"}));
        classes.insert("F", new Class("F", {"C"}));
        classes.insert("G", new Class("G", {"C"}));
        classes.insert("H", new Class("H", {"E", "D"}));
        classes.insert("I", new Class("I", {"F", "G"}));
        classes.insert("J", new Class("J", {"G", "E"}));
        classes.insert("K", new Class("K", {"J", "H"}));
        classes.insert("P", new Class("P", {"I", "J"}));
        classes.insert("O", new Class("O", {"P", "K"}));
        IntermediatesMap intermediates;
        intermediates.insert(1, "P");
        intermediates.insert(1, "K");
        intermediates.insert(2, "I");
        intermediates.insert(2, "J");
        intermediates.insert(2, "H");
        intermediates.insert(3, "F");
        intermediates.insert(3, "G");
        intermediates.insert(3, "E");
        intermediates.insert(3, "D");
        intermediates.insert(4, "C");
        intermediates.insert(4, "B");
        QTest::newRow("ChainOfRelatedRhombuses") << "O" << "A" << intermediates << inheritanceMatrix << classes << "";
    }
}

void TestFindMergePoint::testFindMergePoint()
{
    QFETCH(QString, bottom);
    QFETCH(QString, top);
    QFETCH(IntermediatesMap, intermediates);
    QFETCH(InheritanceMatrix, inheritanceMatrix);
    QFETCH(ClassMap, classes);
    QFETCH(QString, expected);

    QString result = findMergePoint(bottom, top, intermediates, inheritanceMatrix, classes);
    QCOMPARE(result, expected);

    qDeleteAll(classes);
}
