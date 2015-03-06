#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <common.h>
#include <msg.h>

static int msg_init(GlobalHandle *handle,int key)
{
     int qid;
     if((qid = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) == -1)
     {
         if((qid = msgget(key, IPC_CREAT|0666)) ==  -1)
             return qid;
     }
     handle->qid = qid;
     return qid;
}

static void msg_exit(GlobalHandle *handle)
{
    msgctl(handle->qid, IPC_RMID, NULL);
}

static int shm_init(GlobalHandle *handle,int key)
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
    handle->mid = shmid;
    handle->share_memory = share_memory;
    return 0;

}

static void shm_exit(GlobalHandle * handle)
{
    shmdt(handle->share_memory);
    shmctl(handle->mid, IPC_RMID, NULL);
}

int handle_init(GlobalHandle * handle)
{
    int ret;
    if(handle == NULL)
    {
        printf("%s: handle is NULL!\n",__func__);
        return -1;
    }
    memset(handle, 0, sizeof(GlobalHandle));
    
    handle->qid = msg_init(handle, MSGID);
    if((handle)->qid < 0)
    {
        perror("msg_init error");
        return handle->qid;
    }
    handle->mid = shm_init(handle, SHMKEY);
    if(handle->mid < 0)
    {
        perror("shm_init error");
        return handle->mid;
    }
    handle->mfc = (mfc_handle * )malloc(sizeof(mfc_handle));
    if(handle->mfc == NULL)
    {
        perror("mfc malloc!");
        return -1;
    }
    ret = 0;
    ret |= create_queue(&handle->capture_emptyque);
    ret |= create_queue(&handle->capture_fullque);
    ret |= create_queue(&handle->display_emptyque);
    ret |= create_queue(&handle->display_fullque);
    ret |= create_queue(&handle->encoder_emptyque);
    ret |= create_queue(&handle->encoder_fullque);
 
    if(ret)
    {
        printf("create queue error!\n");
        return -1;
    }  
    ret = pthread_mutex_init(&handle->enc_parm.lock, NULL);
    if(ret)
    {
        printf("init encoder lock error!\n");
        return -1;
    }
    return 0;
}

void handle_exit(GlobalHandle *handle)
{
     if(handle == NULL)
     {
         printf("%s:handle is NULL!\n",__func__);
         return;
     }
     msg_exit(handle);
     shm_exit(handle);
}
int getMsgData(GlobalHandle * handle,int offset,int size, void * addr)
{
    unsigned char * share_memory = handle->share_memory;
    memcpy(addr, share_memory+offset, size);
}
int setMsgData(GlobalHandle * handle, int offset, int size, void * addr)
{
    unsigned char * share_memory = handle->share_memory;
    memcpy(share_memory+offset, addr, size);
}

void swap_cmd(Msgbuf * msgbuf)
{
    int temp;
    temp = msgbuf->cmd_src;
    msgbuf->cmd_src = msgbuf->cmd_dst;
    msgbuf->cmd_dst = temp;
    msgbuf->msgtype = msgbuf->cmd_dst;
}

