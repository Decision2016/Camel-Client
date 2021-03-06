#ifndef CONSTANTS_H
#define CONSTANTS_H

const int PATH_LENGTH = 32;
const int STATUS_LENGTH = 2;
const int USERNAME_LENGTH = 32;
const int PASSWORD_LENGTH = 32;
const int KEY_LENGTH = 32;
const int TOKEN_LENGTH = 32;
const int IV_LENGTH = 16;
const int AES_KEY_LENGTH = 256;
const int BUFFER_LENGTH = 1440;
const int RSA_KEY_LENGTH = 270;
const int RSA_KEY_BIT_LENGTH = 2048;
const int DATA_START = 34;
const unsigned long long ONCE_MAX_LENGTH = 1392ULL;


/*
 * 与服务端交互状态码定义
 */
const int REFRESH_DIR = 200;
const int ENTER_DIR = 201;
const int BACKUP_DIR = 202;
const int DELETE_DIR = 203;
const int CREATE_DIR = 204;
const int RENAME_DIR_FILE = 205;
const int FIRST_CONNECT = 210;
const int SECOND_CONNECT = 211;
const int CONNECTION_END = 212;
const int FILE_UPLOAD = 220;
const int CLIENT_FILE_END = 221;
const int FILE_DELETE = 222;
const int FILE_PAUSED = 223;
const int FILE_DOWNLOAD = 224;
const int FILE_STOP = 225;
const int FILE_TRANSPORT = 226;
const int FILE_GET_PATH = 227;
const int RECEIVE_SUCCESS = 402;
/*
 * 服务端返回状态码
 */
const int SERVER_DIR_INFO = 100;
const int SERVER_NOT_EXISTED = 101;
const int SERVER_DELETE = 102;
const int SERVER_DIR_CREATED = 104;
const int SERVER_EXISTED = 105;
const int SERVER_FIRST_CONNECT = 110;
const int SERVER_SECOND_CONNECT = 111;
const int SERVER_PASS_ERROR = 112;
const int SERVER_USER_NOT_EXIST= 113;
const int SERVER_CLOSE = 120;
const int SERVER_TIMEOUT = 121;
const int SERVER_RENAME = 131;
const int SERVER_RENAME_ERROR = 132;
const int SERVER_FILE_RECEIVE = 133;
const int SERVER_FILE_SEND = 134;
const int SERVER_FILE_TRANSPORT = 135;
const int SERVER_FILE_FINISHED = 136;
const int SERVER_FILE_PATH = 137;
const int SERVER_INFO_END = 504;

enum Type{RUNNING, PAUSED};
enum taskType {HEAD, UPLOAD, DOWNLOAD};
#endif // CONSTANTS_H
