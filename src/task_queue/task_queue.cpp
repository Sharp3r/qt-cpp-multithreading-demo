#include "task_queue.h"
#include <QDebug>
#include <QThread>
#include <QFileInfo>

TaskQueue::TaskQueue(QObject* parent)
    : QObject(parent)
{}

TaskQueue::~TaskQueue()
{}

void TaskQueue::enqueue(const Task& task)
{
    // QMutexLocker — RAII обгортка над QMutex.
    // При виході з блоку {} mutex автоматично звільняється.
    // Це захищає від забутого unlock() та від виключень.
    QMutexLocker locker(&m_mutex);
    m_queue.enqueue(task);

    // Будимо потік-споживач, якщо він чекає на wakeOne()
    m_condition.wakeOne();
}   // locker виходить зі скоупу — mutex звільняється тут

void TaskQueue::stop()
{
    QMutexLocker locker(&m_mutex);
    m_running.store(false);
    m_condition.wakeAll(); // будимо всіх, щоб вони побачили m_running=false
}

void TaskQueue::process()
{
    qDebug() << "Worker запущено в потоці:" << QThread::currentThread();

    while (m_running.load()) {
        Task task;
        {
            // Окремий блок для мінімального утримання mutex
            QMutexLocker locker(&m_mutex);

            // wait() атомарно: звільняє mutex І блокує потік.
            // Коли прийде wakeOne() — знову захоплює mutex і продовжує.
            // Умова: чекаємо поки черга НЕ порожня АБО зупинено
            while (m_queue.isEmpty() && m_running.load()) {
                m_condition.wait(&m_mutex);
            }

            if (!m_running.load() && m_queue.isEmpty()) break;
            task = m_queue.dequeue();
        } // mutex звільняється тут — обробка іде без блокування черги

        // Обробляємо задачу ПОЗА mutex — інші потоки можуть enqueue()
        QFileInfo info(task.filename);
        QThread::msleep(500); // симуляція роботи
        emit taskCompleted(task.id,
                           info.exists()
                               ? QString("OK: %1").arg(info.fileName())
                               : QString("NOT FOUND: %1").arg(task.filename));
    }

    emit allDone();
}

