#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "common.h"
#include "mfc.h"
#include "mem_mng.h"
#include "queue.h"
extern FILE * fp;

void *encoder_thread(void * args)
{
    int frame = 1;
    struct timeval tv1,tv2;
    
    GlobalHandle *handle= (GlobalHandle *)args;
    encoder_parm * parm = &handle->enc_parm;
    void **pY_addr = &parm->Y_addr;
    void **pUV_addr = &parm->UV_addr;
    void **pdata_addr = &parm->data_addr;
    unsigned char **pheader = &parm->h264header_addr;
    unsigned long  *headerSize = &parm->header_size; 
    unsigned long  *readSize = &parm->data_size;
    unsigned long *totalSize = &parm->total_size;

    int width = handle->width;
    int height = handle->height;
    int length = handle->length;
    mfc_handle * mfc = handle->mfc;
    *headerSize=getHeader(mfc,pheader);
    getInputBuf(mfc, pY_addr, pUV_addr);
    gettimeofday(&tv1, NULL);
    while(1)
    {
#ifndef QUEUE_MODE
        ca_mem_read(*pY_addr, 0, length * 2 / 3);
	ca_mem_read(*pUV_addr, length * 2 / 3, length / 3);
#else
        unsigned char * addr;
        get_queue(&handle->display_fullque, (unsigned int *)&addr);
        memcpy(*pY_addr, addr, length * 2 / 3);
        memcpy(*pUV_addr, addr + length * 2 / 3, length / 3);
#endif
        pthread_mutex_lock(&parm->lock);	
        *readSize=encode(mfc,pdata_addr);
        pthread_mutex_unlock(&parm->lock);
        if((frame % 1000) == 0)
        {
            gettimeofday(&tv2, NULL);
            printf("encoderThread:%d frames, %d fps\n",
                frame, 1000 / (tv2.tv_sec - tv1.tv_sec));
            gettimeofday(&tv1, NULL);
        }
        frame++;
    }
}
