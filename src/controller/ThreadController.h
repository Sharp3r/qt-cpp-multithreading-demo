#pragma once
#include <QObject>
#include <QString>
#include "worker_thread.h"

class ThreadController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit ThreadController(QObject *parent = nullptr);

    int progress() const { return m_progress; }
    QString status() const { return m_status; }
    bool running() const { return m_running; }

    Q_INVOKABLE void startProcessing(const QString& filename);
    Q_INVOKABLE void stopProcessing();

signals:
    void progressChanged(int progress);
    void statusChanged(const QString& status);
    void runningChanged(bool running);

private:
    void setProgress(int v);
    void setStatus(const QString& v);
    void setRunning(bool v);

private:
    WorkerThread* m_worker{nullptr};
    int m_progress{0};
    QString m_status{"Ready"};
    bool m_running{false};
};
