#include "ThreadController.h"
#include <QDebug>
#include <QThread>

ThreadController::ThreadController(QObject *parent)
    : QObject{parent}
{}

ThreadController::~ThreadController()
{
    stopAll();
}

void ThreadController::addFile(const QString &filename)
{
    if (filename.trimmed().isEmpty())
        return;

    Task t;
    t.filename = filename.trimmed();
    t.id = m_nextId++;
    t.state = Task::State::Pending;
    m_model.addTask(t);
    setStatus(QString("Added: %1").arg(t.filename));
}

void ThreadController::startAll()
{
    if (m_running)
        return;

    // Прибираємо попередній потік
    if (m_thread) {
        m_queue->stop();
        m_thread->wait();
        m_thread->deleteLater();
        m_queue->deleteLater();
    }

    m_queue  = new TaskQueue();
    m_thread = new QThread(this);
    m_queue->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_queue, &TaskQueue::process);
    connect(m_queue,  &TaskQueue::allDone, this, [this]() {
        setRunning(false);
        setStatus("All tasks completed");
    });
    connect(m_queue, &TaskQueue::taskStarted, this, [this](int id) {
        m_model.updateTask(id, Task::State::Running);
    });
    connect(m_queue, &TaskQueue::taskCompleted, this, [this](int id, const QString& r) {
        m_model.updateTask(id, Task::State::Done, r);
    });
    connect(m_queue, &TaskQueue::taskFailed, this, [this](int id, const QString& e) {
        m_model.updateTask(id, Task::State::Error, e);
    });

    // Ставимо всі pending задачі в чергу
    // (доступ до m_model — тільки з main thread, тут ок)
    for (int i = 0; i < m_model.rowCount(); i++) {
        QModelIndex idx = m_model.index(i);
        if (m_model.data(idx, TaskModel::StateRole).toInt() == static_cast<int>(Task::State::Pending)) {
            Task t;
            t.filename = m_model.data(idx, TaskModel::FilenameRole).toString();
            t.id = m_model.data(idx, TaskModel::IdRole).toInt();
            m_queue->enqueue(t);
        }
    }

    setRunning(true);
    setStatus("Process started...");
    m_thread->start();
}

void ThreadController::stopAll()
{
    if (m_queue)
        m_queue->stop();
    if (m_thread) {
        m_thread->quit();
        m_thread->wait(2000);
    }
    setRunning(false);
}

void ThreadController::clearCompleted()
{
    m_model.removeCompletedTasks();
    setStatus("Cleared completed tasks");
}

void ThreadController::setStatus(const QString &v)
{
    if (m_status == v)
        return;
    m_status = v;
    emit statusChanged(v);
}

void ThreadController::setRunning(bool v)
{
    if (m_running == v)
        return;
    m_running = v;
    emit runningChanged(v);
}

