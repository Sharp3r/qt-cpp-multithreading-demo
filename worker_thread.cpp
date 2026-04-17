#include "worker_thread.h"
#include <QDebug>
#include <QFileInfo>

WorkerThread::WorkerThread(const QString &filename, QObject *parent)
    : QThread(parent), m_filename(filename)
{}

WorkerThread::~WorkerThread()
{}

void WorkerThread::requestStop()
{
    m_stopRequested.store(true);
}

void WorkerThread::run()
{
    // Все що тут — виконується в НОВОМУ потоці, не в main thread
    qDebug() << "[Thread" << QThread::currentThread() << "] Processing started:" << m_filename;

    QFileInfo info(m_filename);
    if (!info.exists()) {
        // signal можна emit з будь-якого потоку в Qt — Qt::QueuedConnection
        // автоматично ставить виклик слота в чергу потоку-одержувача
        emit errorOccurred(QString("File not found: %1").arg(m_filename));
        return;
    }

    // Симулюємо важку обробку: 10 кроків по 200ms
    int step_count = 10;
    auto emitFinished = [this](const QString &name, qint64 size) {
        emit finished(QString("Processed: %1 (%2 bytes)").arg(name).arg(size));
    };

    for (int i = 0; i <= step_count; ++i) {
        // Перевіряємо прапор зупинки на кожній ітерації
        if (m_stopRequested.load()) {
            qDebug() << "Processing canceled";
            emitFinished(info.fileName(), i * info.size() / step_count);
            return;
        }

        // msleep — зупиняє ПОТОЧНИЙ потік (новий), не main thread
        QThread::msleep(200);
        emit progressChanged(i * step_count);
    }
    emitFinished(info.fileName(), info.size());
}
