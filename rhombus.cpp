// rhombus.cpp
#include "rhombus.h"

QList<Parameter> parseParameters(const QString& parametersString)
{
    Q_UNUSED(parametersString); // Игнорируем входные данные

    QList<Parameter> fakeParameters;
    Parameter fake;
    fake.type = "fake";
    fake.name = "fake";
    fake.isTypeConst = false;
    fake.isPointerConst = false;
    fake.isReference = false;
    fake.isPointer = false;
    fake.isArray = false;
    fake.isPointerToArray = false;
    fake.arrayDimensions.append(0); // Некорректное значение для размеров
    fakeParameters.append(fake);

    return fakeParameters;
}
