/*!
* \file
* \brief Данный файл содержит главную функцию программы rhomboidInheritanceDetector.
*
* \mainpage Документация для программы "rhomboidInheritanceDetector"
Программа предназначена для поиска случаев ромбовидного наследования в наборе классов C++.
Для функционирования программы необходима операционная система Windows 7 или выше.
Программа разработана на языке С++ с использованием стандартных библиотек C++, библиотеки Qt и фреймворка Qt Creator.
Программа должна получать один параметр командной строки: имя входного файла с данными о наборе классов и их методах и имя двух выходных файлов:
первый – входной набор классов в форме графа, второй - папка с  графами, в которых возникло ромбовидное наследование.
Либо программа может работать без дополнительных аргументов командной строки - для запуска модульных тестов.

Пример команды запуска программы:
* \code
rhomboidInheritanceDetector.exe testxml.txt output.dot rhombus_diagrams/
* \endcode

* \author Полежаев Александр
* \date 19 Июня 2025
* \version 1.0
*/
#include <QCoreApplication>
#include "testFindIntermediates.h"
#include "testfindmergepoint.h"
#include "testcollectoverriddenmethods.h"
#include "testdetectrhomboidinheritance.h"
#include "testparseparameters.h"
#include "testcheckforoverriddenparameters.h"
#include <QDebug>
#include "rhombus.h"
#include <QTextStream>
#include <clocale>
#include "detectrhomboidinheritance.h"
#include "classdiagram.h"
#include <QDir>
#include <QFile>

int runTests() {
    TestFindIntermediates testsFindIntermediates;
    TestFindMergePoint testsFindMergePoint;
    TestCollectOverridden testsCollectOverriddenMethods;
    TestDetectRhomboid testsDetectRhomboidInheritance;
    TestParseParameters testsParseParameters;
    TestCheckForOverriddenParameters testsCheckForOverriddenParameters;

    QTest::qExec(&testsFindIntermediates);
    QTest::qExec(&testsFindMergePoint);
    QTest::qExec(&testsParseParameters);
    QTest::qExec(&testsCheckForOverriddenParameters);
    QTest::qExec(&testsCollectOverriddenMethods);
    QTest::qExec(&testsDetectRhomboidInheritance);

    return 0;
}

/*!
 * \brief Главная функция программы BracketExpansion
 * \param [in] argc - количество переданных аргументов командной строки
 * \param [in] argv - переданные аргументы командной строки
 * \param [in] argv[0] - аргумент запуска программы
 * \param [in] argv[1] - путь к входному xml файлу
 * \param [in] argv[2] - путь к преобразованному xml файлу в входной набор классов в форме графа
 * \param [in] argv[3] - путь к папке с графами, в которых возникло ромбовидное наследование
 * \return 0 - программа завершилась успешно; 1 - была найдена ошибка
 */
int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Если аргументов нет - запускаем тесты
        return runTests();
    }

    // Основная логика программы
    setlocale(LC_ALL, "Russian_Russia.1251");
    QCoreApplication app(argc, argv);

    // 1. Проверка аргументов командной строки
    if (argc != 4 && argc != 1) {
        qCritical() << "Ошибка: Неверное количество аргументов";
        qCritical() << "Использование:" << argv[0]
                    << "<input.xml> <output.dot> <rhombus_output_dir>";
        return 1;
    }

    const QString inputFile = argv[1];
    const QString outputFile = argv[2];
    const QString rhombusOutputDir = argv[3];

    // 2. Разбор XML-файла
    qDebug() << "Парсинг файла:" << inputFile;
    auto [classes, errors] = parseXmlFile(inputFile);

    if (!errors.isEmpty()) {
        qCritical() << "Ошибки при парсинге:";
        for (const Error& error : errors) {
            qCritical() << error.message();
        }
        qDeleteAll(classes);
        return 1;
    }

    // 3. Создание полной диаграммы классов
    ClassDiagram fullDiagram(classes);

    // 4. Генерация и сохранение основного DOT-файла
    qDebug() << "Генерация полной диаграммы классов...";
    QString fullDot = fullDiagram.generateDotFile();
    QFile dotFile(outputFile);
    if (!dotFile.open(QIODevice::WriteOnly)) {
        qCritical() << "Ошибка при открытии файла для записи:" << outputFile;
        qDeleteAll(classes);
        return 1;
    }
    dotFile.write(fullDot.toUtf8());
    dotFile.close();
    qDebug() << "Полная диаграмма сохранена в:" << outputFile;

    // 5. Поиск ромбовидного наследования
    qDebug() << "Поиск ромбовидного наследования...";
    QSet<Rhombus> rhombuses = detectRhomboidInheritance(classes);

    // 6. Обработка результатов поиска
    if (rhombuses.isEmpty()) {
        qDebug() << "Ромбовидное наследование не обнаружено.";
    } else {
        qDebug() << "Найдено ромбовидных структур:" << rhombuses.size();

        // Создание директории для ромбовидных диаграмм
        QDir dir(rhombusOutputDir);
        if (!dir.exists() && !dir.mkpath(".")) {
            qCritical() << "Ошибка при создании директории:" << rhombusOutputDir;
            qDeleteAll(classes);
            return 1;
        }

        // 7. Генерация диаграмм для каждого ромба
        int rhombusCount = 1;
        for (const Rhombus& rhombus : rhombuses) {
            qDebug() << "\nРомб #" << rhombusCount;
            qDebug() << "Вершина (top):" << rhombus.top;
            qDebug() << "Основание (bottom):" << rhombus.bottom;
            qDebug() << "Промежуточные классы:" << rhombus.intermediates.values().join(", ");

            // Создание специализированной диаграммы
            ClassDiagram rhombusDiagram(rhombus, classes);
            QString rhombusDot = rhombusDiagram.generateDotFile();

            // Сохранение в файл
            QString filename = QString("%1/rhombus_%2.dot").arg(rhombusOutputDir).arg(rhombusCount++);
            QFile rhombusFile(filename);
            if (!rhombusFile.open(QIODevice::WriteOnly)) {
                qCritical() << "Ошибка при записи файла:" << filename;
                continue;
            }
            rhombusFile.write(rhombusDot.toUtf8());
            rhombusFile.close();
            qDebug() << "Диаграмма сохранена в:" << filename;
        }
    }

    // 8. Очистка памяти
    qDeleteAll(classes);
    qDebug() << "Программа завершена успешно.";

    return 0;
}
