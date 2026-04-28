#include "task_queue.h"
#include <QDebug>
#include <QThread>
#include <QFileInfo>

QString threadId()
{
    return QString("0x%1").arg((quintptr)QThread::currentThreadId(), 0, 16);
}

TaskQueue::TaskQueue(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[TaskQueue constructed]" << threadId() << this;
}

TaskQueue::~TaskQueue()
{
    qDebug() << "[TaskQueue destroyed]" << threadId() << this;
}

void TaskQueue::enqueue(const Task& task)
{
    // QMutexLocker — RAII обгортка над QMutex.
    // При виході з блоку {} mutex автоматично звільняється.
    // Це захищає від забутого unlock() та від виключень.
    QMutexLocker locker(&m_mutex);
    qDebug() << "[TaskQueue enqueue]" << task.filename << threadId() << this;
    m_queue.enqueue(task);

    // Будимо потік, якщо він чекає на wakeOne()
    m_condition.wakeOne();
}   // locker виходить зі скоупу — mutex звільняється тут

void TaskQueue::stop()
{
    QMutexLocker locker(&m_mutex);
    qDebug() << "[TaskQueue stop]" << threadId() << this;
    m_running = false;
    m_condition.wakeAll(); // будимо всіх, щоб вони побачили m_running=false
}

void TaskQueue::process()
{
    qDebug() << "[TaskQueue process]" << threadId() << this;

    while (true) {
        Task task;
        {
            // Окремий блок для мінімального утримання mutex
            QMutexLocker locker(&m_mutex);
            qDebug() << "[TaskQueue process]" << threadId() << this << "Mutex locked!";

            // wait() атомарно: звільняє mutex І блокує потік.
            // Коли прийде wakeOne() — знову захоплює mutex і продовжує.
            // Умова: чекаємо поки черга НЕ порожня АБО зупинено
            while (m_queue.isEmpty() && m_running) {
                m_condition.wait(&m_mutex);
            }

            if (!m_running && m_queue.isEmpty()) {
                qDebug() << "[TaskQueue process]" << threadId() << this << "Not running and queue is empty. Exiting...";
                break;
            }
            task = m_queue.dequeue();
            qDebug() << "[TaskQueue process]" << threadId() << this << "Dequeue task" << task.id << "for" << task.filename << ". Mutex unlocked!";
        } // mutex звільняється тут — обробка іде без блокування черги

        emit taskStarted(task.id);

        // Обробляємо задачу ПОЗА mutex — інші потоки можуть enqueue()
        QFileInfo info(task.filename);
        QThread::msleep(500); // симуляція роботи

        if (!info.exists()) {
            emit taskFailed(task.id,
                            QString("File not found: %1").arg(task.filename));
        } else {
            emit taskCompleted(task.id,
                               QString("%1 (%2 bytes)")
                                   .arg(info.fileName())
                                   .arg(info.size()));
        }
    }

    qDebug() << "[TaskQueue process]" << threadId() << this << "Stopped!";
    emit allDone();
}

