#include "worker_thread.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    auto* worker = new WorkerThread("test.txt");

    QObject::connect(worker, &WorkerThread::progressChanged, [](int p) {
        qDebug() << "Progress:" << p << "%";
    });

    QObject::connect(worker, &WorkerThread::finished, [&app](const QString& result) {
        qDebug() << "Ready:" << result;
        app.quit();
    });

    QObject::connect(worker, &WorkerThread::errorOccurred, [&app](const QString& err) {
        qDebug() << "Error:" << err;
        app.quit();
    });

    // Важливо: видаляємо об'єкт після завершення потоку
    QObject::connect(worker, &WorkerThread::finished,
                     worker, &QObject::deleteLater);

    worker->start(); // Запускає новий потік та викликає run()

    return app.exec();
}
