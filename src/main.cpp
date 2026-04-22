#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "task_queue.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // Створюємо Worker і Thread окремо
    auto* queue  = new TaskQueue();
    auto* thread = new QThread();

    // Переміщуємо Worker в новий потік.
    // Після цього всі слоти queue виконуватимуться в thread.
    queue->moveToThread(thread);

    QObject::connect(thread, &QThread::started, queue, &TaskQueue::process);
    QObject::connect(queue, &TaskQueue::allDone, thread, &QThread::quit);
    QObject::connect(thread, &QThread::finished, queue, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    QObject::connect(queue, &TaskQueue::taskCompleted,
                     [](int id, const QString& r) {
                         qDebug() << "Task" << id << ":" << r;
                     });

    // Стартуємо потік
    thread->start();

    // Додаємо задачі в чергу
    queue->enqueue({"test.txt", 1});
    queue->enqueue({"test2.txt", 2});
    queue->enqueue({"test3.txt", 3});

    QTimer::singleShot(800, [queue]() { // симуляція зупинки
        qDebug() << "[main timer single shot]" << QThread::currentThread() << "Stop queue!";
        queue->stop();
    });

    QObject::connect(thread, &QThread::finished, &app, &QCoreApplication::quit);

    return app.exec();
}
