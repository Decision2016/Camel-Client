#include "task.h"

Task::Task(taskType _type, std::string _origin, std::string _destination):
    type(_type),
    origin(_origin),
    destination(_destination)
{}

Task::Task() {
    type = taskType::HEAD;
    nextTask = nullptr;
}


bool operator == (const Task &_t1, const Task &_t2) {
    return (_t1.origin == _t2.origin && _t1.destination == _t2.destination);
}

taskType Task::getType() {
    return type;
}
