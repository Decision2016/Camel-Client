#ifndef TASK_H
#define TASK_H

#include <string>


class Task
{
public:
    enum taskType {UPLOAD, DOWNLOAD};

    Task(taskType _type, std::string _origin, std::string _destination);
    void packString(std::string &buffer);
private:
    taskType type;

    /*
     * @parmas
     * orgin: 原始文件地址
     *  在type = UPLOAD时为本地文件地址
     *  在type = DOWNLOAD时为服务器文件地址（当前浏览目录下的文件地址）
     * destination与之类似
     *
     * 服务器文件在上传过程中会在服务器进行地址解析，并且上传/下载前会检查文件是否存在
     */
    std::string origin, destination;
};

#endif // TASK_H
