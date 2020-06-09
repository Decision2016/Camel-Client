#include "headers/task.h"

Task::Task(taskType _type, std::string _name, std::string _origin, std::string _destination):
    name(_name),
    origin(_origin),
    destination(_destination),
    type(_type)
{
    totalLength = 0;
    transportedLength = 0;
    percent = 0.0;
}

Task::Task() {
    type = taskType::HEAD;
    nextTask = nullptr;
}

Task::Task(const Task &_task) {
    origin = _task.origin;
    name = _task.name;
    destination = _task.destination;
    nextTask = nullptr;

    type = _task.type;
    totalLength = _task.totalLength;
    transportedLength = 0;
    percent = 0.0;
}

bool operator == (const Task &_t1, const Task &_t2) {
    return (_t1.origin == _t2.origin && _t1.destination == _t2.destination);
}

taskType Task::getType() {
    return type;
}

unsigned long long Task::getTotalLength() {
    return totalLength;
}

unsigned long long Task::getCurrent() {
    return transportedLength;
}

void Task::addTransported(int _value) {
    transportedLength += _value;
    percent = transportedLength * 100.0 / totalLength;
}

void Task::setSize(unsigned long long _size) {
    totalLength = _size;
}

std::string Task::popTaskInfo() {
    std::string res;
    char buffer[10];
    sprintf(buffer, "%.2lf", percent);
    res.clear();
    res.append(name);
    res.push_back('/');
    res.append(std::string(buffer));
    if (type == taskType::UPLOAD) res.append("/1");
    else res.append("/0");
    return res;
}
