#include "baseclass.h"

BaseClass::BaseClass(const unsigned char *_key, const unsigned char *_token) {
    memcpy(token, _token, TOKEN_LENGTH);
    memcpy(key, _key, KEY_LENGTH);
}

void BaseClass::setSocket(SOCKET _socket) {
    thread_socket = _socket;
}

void BaseClass::setPort(int _port) {
    port = _port;
}

void BaseClass::aesEncrypt(const unsigned char *in, unsigned char *out, int len) {
    memset(iv, 0, IV_LENGTH);
    AES_set_encrypt_key(key, AES_KEY_LENGTH, &aesKey);
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_ENCRYPT);
}

void BaseClass::aesDecrypt(const unsigned char *in, unsigned char *out, int len) {
    memset(iv, 0, IV_LENGTH);
    AES_set_decrypt_key(key, AES_KEY_LENGTH, &aesKey);
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_DECRYPT);
}

void BaseClass::pushValue(unsigned char *buffer, const unsigned long long &value, const int bytes_len) {
    for (int i = 1; i <= bytes_len;i++) {
        buffer[i - 1] = (value >> ((bytes_len - i) * 8)) & 0xff;
    }
}

void BaseClass::popValue(const unsigned char *buffer,unsigned long long &value, const int bytes_len) {
    value = 0;
    for (int i = 0; i < bytes_len; i++) {
        value <<= 8;
        value |= buffer[i];
    }
}

void BaseClass::sendStatusCode(unsigned long long _statusCode) {
    char buffer[BUFFER_LENGTH];
    pushValue(_buffer, _statusCode, STATUS_LENGTH);
    memcpy((unsigned char*) &_buffer[2], token, TOKEN_LENGTH);
    aesEncrypt(_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
    send(thread_socket, buffer, BUFFER_LENGTH, 0);
}

void BaseClass::setToken(unsigned char *destination) {
    memcpy(destination, token, TOKEN_LENGTH);
}
