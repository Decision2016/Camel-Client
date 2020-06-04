#include "camel_client.h"

camel_client::camel_client() {
    server_key = RSA_new();
    client_key = RSA_new();
}

camel_client::~camel_client() {
    RSA_free(server_key);
    RSA_free(client_key);
}

void camel_client::signUser(QString username, QString password) {
    if(! vaildUser(username, password)) return ;
    if (!firstConnect) fnFirstConnect();
    WSADATA s;
    SOCKET clientSocket;
    struct sockaddr_in sin;
    int n, statusCode;
    char send_buffer[4096], recv_buffer[4096], buffer[4096];

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

    setStatusCode(211, send_buffer);

    QByteArray t = username.toLocal8Bit();
    memcpy(buffer, t.data(), 16);
    t = password.toLocal8Bit();
    memcpy(&buffer[16], t.data(), 16);
    unsigned char *pos = (unsigned char*)&buffer[32];

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
    memcpy(&buffer[32], hash, SHA256_DIGEST_LENGTH);
    AES_set_encrypt_key(hash, 256, &aesKey);

    int result = RSA_public_encrypt(64,(unsigned char*) buffer, (unsigned char*)&send_buffer[2], server_key, RSA_PKCS1_PADDING);
    if (result < 0) {
        ERR_print_errors_fp(stdout);
    }

    send(clientSocket, send_buffer, 4096, 0);
    qDebug() << "login requir send";

    while (1) {
        n = recv(clientSocket, recv_buffer, 4096, 0);
        if (n != -1) {
            getStatusCode(statusCode, recv_buffer);
            if (statusCode == 112) {
                AES_decrypt((unsigned char*)&recv_buffer[2], (unsigned char*)buffer, &aesKey);
                setToken(buffer);
                client_socket = clientSocket;
                qDebug() << "Login successful!";
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
    char send_buffer[4096], recv_buffer[4096];

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

    setStatusCode(210, send_buffer);
    send(clientSocket, send_buffer, 2, 0);
    int statusCode;
    while (1) {
        n = recv(clientSocket, recv_buffer, 4096, 0);
        if (n != -1) {
            getStatusCode(statusCode, recv_buffer);
            if (statusCode == 110 && checkSign(&recv_buffer[2]) == 0) {
                firstConnect = true;
                loginPort = 0;
                loginPort |= ((int)(unsigned char) recv_buffer[2]) << 8;
                loginPort |= (int)(unsigned char) recv_buffer[3];
                qDebug() << loginPort;
                const unsigned char* rsa_start = (unsigned char*)&recv_buffer[4];
                server_key = d2i_RSAPublicKey(nullptr, &rsa_start, 270);
                // notice info
                qDebug() << "first Connect Finished!";
            }
            break;
        }
    }
    closesocket(clientSocket);
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
