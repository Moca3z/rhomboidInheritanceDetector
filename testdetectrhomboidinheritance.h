#ifndef TESTDETECTRHOMBOIDINHERITANCE_H
#define TESTDETECTRHOMBOIDINHERITANCE_H

#include <QObject>
#include <QTest>

class TestDetectRhomboid : public QObject
{
    Q_OBJECT
private slots:
    void testDetectRhomboidInheritance_data();
    void testDetectRhomboidInheritance();


};

#endif // TESTDETECTRHOMBOIDINHERITANCE_H

/*
 * \brief находит случаи ромбовидного наследования в наборе классов
 * \param [in] classes - набор классов, каждый из которых хранит в себе информацию о методах и родителях
 * \return набор ромбов, в которых возникло ромбовидное наследование
*/
