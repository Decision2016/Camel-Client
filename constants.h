#ifndef CONSTANTS_H
#define CONSTANTS_H

const int BUFFER_LENGTH = 4096;
const int TOKEN_LENGTH = 32;
const int KEY_LENGTH = 32;
const int AES_KEY_LENGTH = 256;

/*
 * 与服务端交互状态码定义
 */
const int REFRESH_DIR = 200;
const int ENTER_DIR = 201;
const int BACKUP_DIR = 202;
const int DELETE_DIR = 203;
const int CREATE_DIR = 204;


/*
 * 服务端返回状态码
 */
const int SERVER_DIR_INFO = 100;
const int SERVER_DIR_NOT_EXISTED = 101;
const int SERVER_DIR_CREATED = 104;
const int SERVER_DIR_EXISTED = 105;
#endif // CONSTANTS_H
