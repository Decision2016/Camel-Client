#ifndef BASECLASS_H
#define BASECLASS_H

#include <winsock2.h>
#include <openssl/aes.h>

#include "constants.h"

class BaseClass
{
public:
    BaseClass(const unsigned char *_key, const unsigned char *_token);
    unsigned long long recvStatusCode();
    void aesEncrypt(const unsigned char *in, unsigned char *out, int len);
    void aesDecrypt(const unsigned char *in, unsigned char *out, int len);
    void setSocket(SOCKET _socket);
    void setPort(int _port);
    void setToken(unsigned char *destination);
    void sendStatusCode(unsigned long long _statusCode);

    static void pushValue(unsigned char *buffer, const unsigned long long &value, const int bytes_len);
    static void popValue(const unsigned char *buffer, unsigned long long &value, const int bytes_len);
private:
    AES_KEY aesKey;
protected:
    int port;
    unsigned char _buffer[BUFFER_LENGTH];
    unsigned char token[TOKEN_LENGTH], key[KEY_LENGTH], iv[IV_LENGTH];
    SOCKET thread_socket;
};

#endif // BASECLASS_H
