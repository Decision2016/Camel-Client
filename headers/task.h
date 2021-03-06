#ifndef TASK_H
#define TASK_H

#include <string>
#include "constants.h"


class Task
{
public:
    Task();
    Task(const Task &_task);
    Task(taskType _type, std::string _name, std::string _origin, std::string _destination);
    taskType getType();
    unsigned long long getTotalLength();
    unsigned long long getCurrent();
    void addTransported(int _value);
    void setSize(unsigned long long _size);
    std::string popTaskInfo();
    friend bool operator == (const Task &_t1, const Task &_t2);

    Task* nextTask = nullptr;

    /*
     * @parmas
     * orgin: 原始文件地址
     *  在type = UPLOAD时为本地文件地址
     *  在type = DOWNLOAD时为服务器文件地址（当前浏览目录下的文件地址）
     * destination与之类似
     *
     * 服务器文件在上传过程中会在服务器进行地址解析，并且上传/下载前会检查文件是否存在
     */
    std::string name, origin, destination;
private:
    taskType type;
    unsigned long long totalLength;
    unsigned long long transportedLength;
    double percent;
};

#endif // TASK_H
