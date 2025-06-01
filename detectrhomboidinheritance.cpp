#include "detectrhomboidinheritance.h"


QSet<Rhombus> detectRhomboidInheritance(const QMap<QString, Class*>& classes) {
    Q_UNUSED(classes);

    // Заглушка: возвращаем заведомо неверный результат
    Rhombus fakeRhombus;
    fakeRhombus.bottom = "FakeBottom";
    fakeRhombus.top = "FakeTop";
    fakeRhombus.intermediates = {"FakeIntermediate"};

    // Создаем некорректный метод
    Method* fakeMethod = new Method();
    fakeMethod->methodName = "FAKE_METHOD";
    fakeMethod->isVirtual = false;
    fakeRhombus.overriddenMethods["FakeClass"] = {fakeMethod};

    return {fakeRhombus};
}
