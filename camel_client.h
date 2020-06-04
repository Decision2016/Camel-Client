#ifndef CAMEL_CLIENT_H
#define CAMEL_CLIENT_H

#include <QObject>
#include <qqml.h>
#include <QDebug>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstring>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>

class camel_client : public QObject
{
    Q_OBJECT
public:
    camel_client();
    ~camel_client();
public slots:
    void signUser(QString username, QString password);
private:
    RSA *server_key, *client_key;
    AES_KEY aesKey;
    SOCKET client_socket;
    unsigned char token[32];
    int loginPort;
    bool firstConnect = false;

    bool vaildUser(QString &username, QString &password);
    void fnFirstConnect();
    void setToken(char* buffer);

    static void setStatusCode(const int &_statusCode, char* buffer);
    static void getStatusCode(int &_statusCode,const char* buffer);
    static bool checkSign(const char *_buffer);
};

#endif // CAMEL_CLIENT_H
