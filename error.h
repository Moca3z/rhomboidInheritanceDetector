/*!
* \file
* \brief Заголовочный файл класса Error.
*/
#ifndef ERROR_H
#define ERROR_H

#include <QString>

/*!
 * \brief Класс для хранения информации об ошибках.
 */
class Error {
public:
    /*!
     * \brief Перечисление типов ошибок
     */
    enum ErrorType {
        NoError, FileNotFound, NoRootTag, MissingClassName, MissingAncestors,
        MissingVirtual, InvalidVirtual, MissingReturnType, MissingMethodName,
        MissingParameters, InvalidParameters, CyclicInheritance, DuplicateClassName
    };
    ErrorType type;
    QString className;
    QString methodName;

    Error(ErrorType t = NoError, const QString& c = "", const QString& m = "")
        : type(t), className(c), methodName(m) {}

    QString message() const {
        switch (type) {
        case FileNotFound: return "Неверно указан файл с входными данными. Возможно, файл не существует";
        case NoRootTag: return "Отсутствует корневой тег program.";
        case MissingClassName: return "В одном из тегов class отсутствует атрибут className.";
        case MissingAncestors: return QString("В классе %1 отсутствует атрибут ancestors.").arg(className);
        case MissingVirtual: return QString("В методе %1 класса %2 отсутствует атрибут virtual.").arg(methodName, className);
        case InvalidVirtual: return QString("В методе %1 класса %2 атрибут virtual имеет некорректные входные данные.").arg(methodName, className);
        case MissingReturnType: return QString("В методе %1 класса %2 отсутствует атрибут returnType.").arg(methodName, className);
        case MissingMethodName: return QString("В одном из методов класса %1 отсутствует атрибут methodName.").arg(className);
        case MissingParameters: return QString("В методе %1 класса %2 отсутствует атрибут parameters.").arg(methodName, className);
        case InvalidParameters: return QString("В методе %1 класса %2 атрибут parameters имеет некорректные входные данные.").arg(methodName, className);
        case CyclicInheritance: return QString("Обнаружено циклическое наследование для класса %1.").arg(className);
        case DuplicateClassName: return QString("Класс с именем %1 объявлен более одного раза.").arg(className);
        default: return "Неизвестная ошибка";
        }
    }
};

#endif // ERROR_H
