#ifndef BASECLASS_H
#define BASECLASS_H

#include <winsock2.h>
#include <openssl/aes.h>

#include "constants.h"

class BaseClass
{
public:
    BaseClass();
    void aesEncrypt(const unsigned char* in, unsigned char* out, int len);
    void aesDecrypt(const unsigned char* in, unsigned char* out, int len);
    void setSocket(SOCKET _socket);
    void setToken(char* _token);
    void setKey(char* _key);

    static void pushValue(unsigned char *buffer, const long long &value, const int bytes_len);
    static void popValue(const unsigned char *buffer, long long &value, const int bytes_len);
private:
    AES_KEY aesKey;
protected:
    unsigned char token[TOKEN_LENGTH], key[KEY_LENGTH], iv[IV_LENGTH];
    SOCKET thread_socket;
};

#endif // BASECLASS_H
