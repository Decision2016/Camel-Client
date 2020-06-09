#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <winsock2.h>
#include <QString>
#include <string>
#include <openssl/aes.h>

#include "constants.h"
#include "baseclass.h"

class FileManager : public BaseClass {
public:
    FileManager(SOCKET _client, unsigned char* _token, unsigned char* _key);

    bool createDirectory(QString _dirName);
    bool deleteDirectory(QString _dirName);
    bool openDirectory(QString _dirName);
    bool backupDirectory();
    bool rename(QString &_originName, QString &_newName);

    bool deleteFile(QString _fileName);

    std::string getNowPath();
    bool checkFileExist(QString &_fileName);
private:
    AES_KEY aesKey;
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH], buffer[BUFFER_LENGTH];

    inline void clearBuffer();
    void sendDirInfo(const int &statusCode, QString _dirName);
};

#endif // FILEMANAGER_H
