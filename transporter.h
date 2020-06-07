#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#include "task.h"

#include <thread>
#include <openssl/aes.h>
#include <winsock2.h>
#include <algorithm>
#include "taskqueue.h"
#include "task.h"
#include "constants.h"

class Transporter
{
public:
    Transporter(SOCKET _socket);
    void threadInstance();

    void addTask(const Task &_task);
    void pauseRunningTask();
    void pauseTask(const int &_index);
    void startTask(const int &_index);
    void deleteTask(const int &_index, Type _type);
    bool checkTask(const Task &_task);
    void pauseNowTask();
    std::string getList();
private:
    SOCKET file_socket;
    TaskQueue taskQueue;
    Task* nowTask;
    AES_KEY aesKey;
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH], buffer[BUFFER_LENGTH];
    unsigned char iv[TOKEN_LENGTH], key[KEY_LENGTH];
    bool pause = false, stopThread = false;

    void uploadFile();
    void downloadFile();

    void cleanBuffer();
    void aesEncrypto(unsigned char *in, unsigned char *out, int len);
    void aesDecrypto(unsigned char *in, unsigned char *out, int len);

    static void pushValue(unsigned char *buffer, const long long &value, const int bytes_len);
    static void popValue(const unsigned char *buffer, long long &value, const int bytes_len);
};

#endif // TRANSPORTER_H
