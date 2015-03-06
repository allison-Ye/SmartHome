#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <interface.h>
#include <msg.h>


void cgi_header()
{
    printf("Content type: text/html\n\n");
    printf("<html>\n");
    printf("<head><title>Cloud Sail Smart Home</title></head>\n");
    printf("<body bgcolor=\"#666666\"></body>\n");
}
void cgi_tail()
{
    printf("</html>\n");
    fflush(stdout);
}
static int getMsgData(API_HANDLE *handle, int offset,int size, void * addr)
{
    unsigned char * share_memory = handle->share_memory;
    memcpy(addr, share_memory+offset, size);
}
static int setMsgData(API_HANDLE *handle, int offset, int size, void * addr)
{
    unsigned char * share_memory = handle->share_memory;
    memcpy(share_memory+offset, addr, size);
}

static int msg_init(API_HANDLE * handle,int key)
{
     int qid;
     if((qid = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) < 0)
     {
         if((qid = msgget(key, IPC_CREAT|0666)) < 0)
             return qid;
     }
     handle->qid = qid;
     return qid;
}
static void msg_exit(API_HANDLE *handle)
{
    msgctl(handle->qid, IPC_RMID, NULL);
}


static int  shm_init(API_HANDLE * handle, int key)
{
    int shmid = shmget(key, SHARE_SIZE, IPC_CREAT | IPC_EXCL| 0666);
    unsigned char * share_memory = NULL;
    if(shmid == -1) 
    {   
        shmid = shmget(key, SHARE_SIZE, IPC_CREAT|0666);
        if(shmid == -1) 
            return shmid;
    }   
    share_memory = shmat(shmid, NULL, 0); 
    if(share_memory == NULL)
        return -1; 
    handle->share_memory = share_memory;
    handle->mid = shmid;
    return 0;
    
}
static void shm_exit(API_HANDLE * handle)
{
    shmdt(handle->share_memory);
    shmctl(handle->mid, IPC_RMID, NULL);
}


int GetTempData(API_HANDLE  * handle, float * val)
{
    Msgbuf buf;
    int len;
    float value;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERGET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(value);
    buf.cmd = CMD_GET_TEMP;
    len = sizeof(Msgbuf) - sizeof(long);
    
    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_GET_TEMP\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_GET_TEMP)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_GET_TEMP2\n");
    getMsgData(handle,buf.offset, buf.size, val);   
    pthread_mutex_unlock(&handle->lock);
    
    return 0;
}
int GetHumiData(API_HANDLE  * handle, float * val)
{
    Msgbuf buf;
    int len;
    float value;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERGET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(value);
    buf.cmd = CMD_GET_HUMIDITY;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_GET_HUMIDITY\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_GET_HUMIDITY)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_GET_HUMIDITY2\n");
    getMsgData(handle,buf.offset, buf.size, val);
    pthread_mutex_unlock(&handle->lock);

    return 0;
}
int GetLuxData(API_HANDLE  * handle, float * val)
{
    Msgbuf buf;
    int len;
    float value;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERGET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(value);
    buf.cmd = CMD_GET_LIGHT;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_GET_LIGHT\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_GET_LIGHT)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_GET_LIGHT2\n");
    getMsgData(handle,buf.offset, buf.size, val);
    pthread_mutex_unlock(&handle->lock);

    return 0;
}
int GetAlarmStatus(API_HANDLE  * handle, int * val)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERGET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(int);
    buf.cmd = CMD_GET_ALARM;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_GET_ALARM\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_GET_ALARM)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_GET_ALARM2\n");
    getMsgData(handle,buf.offset, buf.size, val);
    pthread_mutex_unlock(&handle->lock);

    return 0;
}
int GetRFIDData(API_HANDLE  * handle, unsigned char * id)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERGET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = 5;
    buf.cmd = CMD_GET_RFID;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_GET_RFID\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_GET_RFID)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_GET_RFID2\n");
    getMsgData(handle,buf.offset, buf.size, id);
    pthread_mutex_unlock(&handle->lock);

    return 0;
}

int SetLed1Enable(API_HANDLE  * handle, int enable)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERSET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(int);
    buf.cmd = CMD_SET_LED1_ENABLE;
    len = sizeof(Msgbuf) - sizeof(long);
    
    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_SET_LED1_ENABLE\n");
    setMsgData(handle, buf.offset, buf.size, &enable);
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_SET_LED1_ENABLE)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_SET_LED1_ENABLE1\n");
    pthread_mutex_unlock(&handle->lock);
    return 0;
}
int SetLed2Enable(API_HANDLE  * handle, int enable)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERSET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(int);
    buf.cmd = CMD_SET_LED2_ENABLE;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    printf(NAME":send cmd: CMD_SET_LED2_ENABLE\n");
    setMsgData(handle, buf.offset, buf.size, &enable);
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_SET_LED2_ENABLE)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    printf(NAME":send cmd: CMD_SET_LED2_ENABLE1\n");
    pthread_mutex_unlock(&handle->lock);
    return 0;
}

int SetMotorDir(API_HANDLE  * handle, int dir)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERSET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(int);
    buf.cmd = CMD_SET_MOTOR_DIR;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    setMsgData(handle, buf.offset, buf.size, &dir);
    printf(NAME":send cmd: CMD_SET_MOTOR_DIR\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_SET_MOTOR_DIR)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    pthread_mutex_unlock(&handle->lock);
    return 0;
}
int SetMotorStop(API_HANDLE  * handle)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERSET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(int);
    buf.cmd = CMD_SET_MOTOR_STOP;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    //setMsgData(handle, buf.offset, buf.size, &dir);
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    printf(NAME":send cmd: CMD_SET_MOTOR_STOP\n");
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_SET_MOTOR_STOP)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    pthread_mutex_unlock(&handle->lock);
    return 0;
}
int SetBeeperEnable(API_HANDLE  * handle, int enable)
{
    Msgbuf buf;
    int len;
    int qid = handle->qid;
    buf.cmd_dst = MSG_TYPE_SERVERSET;
    buf.msgtype = buf.cmd_dst;
    buf.cmd_src = handle->msgtype;
    buf.size = sizeof(int);
    buf.cmd = CMD_SET_BEEPER;
    len = sizeof(Msgbuf) - sizeof(long);

    buf.offset = (handle->msgtype - 1 ) * CMD_MAX_NUM * 128 + buf.cmd * 128;
    pthread_mutex_lock(&handle->lock);
    setMsgData(handle, buf.offset, buf.size, &enable);
    printf(NAME":send cmd: CMD_SET_BEEPER\n");
    if(msgsnd(qid, &buf, len, 0))
    {
        perror("msgsnd");
        printf("%s: msgsnd error!\n",__func__);
        pthread_mutex_unlock(&handle->lock);
        return -1;
    }
    msgrcv(qid, &buf, len, handle->msgtype, 0);
    while(buf.cmd != CMD_SET_BEEPER)
    {
        printf("%s:cmd error %d,continue msgrcv!\n",__func__,buf.cmd);
        msgrcv(qid, &buf, len, handle->msgtype, 0);
    }
    pthread_mutex_unlock(&handle->lock);
    return 0;
}


int api_init(API_HANDLE * handle, int msgtype)
{
    int qid,mid;
    int ret;
    if(handle == NULL)
    {
        printf(NAME":handle is NULL!\n");
        return -1;
    }
    qid = msg_init(handle, MSGID);
    if(qid < 0)
    {
        perror(NAME":msg_init");
        return -1;
    }
    ret = shm_init(handle, SHMKEY);
    if(ret)
    {
        printf(NAME":shm_init error!\n");
        return -1;
    }
    if(msgtype < 0)
    {
        printf(NAME":msgtype error!\n");
        return -1;
    }
    handle->msgtype = msgtype;
    ret = pthread_mutex_init(&handle->lock, NULL);
    if(ret)
    {
        printf("thread lock init error!\n");
        return -1;
    }
    return 0;
}
void api_exit(API_HANDLE *handle)
{
     if(handle == NULL)
     {
         printf(NAME": %s:handle is NULL!\n",__func__);
         return;
     }
     free(handle);
}

