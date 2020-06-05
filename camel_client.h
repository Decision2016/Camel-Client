#ifndef CAMEL_CLIENT_H
#define CAMEL_CLIENT_H

#include <QObject>
#include <qqml.h>
#include <QDebug>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstring>
#include <string>
#include <ctime>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>

static const int MAX_TIME_WAITING = 300;

class camel_client : public QObject
{
    Q_OBJECT
public:
    camel_client();
    ~camel_client();
public slots:
    void signUser(QString username, QString password);
    QString getDirInfo();
signals:
    void loginSuccess();
private:
    RSA *server_key, *client_key;
    AES_KEY aesKey;
    SOCKET client_socket;
    unsigned char token[32], key[32], iv[16];
    int loginPort;
    bool firstConnect = false;
    long long lastTimestamp;

    bool vaildUser(QString &username, QString &password);
    void fnFirstConnect();
    void setToken(char* buffer);
    void clearIv();
    void aesEncrypt(const unsigned char* in, unsigned char* out, int len);
    void aesDecrypt(const unsigned char* in, unsigned char* out, int len);

    inline void setBufferToken(char *buffer);
    inline bool checkTimeout(long long timeLimit = MAX_TIME_WAITING);
    inline void clickTime();

    static void setStatusCode(const int &_statusCode, char* buffer);
    static void getStatusCode(int &_statusCode,const char* buffer);
    static bool checkSign(const char *_buffer);
};

#endif // CAMEL_CLIENT_H
