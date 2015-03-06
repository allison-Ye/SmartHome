#ifndef _MSG_H_
#define _MSG_H_

#define MSGID 0xdf233125
enum{
    MSG_TYPE_SERVERSET = 1,
    MSG_TYPE_SERVERGET,
    MSG_TYPE_TEST,
    MSG_TYPE_BOA,
    MSG_TYPE_GUI,
    MSG_MAX_NUM,
};
enum{
    CMD_GET_TEMP = 0,
    CMD_GET_HUMIDITY, 
    CMD_GET_AUDIOCTRL,
    CMD_GET_LIGHT,
    CMD_GET_RFID,
    CMD_GET_ALARM,

/////////SET///////////////////////

    CMD_SET_LED1_ENABLE,
    CMD_SET_LED2_ENABLE,
    CMD_SET_MOTOR_DIR,
    CMD_SET_MOTOR_STOP,
    CMD_SET_BEEPER,
    CMD_MAX_NUM,
};

typedef struct
{
    long msgtype;
    unsigned int cmd;
    unsigned int cmd_src;
    unsigned int cmd_dst;
    unsigned int offset;
    unsigned int size;
}Msgbuf;

#define SHARE_SIZE (CMD_MAX_NUM * MSG_MAX_NUM * 128)
#define SHMKEY 0x22553311
#endif
