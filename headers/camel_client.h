#ifndef CAMEL_CLIENT_H
#define CAMEL_CLIENT_H

#include <QObject>
#include <QSettings>
#include <qqml.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <sys/stat.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <ctime>
#include <thread>
#include "filemanager.h"
#include "constants.h"
#include "task.h"
#include "transporter.h"

#include <iostream>

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
    void stopTask();
    void deleteTask(int _index, bool isRunning);

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
    std::string ip;
    unsigned char token[32], key[32], iv[16];
    int loginPort, filePort, port;
    bool firstConnect = false, secondConnect = false;
    long long lastTimestamp;

    bool vaildUser(QString &username, QString &password);
    void fnFirstConnect();
    void setToken(char* buffer);
    void clearIv();
    void aesEncrypt(const unsigned char* in, unsigned char* out, int len);
    void aesDecrypt(const unsigned char* in, unsigned char* out, int len);
    void settingInit();

    inline void setBufferToken(char *buffer);
    inline bool checkTimeout(long long timeLimit = MAX_TIME_WAITING);
    inline void clickTime();

    static unsigned long long getFileSize(const char *_filePath);
    static void setStatusCode(const int &_statusCode, char* buffer);
    static void getStatusCode(int &_statusCode,const char* buffer);
    static bool checkSign(const char *_buffer);
};

#endif // CAMEL_CLIENT_H
