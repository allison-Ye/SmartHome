#ifndef _MNG_MEM_H_
#define _MNG_MEM_H_


typedef struct
{
    long cmd;
    char temp[10];
}MSGBUF;
#define MSG_TYPE_CAPTURE 1
#define MSG_TYPE_DISPLAY 2
#define MSG_TYPE_ENCODER 3
#define MSG_TYPE_LIVE    4

#ifdef __cplusplus
extern "C" {
#endif

int sys_init();
int mem_write(void * addr, unsigned long offset ,unsigned long size);
int mem_read(void * addr, unsigned long offset,unsigned long size);
int ca_mem_write(void * addr, unsigned long offset ,unsigned long size);
int ca_mem_read(void * addr, unsigned long offset,unsigned long size);

int msg_rcv_from_live();
int msg_rcv_from_encoder();
int msg_send_to_live();
int msg_send_to_encoder();
int send_cmd_new_data(int dst);
int wait_cmd_new_data(int myid);
#ifdef __cplusplus
}
#endif

#endif 
