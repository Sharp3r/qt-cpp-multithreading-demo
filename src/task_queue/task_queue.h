#pragma once
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QString>

// DTO — описує одну задачу. Відокремлений від TaskQueue.
struct Task {
    QString filename;
    int id;

    enum class State {
        Pending,
        Running,
        Done,
        Error
    } state{State::Pending};
    QString result;
};

// JUNIOR рівень: Worker — це звичайний QObject.
// Потік існує окремо, Worker "живе" в ньому через moveToThread().
// Перевага: Worker можна замінити, протестувати, підключити до UI — незалежно від потоку.
class TaskQueue : public QObject
{
    Q_OBJECT

public:
    explicit TaskQueue(QObject* parent = nullptr);
    ~TaskQueue() override;

    // Додати задачу з БУДЬ-ЯКОГО потоку (thread-safe)
    void enqueue(const Task& task);

public slots:
    // Цей слот викличеться в потоці, куди переміщено об'єкт
    void process();

    // Зупинити чергу
    void stop();

signals:
    void taskStarted(int id);
    void taskFailed(int id, const QString& error);
    void taskCompleted(int id, const QString& result);
    void allDone();

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    QQueue<Task> m_queue;
    bool m_running{true};
};
