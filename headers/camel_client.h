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
#include <thread>
#include <sys/stat.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include "filemanager.h"
#include "constants.h"
#include "task.h"
#include "transporter.h"
#include <errno.h>

static const int MAX_TIME_WAITING = 300;

class camel_client : public QObject
{
    Q_OBJECT
public:
    camel_client();
    ~camel_client();
public slots:
    void signUser(QString username, QString password);
    void createDirectory(QString _dirName);
    void deleteDirectory(QString _dirName);
    void openDirectory(QString _dirName);
    void rename(QString _originName, QString _newName);
    void backupDirectory();

    void deleteFile(QString _fileName);
    void downloadFile(QString _fileName);
    void uploadFile(QString _filePath);

    void startTask(int _index);
    void pauseTask(int _index);

    QString getDirInfo();
    QString getQueueInfo();
signals:
    void loginSuccess();
    void createDirSuccess();
    void createDirError();
    void deleteDirSuccess();
    void deleteDirError();
    void enterDirSuccess();
    void enterDirError();
    void backupSuccess();
    void backupError();
    void renameSuccess();
    void renameError();
    void deleteSuccess();
    void deleteError();
private:
    RSA *server_key, *client_key;
    AES_KEY aesKey;
    SOCKET client_socket;
    FileManager *fm = nullptr;
    Transporter *tp = nullptr;
    unsigned char token[32], key[32], iv[16];
    int loginPort, filePort;
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

    static unsigned long long getFileSize(const char *_filePath);
    static void setStatusCode(const int &_statusCode, char* buffer);
    static void getStatusCode(int &_statusCode,const char* buffer);
    static bool checkSign(const char *_buffer);
};

#endif // CAMEL_CLIENT_H
