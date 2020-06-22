#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#include "task.h"
#include <QSettings>
#include <thread>
#include <openssl/aes.h>
#include <winsock2.h>
#include <algorithm>
#include <string>
#include <ctime>
#include "taskqueue.h"
#include "constants.h"
#include "baseclass.h"

class Transporter : public BaseClass {
public:
    Transporter(const unsigned char* _key, const unsigned char *_token, const int &_port);
    ~Transporter();
    void threadInstance();

    void addTask(const Task &_task);
    void pauseRunningTask();
    void pauseTask(const int &_index);
    void startTask(const int &_index);
    void deleteTask(const int &_index, Type _type);
    void stopThread();
    bool checkTask(const Task &_task);
    void pauseNowTask();
    void stopTask();

    std::string getList();
    std::string getQueueInfo();
private:
    TaskQueue taskQueue;
    Task* nowTask;
    AES_KEY aesKey;
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH], buffer[BUFFER_LENGTH];
    bool pause = false, threadStatus = false, stop = false;

    void uploadFile();
    void downloadFile();

    void cleanBuffer();
};

#endif // TRANSPORTER_H
