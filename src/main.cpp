#include "ThreadController.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<ThreadController>("ThreadingDemo", 1, 0, "ThreadController");

    engine.loadFromModule("ThreadingDemo.UI", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
