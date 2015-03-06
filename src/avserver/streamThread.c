#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "queue.h"

void * stream_thread(void * args)
{
    GlobalHandle *handle = (GlobalHandle *)args;
    encoder_parm * parm = &handle->enc_parm;
    void **pdata_addr = &parm->data_addr;
    unsigned char **pheader = &parm->h264header_addr;
    unsigned long  *headerSize = &parm->header_size;
    unsigned long  *readSize = &parm->data_size;
    unsigned long  *totalSize = &parm->total_size;
while(1)
{
    msg_rcv_from_live();
    pthread_mutex_lock(&parm->lock);
    *totalSize = *readSize + *headerSize;
    mem_write(totalSize, 0, sizeof(unsigned long));
    mem_write(*pheader, sizeof(unsigned long), *headerSize);
    mem_write(*pdata_addr, sizeof(unsigned long) + *headerSize,*readSize);
    pthread_mutex_unlock(&parm->lock);
    msg_send_to_live();
}
}
