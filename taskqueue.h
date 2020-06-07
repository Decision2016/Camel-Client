#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "task.h"
#include "constants.h"

class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();
    void addTask(const Task &_task);
    void addTaskPointer(Task *_task, Type _type);
    void deleteTask(const int &_index, Type _type);
    void pauseTask(const int &_index);
    void startTask(const int &_index);
    bool checkTask(const Task &_task);
    bool empty();
    Task* popTask();
private:
    Task *runningTaskHead, *pauseTaskHead;
};

#endif // TASKQUEUE_H
