#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QList>
#include "task_queue.h"

// Модель для ListView в QML.
// Кожен рядок — одна задача з полями: filename, state, result.
class TaskModel : public QAbstractListModel
{
    Q_OBJECT

public:
    // Ролі — імена полів, доступних у QML делегаті
    // model.filename, model.stateText, model.result
    enum Roles {
        FilenameRole  = Qt::UserRole + 1,
        StateRole,
        StateTextRole,
        ResultRole
    };

    explicit TaskModel(QObject* parent = nullptr);

    // Обов'язкові для QAbstractListModel:
    int rowCount(const QModelIndex& = {}) const override;
    QVariant data(const QModelIndex& idx, int role) const override;
    QHash<int, QByteArray> roleNames() const override; // ← імена для QML

    void addTask(const Task& task);
    void updateTask(int id, Task::State state, const QString& result = {});

private:
    QList<Task> m_tasks;
};