#include "transporter.h"

Transporter::Transporter(SOCKET _socket): file_socket(_socket) {}

void Transporter::threadInstance() {
    while (true) {
        if (stopThread) break;
        if (! taskQueue.empty()) {
            nowTask = taskQueue.popTask();
        }
        if (nowTask ->getType() == taskType::UPLOAD) uploadFile();
        else downloadFile();
    }
}

void Transporter::uploadFile() {
    long long statusCode;
    int length, nextLen;
    int total = nowTask -> getTotalLength();
    int index = nowTask -> getCurrent();
    cleanBuffer();
    FILE *fp = fopen(nowTask -> origin.c_str(), "rb");

    pushValue((unsigned char*) send_buffer, FILE_UPLOAD, 2);
    length = nowTask -> destination.length();
    pushValue((unsigned char*) &send_buffer[2], length, 2);
    aesEncrypto((unsigned char*)nowTask -> destination.c_str(), (unsigned char*) &buffer[4], length);

    send(file_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(file_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_FILE_UPLOAD) {
            //todo： 异常处理
        }
        if (index == total) {
            // 仅发送状态码的代码可以整合为一个函数
            pushValue((unsigned char*)send_buffer, FILE_END, 2);
            send(file_socket, send_buffer, 2, 0);
            delete nowTask;
            nowTask = nullptr;
            break;
        }
        if (pause) {
            pushValue((unsigned char*) send_buffer, FILE_PAUSED, 2);
            send(file_socket, send_buffer, 2, 0);
            taskQueue.addTaskPointer(nowTask, Type::PAUSED);
            pause = false;
            break;
        }
        nextLen = std::min(total - index, ONCE_LENGTH);
        pushValue((unsigned char*) send_buffer, FILE_TRANSPORT, 2);
        pushValue((unsigned char*) &send_buffer[2], nextLen, 2);

        fread(buffer, 1, BUFFER_LENGTH, fp);
        aesEncrypto((unsigned char*)buffer, (unsigned char*) &send_buffer[4], nextLen);
        send(file_socket, send_buffer, BUFFER_LENGTH, 0);
        index += nextLen;
        nowTask -> addTransported(nextLen);

    }
    fclose(fp);
}

void Transporter::downloadFile() {
    long long statusCode;
    int length;
    cleanBuffer();

    FILE *fp;
    fp = fopen(nowTask -> destination.c_str(), "ab");

    pushValue((unsigned char*) send_buffer, FILE_DOWNLOAD, 2);
    length = nowTask -> origin.length();
    pushValue((unsigned char*) &send_buffer[2], nowTask -> getCurrent(), 8);
    pushValue((unsigned char*) &send_buffer[10], length, 2);
    aesEncrypto((unsigned char*) nowTask -> origin.c_str(), (unsigned char*)&send_buffer[12], length);

    send(file_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(file_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        if (pause) {
            pushValue((unsigned char*) send_buffer, FILE_PAUSED, 2);
            send(file_socket, send_buffer, 2, 0);
            taskQueue.addTaskPointer(nowTask, Type::PAUSED);
            pause = false;
            break;
        }
        long long len;
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        popValue((unsigned char*) &recv_buffer[2], len, 2);
        aesDecrypto((unsigned char*)recv_buffer, (unsigned char*) buffer, len);
        fwrite(buffer, 1, len, fp);
        nowTask -> addTransported(len);
        if (statusCode == SERVER_FILE_TRANSPORT) continue;
        if (statusCode == SERVER_FILE_FINISHED) {
            delete nowTask;
            nowTask = nullptr;
            break;
        }
    }
    fclose(fp);
}

void Transporter::addTask(const Task &_task) {
    taskQueue.addTask(_task);
}

void Transporter::pauseTask(const int &_index) {
    taskQueue.pauseTask(_index);
}

void Transporter::startTask(const int &_index) {
    taskQueue.startTask(_index);
}

void Transporter::deleteTask(const int &_index, Type _type) {
    taskQueue.deleteTask(_index, _type);
}

bool Transporter::checkTask(const Task &_task) {
    bool inQueue = taskQueue.checkTask(_task);
    if (! nowTask) {
        return inQueue;
    }
    else
        return inQueue && (! (_task == *nowTask));
}

void Transporter::cleanBuffer() {
    memset(send_buffer, 0, BUFFER_LENGTH);
    memset(recv_buffer, 0, BUFFER_LENGTH);
    memset(buffer, 0, BUFFER_LENGTH);
}

void Transporter::aesEncrypto(unsigned char *in, unsigned char *out, int len) {
    memset(iv, 0, 16);
    AES_set_encrypt_key(key, AES_KEY_LENGTH, &aesKey);
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_ENCRYPT);
}

void Transporter::aesDecrypto(unsigned char *in, unsigned char *out, int len) {
    memset(iv, 0, 16);
    AES_set_decrypt_key(key, AES_KEY_LENGTH, &aesKey);
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_DECRYPT);
}


void Transporter::pushValue(unsigned char *buffer, const long long &value, const int bytes_len) {
    for (int i = 1; i <= bytes_len;i++) {
        buffer[i - 1] = (value >> ((bytes_len - i) * 8)) & 0xff;
    }
}

void Transporter::popValue(const unsigned char *buffer, long long &value, const int bytes_len) {
    value = 0;
    for (int i = 0; i < bytes_len; i++) {
        value <<= 8;
        value |= buffer[i];
    }
}
