#include "TaskModel.h"

TaskModel::TaskModel(QObject* parent)
    : QAbstractListModel(parent)
{}

int TaskModel::rowCount(const QModelIndex &) const
{
    return m_tasks.size();
}

// Повертає значення поля за роллю для рядка idx.row()
QVariant TaskModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_tasks.size())
        return {};

    const Task& t = m_tasks[index.row()];
    switch (role) {
        case FilenameRole: return t.filename;
        case StateRole: return static_cast<int>(t.state);
        case StateTextRole:
            switch (t.state) {
                case Task::State::Pending: return "In queue";
                case Task::State::Running: return "Processing";
                case Task::State::Done:    return "Ready";
                case Task::State::Error:   return "Error";
            }
        case ResultRole: return t.result;
    }
    return {};
}

// roleNames() — маппінг: роль → ім'я у QML делегаті
// QML: model.filename, model.stateText, model.result
QHash<int, QByteArray> TaskModel::roleNames() const
{
    return {
        {FilenameRole,  "filename"},
        {StateRole,     "state"},
        {StateTextRole, "stateText"},
        {ResultRole,    "result"}
    };
}

void TaskModel::addTask(const Task& task)
{
    // beginInsertRows/endInsertRows — повідомляємо ListView про новий рядок
    beginInsertRows({}, m_tasks.size(), m_tasks.size());
    m_tasks.append(task);
    endInsertRows();
}

void TaskModel::updateTask(int id, Task::State state, const QString& result)
{
    for (int i = 0; i < m_tasks.size(); ++i) {
        if (m_tasks[i].id == id) {
            m_tasks[i].state  = state;
            m_tasks[i].result = result;
            // dataChanged — ListView перемальовує тільки цей рядок
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {StateRole, StateTextRole, ResultRole});
            return;
        }
    }
}