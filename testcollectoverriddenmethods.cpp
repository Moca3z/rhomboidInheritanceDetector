#include "testcollectoverriddenmethods.h"
#include "collectoverriddenmethods.h"
#include "class.h"

using InheritanceMatrix = QMap<QString, QSet<QString>>;
using ClassMap = QMap<QString, Class*>;
using MethodMap = QMap<QString, QList<Method*>>;
using IntermediatesMap = QMultiMap<int, QString>;

void TestCollectOverridden::testCollectOverridden_data()
{
    QTest::addColumn<InheritanceMatrix>("inheritanceMatrix");
    QTest::addColumn<ClassMap>("classes");
    QTest::addColumn<QString>("bottomClass");
    QTest::addColumn<QString>("topClass");
    QTest::addColumn<QString>("mergePoint");
    QTest::addColumn<IntermediatesMap>("intermediates");
    QTest::addColumn<MethodMap>("expectedResult");

    // Общий параметр для большинства тестов
    Parameter p1("int", "count");

    // Тест 1: переопределения нет ни в одном потомке top
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "count", {})}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("NoOverriddenMethods") << inheritanceMatrix << classes << "A" << "B" << "Null" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "count", {})}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}, {new Method("Void", "method", {})}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverriddenInBottomOnly") << inheritanceMatrix << classes << "D" << "A" << "Null" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {p1})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "method", {p1})}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        expectedResult["B"] = {new Method("Void", "method", {p1})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverriddenInOneIntermediate") << inheritanceMatrix << classes << "D" << "A" << "Null" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {
                                                   new Method("Void", "method1", {}),
                                                   new Method("Void", "method2", {}),
                                                   new Method("Void", "method3", {})
                                               }));
        classes.insert("B", new Class("B", {"A"}, {
                                                      new Method("Void", "method1", {}),
                                                      new Method("Void", "method3", {})
                                                  }));
        classes.insert("C", new Class("C", {"A"}, {new Method("Void", "method2", {})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        expectedResult["B"] = {new Method("Void", "method1", {}), new Method("Void", "method3", {})};
        expectedResult["C"] = {new Method("Void", "method2", {})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("MultipleMethodsOverriddenInOneClass") << inheritanceMatrix << classes << "D" << "A" << "Null" << intermediates << expectedResult;
    }

    // Тест 5: переопределение происходит и в промежуточном классе, и в bottom
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter pTest5("int", "a");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest5})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "method", {pTest5})}));
        classes.insert("C", new Class("C", {"A"}));
        classes.insert("D", new Class("D", {"B", "C"}, {new Method("Void", "method", {pTest5})}));

        MethodMap expectedResult;
        expectedResult["B"] = {new Method("Void", "method", {pTest5})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverriddenInIntermediateAndBottom") << inheritanceMatrix << classes << "D" << "A" << "Null" << intermediates << expectedResult;
    }

    // Тест 6: отсутствие intermediates
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("D", new Class("D", {"A"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;

        QTest::newRow("NoIntermediates") << inheritanceMatrix << classes << "D" << "A" << "Null" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "method", {})}));
        classes.insert("C", new Class("C", {"B"}, {new Method("Void", "count", {})}));
        classes.insert("D", new Class("D", {"C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "C");
        intermediates.insert(2, "B");

        QTest::newRow("OverriddenInDirectChainNotMergePoint") << inheritanceMatrix << classes << "D" << "A" << "C" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}, {new Method("Void", "method", {})}));
        classes.insert("D", new Class("D", {"C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "C");
        intermediates.insert(2, "B");

        QTest::newRow("OverriddenInDirectChainInMergePoint") << inheritanceMatrix << classes << "D" << "A" << "C" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {p1})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}, {new Method("Void", "count", {p1})}));
        classes.insert("D", new Class("D", {"B", "C"}, {new Method("Void", "method", {p1})}));
        classes.insert("E", new Class("E", {"D"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");

        QTest::newRow("OverriddenInMergePointDirectAncestor") << inheritanceMatrix << classes << "E" << "A" << "D" << intermediates << expectedResult;
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
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}, {new Method("Void", "method", {})}));
        classes.insert("D", new Class("D", {"B", "C"}));
        classes.insert("E", new Class("E", {"D"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");

        QTest::newRow("OverriddenInAncestorsOfMergePoint") << inheritanceMatrix << classes << "E" << "A" << "D" << intermediates << expectedResult;
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

        Parameter pTest11("QString", "a");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest11})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "method", {pTest11})}));
        classes.insert("C", new Class("C", {"B"}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "C");
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");

        QTest::newRow("OverriddenInMergePointDirectDescendant") << inheritanceMatrix << classes << "E" << "A" << "B" << intermediates << expectedResult;
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

        Parameter pTest12("QString", "a");
        Parameter pTest12ForD1("int", "a");
        Parameter pTest12ForD2("int", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest12})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}, {new Method("Void", "method", {pTest12})}));
        classes.insert("D", new Class("D", {"B"}, {new Method("int", "count", {pTest12ForD1, pTest12ForD2})}));
        classes.insert("E", new Class("E", {"C", "D"}));

        MethodMap expectedResult;
        expectedResult["C"] = {new Method("Void", "method", {pTest12})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "C");
        intermediates.insert(1, "D");
        intermediates.insert(2, "B");

        QTest::newRow("OverriddenInDescendantOfMergePoint") << inheritanceMatrix << classes << "E" << "A" << "B" << intermediates << expectedResult;
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

        Parameter pTest13ForC1("float", "a");
        Parameter pTest13For2("int", "b");
        Parameter pTest13ForD1("int", "a");


        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "method", {})}));
        classes.insert("C", new Class("C", {"B"}, {new Method("QString", "sum", {pTest13ForC1, pTest13For2})}));
        classes.insert("D", new Class("D", {"B"}, {new Method("int", "count", {pTest13ForD1, pTest13For2})}));
        classes.insert("E", new Class("E", {"C", "D"}, {new Method("Void", "method", {})}));
        classes.insert("F", new Class("F", {"E"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "E");
        intermediates.insert(2, "C");
        intermediates.insert(2, "D");
        intermediates.insert(3, "B");

        QTest::newRow("OverriddenInTwoMergePoints") << inheritanceMatrix << classes << "F" << "A" << "E" << intermediates << expectedResult;
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

        Parameter pTest14("float", "a");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest14})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}, {new Method("Void", "method", {pTest14})}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("H", new Class("H", {"E"}, {new Method("Void", "method", {pTest14})}));
        classes.insert("F", new Class("F", {"H", "G"}));

        MethodMap expectedResult;
        expectedResult["H"] = {new Method("Void", "method", {pTest14})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "H");
        intermediates.insert(1, "G");
        intermediates.insert(2, "E");
        intermediates.insert(3, "C");
        intermediates.insert(3, "D");
        intermediates.insert(4, "B");

        QTest::newRow("OverriddenInDescendantsOfTwoMergePoints") << inheritanceMatrix << classes << "F" << "A" << "E" << intermediates << expectedResult;
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

        Parameter pTest15("float", "a");

        Parameter pTest15ForH1("int", "a");
        Parameter pTest15ForH2("int", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest15})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}, {new Method("Void", "count", {pTest15})}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}, {new Method("Void", "method", {pTest15})}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("H", new Class("H", {"E"}, {new Method("Void", "findMax", {pTest15ForH1, pTest15ForH2})}));
        classes.insert("F", new Class("F", {"B", "H", "G"}));

        MethodMap expectedResult;
        expectedResult["E"] = {new Method("Void", "method", {pTest15})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "H");
        intermediates.insert(1, "G");
        intermediates.insert(2, "E");
        intermediates.insert(3, "C");
        intermediates.insert(3, "D");
        intermediates.insert(1, "B");

        QTest::newRow("OverriddenInBypassableMergePoint") << inheritanceMatrix << classes << "F" << "A" << "B" << intermediates << expectedResult;
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

        Parameter pTest16("float", "a");

        Parameter pTest16ForE1("int", "a");
        Parameter pTest16ForE2("int", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest16})}));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"B"}, {new Method("Void", "method", {pTest16})}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"C", "D"}, {new Method("Void", "findMax", {pTest16ForE1, pTest16ForE2})}));
        classes.insert("G", new Class("G", {"E"}));
        classes.insert("H", new Class("H", {"E"}, {new Method("Void", "method", {pTest16})}));
        classes.insert("F", new Class("F", {"B", "H", "G"}));

        MethodMap expectedResult;
        expectedResult["C"] = {new Method("Void", "method", {pTest16})};
        expectedResult["H"] = {new Method("Void", "method", {pTest16})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "H");
        intermediates.insert(1, "G");
        intermediates.insert(2, "E");
        intermediates.insert(3, "C");
        intermediates.insert(3, "D");
        intermediates.insert(1, "B");

        QTest::newRow("OverriddenInAncestorsAndDescendantsOfBypassableMergePoint") << inheritanceMatrix << classes << "F" << "A" << "B" << intermediates << expectedResult;
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

        Parameter pTest17("float", "a");

        Parameter pTest17For1("int", "a");
        Parameter pTest17ForE2("int", "b");


        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("Void", "method", {pTest17})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("Void", "count", {pTest17For1})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("Void", "method", {pTest17})}));
        classes.insert("D", new Class("D", {"B"}));
        classes.insert("E", new Class("E", {"B", "C"}, {new Method("Void", "findMax", {pTest17For1, pTest17ForE2})}));
        classes.insert("F", new Class("F", {"D", "E"}));

        MethodMap expectedResult;
        expectedResult["C"] = {new Method("Void", "method", {pTest17})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(1, "E");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");

        QTest::newRow("OverriddenInOneOfTwoLinkedRhombuses") << inheritanceMatrix << classes << "F" << "A" << "Null" << intermediates << expectedResult;
    }

    // Тест 18: переопределение различных методов в нескольких промежуточных классах
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

        Parameter pTest18Count("float", "a");
        Parameter pTest18Init("QString", "a");

        Parameter pTest18For1("int", "a");
        Parameter pTest18ForE2("int", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {
                                                   new Method("int", "count", {pTest18Count}),
                                                   new Method("Void", "method", {}),
                                                   new Method("QList", "init", {pTest18Init})
                                               }));
        classes.insert("B", new Class("B", {"A"}));
        classes.insert("C", new Class("C", {"A"}, {new Method("Void", "method", {})}));
        classes.insert("D", new Class("D", {"B"}, {new Method("int", "count", {pTest18Count})}));
        classes.insert("E", new Class("E", {"B", "C"}, {new Method("void", "findMax", {pTest18For1, pTest18ForE2})}));
        classes.insert("G", new Class("G", {"I"}, {new Method("Void", "remove", {pTest18For1})}));
        classes.insert("H", new Class("H", {"I"}));
        classes.insert("I", new Class("I", {"A"}));
        classes.insert("K", new Class("K", {"A"}, {new Method("QList", "init", {pTest18Init})}));
        classes.insert("F", new Class("F", {"D", "E", "K", "G", "H"}));

        MethodMap expectedResult;
        expectedResult["D"] = {new Method("int", "count", {pTest18Count})};
        expectedResult["C"] = {new Method("Void", "method", {})};
        expectedResult["K"] = {new Method("QList", "init", {pTest18Init})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "D");
        intermediates.insert(1, "E");
        intermediates.insert(1, "K");
        intermediates.insert(1, "G");
        intermediates.insert(1, "H");
        intermediates.insert(2, "B");
        intermediates.insert(2, "C");
        intermediates.insert(2, "I");

        QTest::newRow("MultipleMethodsInMultipleIntermediates") << inheritanceMatrix << classes << "F" << "A" << "Null" << intermediates << expectedResult;
    }

    // Тест 19: перегрузка метода top по типу данных параметра
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter pTest19For1("int", "a"), pTest19ForA2("int", "b"), pTest19ForC2("float", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method", {pTest19For1, pTest19ForA2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("float", "count", {pTest19ForC2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("float", "method", {pTest19For1, pTest19ForC2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverloadByParameterDataType") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }


    // Тест 20: перегрузка метода top по количеству параметров
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p1("int", "a"), p2("int", "b"), p3("int", "c"), p4("float", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method", {p1, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("float", "count", {p4})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method", {p1, p2, p3})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverloadByParameterCounter") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 21: переопределение метода top по константному параметру
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p1("int", "a"), p2("int", "b"), p1Const("int", "c", true), p4("float", "b");

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method", {p1, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("float", "count", {p4})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method", {p1Const, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        expectedResult["C"] = {new Method("int", "method", {p1Const, p2})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverrideByConstParameter") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 22: перегрузка методов top по указателям и ссылкам
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p1("int", "a"), p2("int", "b"),
                  p1Reference("int", "a", false, false, true), p1Pointer("int", "a", false, false, false, true);

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method1", {p1, p2}),
                                                new Method("int", "method2", {p1, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method2", {p1Reference, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method1", {p1Pointer, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverloadByPointersAndReferences") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 23: переопределение методов top по указателям и ссылкам
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p2("int", "b"),
            p1Reference("int", "a", false, false, true), p1Pointer("int", "a", false, false, false, true);

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method1", {p1Pointer, p2}),
                                                new Method("int", "method2", {p1Reference, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method2", {p1Reference, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method1", {p1Pointer, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        expectedResult["B"] = {new Method("int", "method2", {p1Reference, p2})};
        expectedResult["C"] = {new Method("int", "method1", {p1Pointer, p2})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverrideByPointerdAndeReferences") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 24: перегрузка методов top по константным указателям и ссылкам
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p2("int", "b"),
            p1Reference("int", "a", false, false, true), p1Pointer("int", "a", false, false, false, true),
            p1ConstReference("int", "a", true, false, true), p1ConstPointer("int", "a", true, false, false, true);

        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method1", {p1Pointer, p2}),
                                                new Method("int", "method2", {p1Reference, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method2", {p1ConstReference, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method1", {p1ConstPointer, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverloadByConstPointersAndReferences") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 25: переопределение метода top с параметром в виде одномерного массива
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p1Array("int", "a", false, false, false, false, true),
            p1Pointer("int", "a", false, false, false, true), p2("int", "b");


        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method", {p1Array, p2})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method", {p1Array, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method", {p1Pointer, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        expectedResult["B"] = {new Method("int", "method", {p1Array, p2})};
        expectedResult["C"] = {new Method("int", "method", {p1Pointer, p2})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverrideByOneDimensionalArray") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 26: перегрузки по применению const к указателю или типу
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "A"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"A", {}}
        };

        Parameter p1A("int", "a", true, true, false, true),
            p1B("int", "a", true, false, false, true), p1C("int", "a", false, true, false, true), p2("int", "b");


        ClassMap classes;
        classes.insert("A", new Class("A", {}, {new Method("int", "method", {p1A})}));
        classes.insert("B", new Class("B", {"A"}, {new Method("int", "method", {p1B, p2})}));
        classes.insert("C", new Class("C", {"A"}, {new Method("int", "method", {p1C, p2})}));
        classes.insert("D", new Class("D", {"B", "C"}));

        MethodMap expectedResult;
        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");

        QTest::newRow("OverloadByMultiConst") << inheritanceMatrix << classes << "D" << "A" << "" << intermediates << expectedResult;
    }

    // Тест 27: переопределение методов с параметрами в виде многомерного массива
    {
        InheritanceMatrix inheritanceMatrix = {
            {"D", {"B", "C", "E", "F", "G"}},
            {"B", {"A"}},
            {"C", {"A"}},
            {"E", {"A"}},
            {"F", {"A"}},
            {"G", {"A"}},
            {"A", {}}
        };

        ClassMap classes;
        // Класс A с исходными методами
        classes.insert("A", new Class("A", {}, {
                                                   new Method("int", "sum", {Parameter("int", "a", false, false, false, false, true, false, {-1, 10, 5, 4, 3})}),
                                                   new Method("int", "count", {Parameter("int", "a", false, false, false, false, true, false, {-1, 3})})
                                               }));
        // Класс B переопределяет sum
        classes.insert("B", new Class("B", {"A"}, {
                           new Method("int", "sum", {Parameter("int", "sum", false, false, false, false, true, false, {-1, 10, 5, 4, 3})})
                       }));
        // Класс C переопределяет sum с указателем на массив
        classes.insert("C", new Class("C", {"A"}, {
                           new Method("int", "sum", {Parameter("int", "sum", false, false, false, false, true, true, {-1, 10, 5, 4, 3})})
                       }));
        // Класс E переопределяет sum с указателем на массив
        classes.insert("E", new Class("E", {"A"}, {
                           new Method("int", "sum", {Parameter("int", "sum", false, false, false, false, true, true, {-1, 10, 5, 8, 3})})
                       }));
        // Класс F переопределяет count
        classes.insert("F", new Class("F", {"A"}, {
                           new Method("int", "count", {Parameter("int", "count", false, false, false, false, true, false, {-1, 3})})
                       }));
        // Класс G переопределяет count с указателем на массив
        classes.insert("G", new Class("G", {"A"}, {
                           new Method("int", "count", {Parameter("int", "count", false, false, false, false, true, true, {-1, 3})})
                       }));
        classes.insert("D", new Class("D", {"B", "C", "E", "F", "G"}));

        MethodMap expectedResult;
        expectedResult["B"] = {new Method("int", "sum", {Parameter("int", "a", false, false, false, false, true, false, {-1, 10, 5, 4, 3})})};
        expectedResult["C"] = {new Method("int", "sum", {Parameter("int", "a", false, false, false, false, true, true, {-1, 10, 5, 4, 3})})};
        expectedResult["F"] = {new Method("int", "count", {Parameter("int", "a", false, false, false, false, true, false, {-1, 3})})};
        expectedResult["G"] = {new Method("int", "count", {Parameter("int", "a", false, false, false, false, true, true, {-1, 3})})};

        IntermediatesMap intermediates;
        intermediates.insert(1, "B");
        intermediates.insert(1, "C");
        intermediates.insert(1, "E");
        intermediates.insert(1, "F");
        intermediates.insert(1, "G");

        QTest::newRow("OverriddenMethodsWithMultiDimensionalArrays") << inheritanceMatrix << classes << "D" << "A" << "Null" << intermediates << expectedResult;
    }

}

void TestCollectOverridden::testCollectOverridden()
{
    QFETCH(InheritanceMatrix, inheritanceMatrix);
    QFETCH(ClassMap, classes);
    QFETCH(QString, bottomClass);
    QFETCH(QString, topClass);
    QFETCH(QString, mergePoint);
    QFETCH(IntermediatesMap, intermediates);
    QFETCH(MethodMap, expectedResult);

    MethodMap result = collectOverriddenMethods(bottomClass, topClass, mergePoint, intermediates, classes, inheritanceMatrix);

    QVERIFY2(result.size() == expectedResult.size(), qPrintable(QString("Expected result size %1, but got %2").arg(expectedResult.size()).arg(result.size())));

    for (const QString& key : expectedResult.keys()) {
        QVERIFY2(result.contains(key), qPrintable(QString("Result does not contain expected key: %1").arg(key)));
        QVERIFY2(result[key].size() == expectedResult[key].size(), qPrintable(QString("For key %1, expected %2 methods, but got %3").arg(key).arg(expectedResult[key].size()).arg(result[key].size())));

        for (int i = 0; i < expectedResult[key].size(); ++i) {
            QVERIFY2(result[key][i]->methodName == expectedResult[key][i]->methodName,
                     qPrintable(QString("For key %1, expected method %2, but got %3").arg(key).arg(expectedResult[key][i]->methodName).arg(result[key][i]->methodName)));
        }
    }

    // Очистка памяти для expectedResult
    for (const QString& key : expectedResult.keys()) {
        for (Method* method : expectedResult[key]) {
            delete method;
        }
    }

    // Очистка памяти для classes (деструктор Class позаботится об освобождении методов)
    qDeleteAll(classes);
}
