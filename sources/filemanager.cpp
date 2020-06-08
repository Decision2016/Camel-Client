#include "filemanager.h"

FileManager::FileManager(SOCKET _client, unsigned char* _token, unsigned char* _key): client_socket(_client) {
    memcpy(token, _token, TOKEN_LENGTH);
    memcpy(key, _key, KEY_LENGTH);
}

bool FileManager::createDirectory(QString _dirName) {
    /*
     * 创建过程中可能存在文件名空格问题
     * 后续调试进行解决
     */
    long long statusCode;
    int length;
    sendDirInfo(CREATE_DIR, _dirName);

    while (true) {
        length = recv(client_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            // todo: 超时检测
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_DIR_CREATED) return false;
        else break;
    }
    return true;
}

bool FileManager::deleteDirectory(QString _dirName) {
    long long statusCode;
    int length;
    sendDirInfo(DELETE_DIR, _dirName);

    while (true) {
        length = recv(client_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            // todo: 超时检测
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_DELETE) return false;
        else break;
    }
    return true;
}

bool FileManager::openDirectory(QString _dirName) {
    long long statusCode;
    int length;
    sendDirInfo(ENTER_DIR, _dirName);

    while (true) {
        length = recv(client_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            // todo: 超时检测
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_DIR_INFO) return false;
        else break;
    }
    return true;
}

bool FileManager::backupDirectory() {
    long long statusCode;
    int length;
    clearBuffer();
    pushValue((unsigned char*) send_buffer, BACKUP_DIR, 2);
    aesEncrypt(token, (unsigned char*) &send_buffer[2], TOKEN_LENGTH);

    send(client_socket, send_buffer, 34, 0);

    while (true) {
        length = recv(client_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            // todo: 超时检测
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_DIR_INFO) return false;
        else break;
    }
    return true;
}

std::string FileManager::getNowPath() {
    std::string res;
    long long statusCode, len;
    int length;
    clearBuffer();

    pushValue((unsigned char*)send_buffer, FILE_GET_PATH, 2);
    aesEncrypt(token, (unsigned char*) &send_buffer[2], TOKEN_LENGTH);
    send(client_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(client_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            // todo: 超时检测
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_FILE_PATH) break;
        popValue((unsigned char*)&recv_buffer[2], len, 2);
        aesDecrypt((unsigned char*)&recv_buffer[4], (unsigned char*) buffer, len);
        res.clear();

        for (int i = 0; i < len;i++) res.push_back(buffer[i]);
        break;
    }
    return res;
}


bool FileManager::rename(QString &_originName, QString &_newName) {
    std::string nameData = _originName.toStdString() + "/" + _newName.toStdString();
    long long statusCode;
    int length = nameData.length();
    clearBuffer();

    pushValue((unsigned char*) &send_buffer, RENAME_DIR_FILE, 2);
    aesEncrypt(token, (unsigned char*) &send_buffer[2], TOKEN_LENGTH);
    pushValue((unsigned char*) &send_buffer[34], length, 2);
    aesEncrypt((unsigned char*)nameData.c_str(), (unsigned char*)&send_buffer[36], length);

    send(client_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(client_socket, recv_buffer, BUFFER_LENGTH, 0);
        if (length == -1) {
            continue;
        }
        popValue((unsigned char*)recv_buffer, statusCode, 2);
        if (statusCode != SERVER_RENAME) return false;
        else break;
    }
    return true;
}

void FileManager::aesEncrypt(unsigned char *in, unsigned char *out, int len) {
    AES_set_encrypt_key(key, AES_KEY_LENGTH, &aesKey);
    clearIv();
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_ENCRYPT);
}


void FileManager::aesDecrypt(unsigned char *in, unsigned char *out, int len) {
    AES_set_decrypt_key(key, AES_KEY_LENGTH, &aesKey);
    clearIv();
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_DECRYPT);
}

void FileManager::clearBuffer() {
    memset(send_buffer, 0, BUFFER_LENGTH);
    memset(recv_buffer, 0, BUFFER_LENGTH);
    memset(buffer, 0, BUFFER_LENGTH);
}

void FileManager::clearIv() {
    memset(iv, 0, 16);
}

void FileManager::setToken(unsigned char *buffer) {
    memcpy(buffer, token, TOKEN_LENGTH);
}

void FileManager::pushValue(unsigned char *buffer, const long long &value, const int bytes_len) {
    for (int i = 1; i <= bytes_len;i++) {
        buffer[i - 1] = (value >> ((bytes_len - i) * 8)) & 0xff;
    }
}

void FileManager::popValue(const unsigned char *buffer, long long &value, const int bytes_len) {
    value = 0;
    for (int i = 0; i < bytes_len; i++) {
        value <<= 8;
        value |= buffer[i];
    }
}

void FileManager::sendDirInfo(const int &statusCode, QString _dirName) {
    std::string dirName = _dirName.toStdString();
    clearBuffer();
    int length = dirName.length();

    pushValue((unsigned char*) send_buffer, statusCode, 2);
    aesEncrypt(token, (unsigned char*) &send_buffer[2], TOKEN_LENGTH);

    pushValue((unsigned char*) &send_buffer[34], length, 2);
    memcpy(buffer, dirName.c_str(), length);
    aesEncrypt((unsigned char*)buffer, (unsigned char*) &send_buffer[36], length);

    send(client_socket, send_buffer, BUFFER_LENGTH, 0);
}
