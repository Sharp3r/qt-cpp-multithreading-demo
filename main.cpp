#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "threadcontroller.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ThreadController>("ThreadingDemo", 1, 0, "ThreadController");

    ThreadController controller;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller", &controller);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
