#include "headers/taskqueue.h"

TaskQueue::TaskQueue() {
    runningTaskHead = new Task;
    pauseTaskHead = new Task;
}

TaskQueue::~TaskQueue() {
    if (!runningTaskHead) delete runningTaskHead;
    if (!pauseTaskHead) delete pauseTaskHead;
}

void TaskQueue::addTask(const Task &_task) {
    Task* nxtTask = new Task(_task);
    Task *p = runningTaskHead;
    while (p -> nextTask != nullptr) {
        p = p -> nextTask;
    }
    p -> nextTask = nxtTask;
    nxtTask -> nextTask = nullptr;
}

void TaskQueue::addTaskPointer(Task *_task, Type _type) {
    Task *p = nullptr;
    if (_type == Type::RUNNING) p = runningTaskHead;
    else p = pauseTaskHead;
    while (p -> nextTask != nullptr) {
        p = p -> nextTask;
    }
    p -> nextTask = _task;
    _task -> nextTask = nullptr;
}

void TaskQueue::deleteTask(const int &_index, Type _type) {
    Task *p = nullptr, *t = nullptr;
    int index = -1;
    if (_type == Type::RUNNING) p = runningTaskHead;
    else p = pauseTaskHead;

    while (index != _index && p != nullptr) {
        p = p -> nextTask;
        index ++;
    }

    if (index != _index && p == nullptr) return ;
    t = p -> nextTask;
    p -> nextTask = t -> nextTask;
    delete t;
}

void TaskQueue::pauseTask(const int &_index) {
    int index = 0;
    Task *p = runningTaskHead, *t = nullptr;

    while (index != _index && p != nullptr) {
        p = p -> nextTask;
        index ++;
    }

    if (index != _index && p == nullptr) return ;
    t = p -> nextTask;
    p -> nextTask = t -> nextTask;
    t -> nextTask = nullptr;
    addTaskPointer(t, Type::PAUSED);
}


void TaskQueue::startTask(const int &_index) {
    int index = 0;
    Task *p = pauseTaskHead, *t = nullptr;

    while (index != _index && p != nullptr) {
        p = p -> nextTask;
        index ++;
    }

    if (index != _index && p == nullptr) return ;
    t = p -> nextTask;
    p -> nextTask = t -> nextTask;
    t -> nextTask = nullptr;
    addTaskPointer(t, Type::RUNNING);
}

Task* TaskQueue::popTask() {
    Task* res = nullptr, *p = runningTaskHead;
    if (p -> nextTask == nullptr) return nullptr;
    res = p -> nextTask;
    p -> nextTask = res -> nextTask;
    res -> nextTask = nullptr;
    return res;
}

bool TaskQueue::checkTask(const Task &_task) {
    Task* p = runningTaskHead -> nextTask;
    while (p != nullptr) {
        if (_task == *p) return true;
        p = p -> nextTask;
    }
    p = pauseTaskHead -> nextTask;
    while (p != nullptr) {
        if (_task == *p) return true;
        p = p -> nextTask;
    }
    return false;
}

bool TaskQueue::empty() {
    return runningTaskHead -> nextTask == nullptr;
}

std::string TaskQueue::popTaskInfo() {
    std::string res;
    res.clear();
    Task *p = runningTaskHead -> nextTask;
    while (p != nullptr) {
        res.append(p -> popTaskInfo());
        res.push_back('/');
        res.append("runnning;");
        p = p -> nextTask;
    }

    p = pauseTaskHead -> nextTask;
    while (p != nullptr) {
        res.append(p -> popTaskInfo());
        res.push_back('/');
        res.append("paused;");
        p = p -> nextTask;
    }
    return res;
}
