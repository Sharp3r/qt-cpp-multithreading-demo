#include "worker_thread.h"
#include <QDebug>
#include <QFileInfo>

QString threadId()
{
    return QString("0x%1").arg((quintptr)QThread::currentThreadId(), 0, 16);
}

WorkerThread::WorkerThread(const QString &filename, QObject *parent)
    : QThread(parent), m_filename(filename)
{
    qDebug() << "[Worker constructed]" << threadId() << this;
}

WorkerThread::~WorkerThread()
{
    qDebug() << "[Worker destroyed]" << threadId() << this;
}

void WorkerThread::requestStop()
{
    qDebug() << "[Worker stop requested]" << threadId() << this;
    m_stopRequested.store(true);
}

void WorkerThread::run()
{
    // Все що тут — виконується в НОВОМУ потоці, не в main thread
    qDebug() << "[Worker run start]" << threadId() << this;

    QFileInfo info(m_filename);
    if (!info.exists()) {
        // signal можна emit з будь-якого потоку в Qt — Qt::QueuedConnection
        // автоматично ставить виклик слота в чергу потоку-одержувача
        qDebug() << "[Worker canceled]" << threadId() << this;
        emit errorOccurred(QString("File not found: %1").arg(m_filename));
        return;
    }

    // Симулюємо важку обробку: 10 кроків по 200ms
    int step_count = 10;
    auto emitFinished = [this](const QString &name, qint64 size) {
        qDebug() << "[Worker finished]" << threadId() << this;
        emit finished(QString("Processed: %1 (%2 bytes)").arg(name).arg(size));
    };

    for (int i = 0; i <= step_count; ++i) {
        // Перевіряємо прапор зупинки на кожній ітерації
        if (m_stopRequested.load()) {
            qDebug() << "[Worker processing canceled]" << threadId() << this;
            emitFinished(info.fileName(), i * info.size() / step_count);
            return;
        }

        // msleep — зупиняє ПОТОЧНИЙ потік (новий), не main thread
        QThread::msleep(500);
        qDebug() << "[Worker progress changed]" << threadId() << this;
        emit progressChanged(i * step_count);
    }
    emitFinished(info.fileName(), info.size());
}
