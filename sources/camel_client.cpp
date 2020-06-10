#include "camel_client.h"

camel_client::camel_client() {
    server_key = RSA_new();
    client_key = RSA_new();
}

camel_client::~camel_client() {
    RSA_free(server_key);
    RSA_free(client_key);
    closesocket(client_socket);

    if (! fm) {
        delete fm;
        fm = nullptr;
    }

    if (! tp) {
        delete tp;
        tp = nullptr;
    }
}

void camel_client::signUser(QString username, QString password) {
    if(! vaildUser(username, password)) return ;
    if (!firstConnect) fnFirstConnect();
    WSADATA s;
    SOCKET clientSocket;
    struct sockaddr_in sin;
    int n, statusCode;
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH], buffer[BUFFER_LENGTH];

    if (WSAStartup(MAKEWORD(2, 2), &s) != 0) {
        // todo: notice error
        return ;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        // todo: notice error
        return ;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(loginPort);
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if ( ::connect(clientSocket, (struct sockaddr*)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        //todo: notice error
        closesocket(clientSocket);
        return ;
    }

    setStatusCode(SECOND_CONNECT, send_buffer);

    QByteArray t = username.toLocal8Bit();
    memcpy(buffer, t.data(), USERNAME_LENGTH);
    t = password.toLocal8Bit();
    memcpy(&buffer[USERNAME_LENGTH], t.data(), PASSWORD_LENGTH);
    unsigned char *pos = (unsigned char*)&buffer[USERNAME_LENGTH + PASSWORD_LENGTH];

    BIGNUM* bignum = BN_new();
    BN_set_word(bignum, RSA_F4);
    RSA_generate_key_ex(client_key, 2048, bignum, NULL);
    BN_free(bignum);
    i2d_RSAPublicKey(client_key, &pos);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, 302);
    SHA256_Final(hash, &sha256);
    memcpy(&buffer[USERNAME_LENGTH + PASSWORD_LENGTH], hash, SHA256_DIGEST_LENGTH);
    memcpy(key, hash, 32);

    int result = RSA_public_encrypt(96,(unsigned char*) buffer, (unsigned char*)&send_buffer[2], server_key, RSA_PKCS1_PADDING);
    if (result < 0) {
        ERR_print_errors_fp(stdout);
    }

    send(clientSocket, send_buffer, BUFFER_LENGTH, 0);
    qDebug() << "login require send";

    while (1) {
        n = recv(clientSocket, recv_buffer, BUFFER_LENGTH, 0);
        if (n != -1) {
            getStatusCode(statusCode, recv_buffer);
            if (statusCode == SERVER_SECOND_CONNECT) {
                aesDecrypt((unsigned char*)&recv_buffer[2], (unsigned char*)buffer, 34);
                filePort = (int)(unsigned char) buffer[0];
                filePort = (filePort << 8) | (int)(unsigned char) buffer[1];
                setToken(&buffer[2]);
                tp = new Transporter(key, token, filePort);
                std::thread(&Transporter::threadInstance, tp).detach();
                client_socket = clientSocket;
                fm = new FileManager(clientSocket, token, key);
                loginSuccess();
            }
            break;
        }
    }
}

void camel_client::fnFirstConnect() {
    WSADATA s;
    SOCKET clientSocket;
    struct sockaddr_in sin;
    int n;
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH];

    if (WSAStartup(MAKEWORD(2, 2), &s) != 0) {
        // todo: notice error
        return ;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        // todo: notice error
        return ;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(25565);
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if ( ::connect(clientSocket, (struct sockaddr*)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        //todo: notice error
        closesocket(clientSocket);
        return ;
    }

    setStatusCode(FIRST_CONNECT, send_buffer);
    send(clientSocket, send_buffer, BUFFER_LENGTH, 0);
    int statusCode;
    while (true) {
        n = recv(clientSocket, recv_buffer, BUFFER_LENGTH, 0);
        if (n != -1) {
            getStatusCode(statusCode, recv_buffer);
            if (statusCode == SERVER_FIRST_CONNECT && checkSign(&recv_buffer[2]) == 0) {
                firstConnect = true;
                loginPort = 0;
                loginPort |= ((int)(unsigned char) recv_buffer[2]) << 8;
                loginPort |= (int)(unsigned char) recv_buffer[3];
                const unsigned char* rsa_start = (unsigned char*)&recv_buffer[4];
                server_key = d2i_RSAPublicKey(nullptr, &rsa_start, 270);
                client_socket = clientSocket;
            }
            break;
        }
    }
}

QString camel_client::getDirInfo() {
    std::string dirInfo;
    char send_buffer[BUFFER_LENGTH], recv_buffer[BUFFER_LENGTH], buffer[BUFFER_LENGTH];
    int length, cipherLength = 0, statusCode;
    memset(recv_buffer, 0, sizeof (recv_buffer));
    dirInfo.clear();
    setStatusCode(REFRESH_DIR, buffer);
    memcpy((unsigned char*) &buffer[2], token, TOKEN_LENGTH);

    aesEncrypt((unsigned char*) buffer, (unsigned char*)send_buffer, 34);
    send(client_socket, send_buffer, BUFFER_LENGTH, 0);

    while (true) {
        length = recv(client_socket, recv_buffer, 4096, 0);
        if (length == -1) {
            if (checkTimeout(10)) break;
            continue;
        }
        aesDecrypt((unsigned char*)recv_buffer, (unsigned char*)buffer, BUFFER_LENGTH);
        getStatusCode(statusCode, buffer);
        if (statusCode == SERVER_INFO_END) break;
        if (statusCode != SERVER_DIR_INFO) continue;
        cipherLength = (int)(unsigned char) buffer[2];
        cipherLength = (cipherLength << 8) | (int)(unsigned char) buffer[3];

        for (int i = 0; i < cipherLength; i++) {
            dirInfo.push_back(buffer[i + 4]);
        }

        setStatusCode(RECEIVE_SUCCESS, buffer);
        aesEncrypt((unsigned char*)buffer, (unsigned char*)send_buffer, 2);
        send(client_socket, send_buffer, BUFFER_LENGTH, 0);
    }

    return QString::fromStdString(dirInfo);
}

void camel_client::setBufferToken(char *buffer) {
    memcpy(buffer, token, 32);
}

bool camel_client::vaildUser(QString &username, QString &password) {
    if (username.length() > 16 || password.length() > 16)  return false;
    return true;
}

void camel_client::setStatusCode(const int &_statusCode, char *buffer) {
    buffer[0] = (_statusCode >> 8) & 0xff;
    buffer[1] = _statusCode & 0xff;
}

void camel_client::getStatusCode(int &_statusCode, const char *buffer) {
    _statusCode = (int)(unsigned char)buffer[0];
    _statusCode = (_statusCode << 8) | (int)(unsigned char) buffer[1];
}


bool camel_client::checkSign(const char *_buffer) {
    unsigned char buffer[300], hash[SHA256_DIGEST_LENGTH];
    memcpy(buffer, &_buffer[2], 278);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, 278);
    SHA256_Final(hash, &sha256);
    return memcmp(&_buffer[280], hash, SHA256_DIGEST_LENGTH);
}

void camel_client::setToken(char *buffer) {
    memcpy(token, buffer, 32);
}

void camel_client::clearIv() {
    memset(iv, 0, 16);
}

void camel_client::aesEncrypt(const unsigned char *in, unsigned char *out, int len) {
    clearIv();
    AES_set_encrypt_key(key, 256, &aesKey);
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_ENCRYPT);
}

void camel_client::aesDecrypt(const unsigned char *in, unsigned char *out, int len) {
    clearIv();
    AES_set_decrypt_key(key, 256, &aesKey);
    AES_cbc_encrypt(in, out, len, &aesKey, iv, AES_DECRYPT);
}

bool camel_client::checkTimeout(long long timeLimit) {
    long long delta = time(nullptr) - lastTimestamp;
    return delta >= timeLimit;
}

void camel_client::clickTime() {
    lastTimestamp = time(nullptr);
}

void camel_client::createDirectory(QString _dirName) {
    if(fm -> createDirectory(_dirName)) createDirSuccess();
    else createDirError();
}

void camel_client::deleteDirectory(QString _dirName) {
    if (fm -> deleteDirectory(_dirName)) deleteDirSuccess();
    else deleteDirError();
}

void camel_client::openDirectory(QString _dirName) {
    if (fm -> openDirectory(_dirName)) enterDirSuccess();
    else enterDirError();
}

void camel_client::backupDirectory() {
    if (fm -> backupDirectory()) backupSuccess();
    else backupError();
}


void camel_client::rename(QString _originName, QString _newName) {
    if (fm -> rename(_originName, _newName)) renameSuccess();
    else renameError();
}

QString camel_client::getQueueInfo() {
    return QString::fromStdString(tp -> getQueueInfo());
}

unsigned long long camel_client::getFileSize(const char *_filePath) {
    struct stat statbuf;
    if (stat(_filePath, &statbuf) == 0) {
        return statbuf.st_size;
    }
    return -1;
}

void camel_client::deleteFile(QString _fileName) {
    if (fm -> deleteFile(_fileName)) deleteSuccess();
    else deleteError();
}

void camel_client::downloadFile(QString _fileName) {
    std::string fileName = _fileName.toStdString();
    std::string originPath = fm -> getNowPath() + '/' + fileName;
    std::string destination("./");
    destination.append(fileName);
    Task task(taskType::DOWNLOAD, fileName, originPath, destination);
    tp -> addTask(task);
}

void camel_client::uploadFile(QString _filePath) {
    std::string filePath = _filePath.toStdString();
    std::string fileName = filePath.substr(filePath.find_last_of('/') + 1);
    std::string destinationPath = fm -> getNowPath() + '/' + fileName;
    Task task(taskType::UPLOAD, fileName, std::string(_filePath.toLocal8Bit()), destinationPath);
    task.setSize(getFileSize(_filePath.toLocal8Bit()));
    tp -> addTask(task);
}

void camel_client::pauseTask(int _index) {
    tp -> pauseTask(_index);
}

void camel_client::startTask(int _index) {
    tp -> startTask(_index);
}

void camel_client::deleteTask(int _index, bool isRunning) {
    if (_index == 0 && isRunning) return ;
    if (isRunning) tp -> deleteTask(_index - 1, Type::RUNNING);
    else tp -> deleteTask(_index, Type::PAUSED);
}

void camel_client::stopTask() {
    tp -> stopTask();
}
