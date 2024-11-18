#include <QThread>
#include <utility>
#include <queue>
#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

template<class Sender, class Task>
class SessionManager : public QThread{
public:
    void addTask(std::pair<Sender*, Task> newTask)
    {
        m_task_queue.push(newTask);
    }

public:
    std::queue<std::pair<Sender*,Task>> m_task_queue;
};
#endif
