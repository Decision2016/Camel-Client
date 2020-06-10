#include "transporter.h"

Transporter::Transporter(const unsigned char* _key, const unsigned char *_token, const int &_port) : BaseClass(_key, _token){
    SOCKET _socket;
    WSADATA s;
    nowTask = nullptr;
    setPort(_port);
    struct sockaddr_in sin;

    if (WSAStartup(MAKEWORD(2, 2), &s) != 0) {
        // todo: notice error
        return ;
    }

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET) {
        // todo: notice error
        return ;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if ( ::connect(_socket, (struct sockaddr*)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        //todo: notice error
        closesocket(_socket);
        return ;
    }
    setSocket(_socket);
}

Transporter::~Transporter() {
    closesocket(thread_socket);
}

void Transporter::threadInstance() {
    while (true) {
        if (stopThread) break;
        if (! taskQueue.empty() && nowTask == nullptr) {
            nowTask = taskQueue.popTask();
        }
        else continue;

        if (nowTask ->getType() == taskType::UPLOAD) uploadFile();
        else downloadFile();
        Sleep(1);
    }
}

void Transporter::uploadFile() {
    unsigned long long statusCode;
    int length, nextLen;
    unsigned long long total = nowTask -> getTotalLength();
    unsigned long long index = nowTask -> getCurrent();
    length = nowTask -> destination.length();
    cleanBuffer();
    FILE *fp = fopen(nowTask -> origin.c_str(), "rb");

    pushValue((unsigned char*) buffer, FILE_UPLOAD, STATUS_LENGTH);
    setToken((unsigned char*) &buffer[2]);
    pushValue((unsigned char*) &buffer[34], index, 8);
    pushValue((unsigned char*) &buffer[42], length, 2);
    memcpy(&buffer[44], nowTask -> destination.c_str(), length);

    aesEncrypt((unsigned char*) buffer, (unsigned char*) send_buffer, BUFFER_LENGTH);

    send(thread_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, 2);
        if (statusCode != SERVER_FILE_RECEIVE) {
            //todo： 异常处理
        }
        if (index == total) {
            sendStatusCode(CLIENT_FILE_END);
            delete nowTask;
            nowTask = nullptr;
            break;
        }
        if (pause) {
            sendStatusCode(FILE_PAUSED);
            taskQueue.addTaskPointer(nowTask, Type::PAUSED);
            nowTask = nullptr;
            pause = false;
            break;
        }
        if (stop) {
            sendStatusCode(FILE_STOP);
            delete nowTask;
            nowTask = nullptr;
            stop = false;
            break;
        }
        nextLen = std::min(total - index, ONCE_MAX_LENGTH);
        pushValue((unsigned char*) buffer, FILE_TRANSPORT, 2);
        setToken((unsigned char*) &buffer[2]);
        pushValue((unsigned char*) &buffer[34], nextLen, 2);

        fread(&buffer[36], 1, nextLen, fp);
        aesEncrypt((unsigned char*)buffer, (unsigned char*) send_buffer, BUFFER_LENGTH);
        send(thread_socket, send_buffer, BUFFER_LENGTH, 0);
        index += nextLen;
        nowTask -> addTransported(nextLen);

    }
    fclose(fp);
}

void Transporter::downloadFile() {
    unsigned long long statusCode;
    int length;
    cleanBuffer();

    FILE *fp;
    fp = fopen(nowTask -> destination.c_str(), "ab");
    length = nowTask -> origin.length();

    pushValue((unsigned char*) buffer, FILE_DOWNLOAD, 2);
    setToken((unsigned char*) &buffer[2]);
    pushValue((unsigned char*) &buffer[34], nowTask -> getCurrent(), 8);
    pushValue((unsigned char*) &buffer[42], length, 2);
    memcpy(&buffer[44], nowTask -> origin.c_str(), length);

    aesEncrypt((unsigned char*)buffer, (unsigned char*)send_buffer, BUFFER_LENGTH);
    send(thread_socket, send_buffer, BUFFER_LENGTH, 0);

    recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
    aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);

    unsigned long long _size;
    popValue((unsigned char*)&buffer[2], _size, 8);
    nowTask->setSize(_size);

    sendStatusCode(FILE_TRANSPORT);
    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }

        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);

        unsigned long long len;
        popValue((unsigned char*)buffer, statusCode, 2);
        popValue((unsigned char*) &buffer[2], len, 2);
        fwrite(&buffer[4], 1, len, fp);
        nowTask -> addTransported(len);
        if (statusCode == SERVER_FILE_TRANSPORT) {
            sendStatusCode(FILE_TRANSPORT);
            continue;
        }
        if (statusCode == SERVER_FILE_FINISHED) {
            delete nowTask;
            nowTask = nullptr;
            fclose(fp);
            break;
        }

        if (pause) {
            sendStatusCode(FILE_PAUSED);
            taskQueue.addTaskPointer(nowTask, Type::PAUSED);
            nowTask = nullptr;
            pause = false;
            fclose(fp);
            break;
        }
        if (stop) {
            sendStatusCode(FILE_STOP);
            remove(nowTask -> destination.c_str());
            delete nowTask;
            nowTask = nullptr;
            stop = false;
            fclose(fp);
            break;
        }
    }
}

void Transporter::addTask(const Task &_task) {
    taskQueue.addTask(_task);
}

void Transporter::pauseTask(const int &_index) {
    if (_index == 0) pause = true;
    else taskQueue.pauseTask(_index - 1);
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

std::string Transporter::getQueueInfo() {
    std::string res;
    if (nowTask != nullptr) {
        res = nowTask->popTaskInfo();
        res.append("/running;");
    }
    else res.clear();

    res.append(taskQueue.popTaskInfo());
    return res;
}


void Transporter::stopTask() {
    stop = true;
}
