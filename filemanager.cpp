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
    std::string dirName = _dirName.toStdString();
    clearBuffer();
    int length = dirName.length();
    long long statusCode;

    pushValue((unsigned char*) send_buffer, CREATE_DIR, 2);
    aesEncrypt(token,(unsigned char*) &send_buffer[2], TOKEN_LENGTH);

    pushValue((unsigned char*) &send_buffer[34], length, 2);
    memcpy(buffer, dirName.c_str(), length);
    aesEncrypt((unsigned char*)buffer, (unsigned char*) &send_buffer[36], length);

    send(client_socket, send_buffer, BUFFER_LENGTH, 0);

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
