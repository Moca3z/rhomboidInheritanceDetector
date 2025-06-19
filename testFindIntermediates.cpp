/*!
 * \file
 * \brief Файл содержит реализацию тестов для функции findIntermediates.
 */
#include "testfindintermediates.h"
#include "findintermediates.h"
#include "class.h"

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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B"));
        classes.insert("D", new Class("D", {"A", "B"}));
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
        classes.insert("F", new Class("F"));
        classes.insert("A", new Class("A", {"F"}));
        classes.insert("C", new Class("C"));
        classes.insert("E", new Class("E"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("D", new Class("D", {"B", "C", "E"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("H", new Class("H"));
        classes.insert("G", new Class("G"));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("B", new Class("B", {"C"}));
        classes.insert("E", new Class("E", {"B", "F", "G"}));
        classes.insert("D", new Class("D", {"E"}));
        classes.insert("F", new Class("F", {"H"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("C", new Class("C", {"E"}));
        classes.insert("B", new Class("B", {"E"}));
        classes.insert("E", new Class("E"));
        classes.insert("D", new Class("D", {"A", "B", "C"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"A", "B", "C"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B"));
        classes.insert("C", new Class("C"));
        classes.insert("G", new Class("G", {"A"}));
        classes.insert("E", new Class("E", {"A"}));
        classes.insert("H", new Class("H", {"G", "E", "B", "C"}));
        classes.insert("I", new Class("I", {"H"}));
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
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("C", new Class("C"));
        classes.insert("F", new Class("F"));
        classes.insert("X", new Class("X"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("G", new Class("G", {"B", "C"}));
        classes.insert("E", new Class("E", {"B", "F"}));
        classes.insert("I", new Class("I", {"G", "E"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("I", new Class("I"));
        classes.insert("J", new Class("J"));
        classes.insert("G", new Class("G"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C", "H"}));
        classes.insert("E", new Class("E", {"B", "G"}));
        classes.insert("F", new Class("F", {"E", "D", "I", "J"}));
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
        classes.insert("M", new Class("M"));
        classes.insert("L", new Class("L"));
        classes.insert("X", new Class("X"));
        classes.insert("Q", new Class("Q"));
        classes.insert("Y", new Class("Y"));
        classes.insert("Z", new Class("Z"));
        classes.insert("A", new Class("A", {"Y"}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"B", "Y"}));
        classes.insert("E", new Class("E", {"C", "D", "Q"}));
        classes.insert("F", new Class("F", {"E"}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("H", new Class("H", {"F", "G", "X"}));
        classes.insert("I", new Class("I", {"H"}));
        classes.insert("J", new Class("J", {"H"}));
        classes.insert("K", new Class("K", {"I", "J"}));
        classes.insert("P", new Class("P", {"K"}));
        classes.insert("O", new Class("O", {"K"}));
        classes.insert("R", new Class("R", {"P", "O", "M", "L"}));
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
        classes.insert("C", new Class("C"));
        classes.insert("E", new Class("E"));
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"A", "C"}));
        classes.insert("D", new Class("D", {"A", "B", "E"}));
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
        classes.insert("N", new Class("N"));
        classes.insert("O", new Class("O"));
        classes.insert("L", new Class("L"));
        classes.insert("M", new Class("M"));
        classes.insert("A", new Class("A"));
        classes.insert("B", new Class("B", {"X", "A"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("H", new Class("H", {"X"}));
        classes.insert("F", new Class("F", {"D", "L", "M"}));
        classes.insert("C", new Class("C", {"H"}));
        classes.insert("G", new Class("G", {"C", "F"}));
        classes.insert("X", new Class("X", {"A"}));
        classes.insert("E", new Class("E", {"A", "G", "N", "O"}));
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
        classes.insert("A", new Class("A"));
        classes.insert("F", new Class("F"));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"F", "A"}));
        classes.insert("D", new Class("D", {"F"}));
        classes.insert("C", new Class("C", {"A", "H"}));
        classes.insert("G", new Class("G", {"H"}));
        classes.insert("E", new Class("E", {"D", "B", "C", "G"}));
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("F", new Class("F", {"X"}));
        classes.insert("H", new Class("H", {"X"}));
        classes.insert("B", new Class("B", {"F", "A"}));
        classes.insert("D", new Class("D", {"F"}));
        classes.insert("C", new Class("C", {"A", "H"}));
        classes.insert("G", new Class("G", {"H"}));
        classes.insert("E", new Class("E", {"D", "B", "C", "G"}));
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
        classes.insert("X", new Class("X"));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("F", new Class("F", {"X"}));
        classes.insert("H", new Class("H", {"X"}));
        classes.insert("B", new Class("B", {"F", "A"}));
        classes.insert("D", new Class("D", {"F"}));
        classes.insert("C", new Class("C", {"A", "H"}));
        classes.insert("G", new Class("G", {"H"}));
        classes.insert("E", new Class("E", {"D", "B", "C", "G"}));
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
        classes.insert("X", new Class("X"));
        classes.insert("I", new Class("I"));
        classes.insert("J", new Class("J", {"I"}));
        classes.insert("K", new Class("K", {"I"}));
        classes.insert("A", new Class("A", {"X"}));
        classes.insert("F", new Class("F", {"X"}));
        classes.insert("H", new Class("H"));
        classes.insert("B", new Class("B", {"F", "A"}));
        classes.insert("D", new Class("D", {"F"}));
        classes.insert("C", new Class("C", {"A", "H"}));
        classes.insert("G", new Class("G", {"H"}));
        classes.insert("E", new Class("E", {"D", "B", "C", "G", "J", "K"}));
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

    // Получаем уникальные ключи
    QList<int> resultKeys = result.uniqueKeys();
    QList<int> expectedKeys = expected.uniqueKeys();

    // Сравниваем наборы ключей
    QCOMPARE(resultKeys, expectedKeys);

    // Для каждого ключа сравниваем отсортированные списки значений
    for (int key : resultKeys) {
        QList<QString> resultValues = result.values(key);
        QList<QString> expectedValues = expected.values(key);

        // Сортируем списки для корректного сравнения
        std::sort(resultValues.begin(), resultValues.end());
        std::sort(expectedValues.begin(), expectedValues.end());

        QCOMPARE(resultValues, expectedValues);
    }

    qDeleteAll(classes);
}
