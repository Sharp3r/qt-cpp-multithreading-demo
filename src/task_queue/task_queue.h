#pragma once
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QString>
#include <atomic>

// DTO — описує одну задачу. Відокремлений від Worker.
struct Task {
    QString filename;
    int id;
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

    // Зупинити чергу
    void stop();

public slots:
    // Цей слот викличеться в потоці, куди переміщено об'єкт
    void process();

signals:
    void taskCompleted(int id, const QString& result);
    void allDone();

private:
    QMutex         m_mutex;
    QWaitCondition m_condition;
    QQueue<Task>   m_queue;
    std::atomic<bool> m_running{true};
};
