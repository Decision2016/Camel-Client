#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <winsock2.h>
#include <QString>
#include <string>
#include <openssl/aes.h>

#include "constants.h"

class FileManager
{
public:
    FileManager(SOCKET _client, unsigned char* _token, unsigned char* _key);

    bool createDirectory(QString _dirName);
    bool deleteDirectory(QString _dirName);
    bool openDirectory(QString _dirName);
    bool backupDirectory();
    bool rename(QString &_originName, QString &_newName);

    void deleteFile(QString _fileNmae);

    std::string getNowPath();
    bool checkFileExist(QString &_fileName);
private:
    AES_KEY aesKey;
    SOCKET client_socket;
    unsigned char token[TOKEN_LENGTH], key[KEY_LENGTH], iv[16];
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH], buffer[BUFFER_LENGTH];

    inline void clearBuffer();
    inline void clearIv();
    void aesEncrypt(unsigned char* in, unsigned char* out, int len);
    void aesDecrypt(unsigned char* in, unsigned char* out, int len);
    void setToken(unsigned char* buffer);
    void sendDirInfo(const int &statusCode, QString _dirName);

    static void pushValue(unsigned char *buffer, const long long &value, const int bytes_len);
    static void popValue(const unsigned char *buffer, long long &value, const int bytes_len);
};

#endif // FILEMANAGER_H
