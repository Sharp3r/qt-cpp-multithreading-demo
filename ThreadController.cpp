#include "ThreadController.h"
#include <QDebug>

ThreadController::ThreadController(QObject *parent)
    : QObject{parent}
{}

void ThreadController::startProcessing(const QString &filename)
{
    if (m_running)
        return;

    if (m_worker) {
        m_worker->requestStop();
        m_worker->wait();
        m_worker->deleteLater();
    }

    m_worker = new WorkerThread(filename, this);

    connect(m_worker, &WorkerThread::progressChanged,
            this, &ThreadController::setProgress);

    connect(m_worker, &WorkerThread::finished,
            this, [this](const QString& result) {
                setStatus("Ready: " + result);
                setRunning(false);
            });

    connect(m_worker, &WorkerThread::errorOccurred,
            this, [this](const QString& err) {
                setStatus("Error: " + err);
                setRunning(false);
            });

    connect(m_worker, &WorkerThread::finished,
            m_worker, &QObject::deleteLater);

    setProgress(0);
    setStatus("Processed: " + filename);
    setRunning(true);
    m_worker->start();
}

void ThreadController::stopProcessing()
{
    if (m_worker && m_running) {
        m_worker->requestStop();
        setStatus("Cancelled!");
        setRunning(false);
    }
}

void ThreadController::setProgress(int v)
{
    if (m_progress == v)
        return;
    m_progress = v;
    emit progressChanged(v);
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

