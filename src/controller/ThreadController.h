#pragma once
#include <QObject>
#include <QString>
#include "TaskModel.h"
#include "task_queue.h"

class ThreadController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(TaskModel* taskModel READ taskModel CONSTANT)

public:
    explicit ThreadController(QObject *parent = nullptr);
    ~ThreadController() override;

    QString status() const { return m_status; }
    bool running() const { return m_running; }
    TaskModel* taskModel() { return &m_model; }

    Q_INVOKABLE void addFile(const QString& filename);
    Q_INVOKABLE void startAll();
    Q_INVOKABLE void stopAll();
    Q_INVOKABLE void clearCompleted();

signals:
    void statusChanged(const QString& status);
    void runningChanged(bool running);

private:
    void setStatus(const QString& v);
    void setRunning(bool v);

private:
    TaskModel m_model;
    TaskQueue* m_queue{nullptr};
    QThread* m_thread{nullptr};
    int m_nextId{1};
    bool m_running{false};
    QString m_status{"Ready"};
};
