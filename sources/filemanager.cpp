#include "headers/filemanager.h"

FileManager::FileManager(SOCKET _client, unsigned char* _token, unsigned char* _key): BaseClass(_key, _token) {
    setSocket(_client);
}

bool FileManager::createDirectory(QString _dirName) {
    unsigned long long statusCode;
    int length;
    sendDirInfo(CREATE_DIR, _dirName);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            // todo: 超时检测
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, STATUS_LENGTH);
        if (statusCode != SERVER_DIR_CREATED) return false;
        else break;
    }
    return true;
}

bool FileManager::deleteDirectory(QString _dirName) {
    unsigned long long statusCode;
    int length;
    sendDirInfo(DELETE_DIR, _dirName);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, STATUS_LENGTH);
        if (statusCode != SERVER_DELETE) return false;
        else break;
    }
    return true;
}

bool FileManager::openDirectory(QString _dirName) {
    unsigned long long statusCode;
    int length;
    sendDirInfo(ENTER_DIR, _dirName);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, STATUS_LENGTH);
        if (statusCode != SERVER_DIR_INFO) return false;
        else break;
    }
    return true;
}

bool FileManager::backupDirectory() {
    unsigned long long statusCode;
    int length;

    sendStatusCode(BACKUP_DIR);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, 2);
        if (statusCode != SERVER_DIR_INFO) return false;
        else break;
    }
    return true;
}

std::string FileManager::getNowPath() {
    std::string res;
    unsigned long long statusCode, len;
    int length;
    clearBuffer();

    sendStatusCode(FILE_GET_PATH);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, STATUS_LENGTH);
        if (statusCode != SERVER_FILE_PATH) break;
        popValue((unsigned char*) &buffer[STATUS_LENGTH], len, STATUS_LENGTH);

        res.clear();
        for (int i = 0; i < (int)len;i++) res.push_back(buffer[i + 4]);
        break;
    }
    return res;
}

bool FileManager::rename(QString &_originName, QString &_newName) {
    std::string nameData = std::string( _originName.toLocal8Bit().data()) + "/" + std::string(_newName.toLocal8Bit().data());
    unsigned long long statusCode;
    int length = nameData.length();
    clearBuffer();

    pushValue((unsigned char*) buffer, RENAME_DIR_FILE, STATUS_LENGTH);
    memcpy((unsigned char*) &buffer[STATUS_LENGTH], token, TOKEN_LENGTH);
    pushValue((unsigned char*) &buffer[DATA_START], length, 2);
    memcpy(&buffer[36], nameData.c_str(), length);

    aesEncrypt((unsigned char*)buffer, (unsigned char*)send_buffer, BUFFER_LENGTH);
    send(thread_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, 2);
        if (statusCode != SERVER_RENAME) return false;
        else break;
    }
    return true;
}

bool FileManager::deleteFile(QString _fileName) {
    unsigned long long statusCode;
    int length;
    sendDirInfo(FILE_DELETE, _fileName);

    while (true) {
        length = recv(thread_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        popValue((unsigned char*)buffer, statusCode, 2);
        if (statusCode != SERVER_DELETE) return false;
        else break;
    }
    return true;
}

void FileManager::clearBuffer() {
    memset(send_buffer, 0, BUFFER_LENGTH);
    memset(recv_buffer, 0, BUFFER_LENGTH);
    memset(buffer, 0, BUFFER_LENGTH);
}

void FileManager::sendDirInfo(const int &statusCode, QString _dirName) {
    std::string dirName = _dirName.toStdString();
    clearBuffer();
    int length = dirName.length();

    pushValue((unsigned char*) buffer, statusCode, STATUS_LENGTH);
    memcpy((unsigned char*) &buffer[STATUS_LENGTH], token, TOKEN_LENGTH);

    pushValue((unsigned char*) &buffer[DATA_START], length, 2);
    memcpy(&buffer[36], dirName.c_str(), length);
    aesEncrypt((unsigned char*)buffer, (unsigned char*) &send_buffer, BUFFER_LENGTH);

    send(thread_socket, send_buffer, BUFFER_LENGTH, 0);
}
