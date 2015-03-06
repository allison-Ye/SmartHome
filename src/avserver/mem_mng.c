#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "mem_mng.h"

#define SHMKEY 0x45786571
#define CA_SHMKEY 0x45786572
#define MSGKEY 0x4457a8d4
#define FRAMESIZE (768*576*2)
int mid;
int qid;
int capture_mid;
pthread_mutex_t mutex;
pthread_mutex_t ca_mutex;

int sys_init()
{
    int ret;
    mid = shmget(SHMKEY, FRAMESIZE, IPC_CREAT|0666);
    if(mid == -1)
    {
        perror("shmget");
        return -1;
    }
	capture_mid = shmget(CA_SHMKEY, FRAMESIZE, IPC_CREAT|0666);
    if(capture_mid == -1)
    {
        perror("shmget");
        return -1;
    }
    qid = msgget(MSGKEY, IPC_CREAT|0666);
    if(qid == -1)
    {
        perror("msgget");
        return -1;
    }
    ret = pthread_mutex_init(&mutex, NULL);
    if(ret < 0)
    {
        printf("mutex init fail!\n");
        return -1;
    }    
	ret = pthread_mutex_init(&ca_mutex, NULL);
    if(ret < 0)
    {
        printf("mutex init fail!\n");
        return -1;
    } 
}
int mem_write(void * addr, unsigned long offset ,unsigned long size)
{
    unsigned char * mem_addr = shmat(mid, NULL, 0);
    if(addr == NULL)
    {
        printf("addr is NULL\n");
        return -1;
    }
    if(mem_addr == NULL)
    {
        printf("shmat error!\n");
        return -1;
    }
    pthread_mutex_lock(&mutex);
    memcpy(mem_addr + offset, addr, size);
    pthread_mutex_unlock(&mutex);
    shmdt(mem_addr);
    return size;
}
int mem_read(void * addr, unsigned long offset,unsigned long size)
{
    unsigned char * mem_addr = shmat(mid, NULL, 0);
    if(addr == NULL)
    {
        printf("addr is NULL\n");
        return -1;
    }
    if(mem_addr == NULL)
    {
        printf("shmat error!\n");
        return -1;
    }
    pthread_mutex_lock(&mutex);
    memcpy(addr, mem_addr + offset, size);
    pthread_mutex_unlock(&mutex);
    shmdt(mem_addr);
    return size;
}
int ca_mem_write(void * addr, unsigned long offset ,unsigned long size)
{
    unsigned char * mem_addr = shmat(capture_mid, NULL, 0);
    if(addr == NULL)
    {
        printf("addr is NULL\n");
        return -1;
    }
    if(mem_addr == NULL)
    {
        printf("shmat error!\n");
        return -1;
    }
    pthread_mutex_lock(&ca_mutex);
    memcpy(mem_addr + offset, addr, size);
    pthread_mutex_unlock(&ca_mutex);
    shmdt(mem_addr);
    return size;
}
int ca_mem_read(void * addr, unsigned long offset,unsigned long size)
{
    unsigned char * mem_addr = shmat(capture_mid, NULL, 0);
    if(addr == NULL)
    {
        printf("addr is NULL\n");
        return -1;
    }
    if(mem_addr == NULL)
    {
        printf("shmat error!\n");
        return -1;
    }
    pthread_mutex_lock(&ca_mutex);
    memcpy(addr, mem_addr + offset, size);
    pthread_mutex_unlock(&ca_mutex);
    shmdt(mem_addr);
    return size;
}

int msg_rcv_from_live()
{
    MSGBUF msgbuf;
    int len;
    len = sizeof(MSGBUF) - sizeof(long);
    msgrcv(mid,&msgbuf, len, MSG_TYPE_ENCODER, 0);
}
int msg_rcv_from_encoder()
{
    MSGBUF msgbuf;
    int len;
    len = sizeof(MSGBUF) - sizeof(long);
    msgrcv(mid,&msgbuf, len, MSG_TYPE_LIVE, 0);
}
int msg_send_to_encoder()
{
    MSGBUF msgbuf;
    int len;
    len = sizeof(MSGBUF) - sizeof(long);
    msgbuf.cmd = MSG_TYPE_ENCODER;
    msgsnd(mid,&msgbuf, len, 0);
} 
int msg_send_to_live()
{
    MSGBUF msgbuf;
    int len;
    len = sizeof(MSGBUF) - sizeof(long);
    msgbuf.cmd = MSG_TYPE_LIVE;
    msgsnd(mid,&msgbuf, len, 0);
}

int send_cmd_new_data(int dst)
{
    MSGBUF msgbuf;
    int len;
    len = sizeof(MSGBUF) - sizeof(long);
    msgbuf.cmd = dst;
    msgsnd(mid, &msgbuf,len,0);
}
int wait_cmd_new_data(int myid)
{
    MSGBUF msgbuf;
    int len;
    len = sizeof(MSGBUF) - sizeof(long);
    msgrcv(mid,&msgbuf, len, myid, 0);
}
 
