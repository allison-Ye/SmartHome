#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct
{
    unsigned int len;
    unsigned int count;
    unsigned int curRd;
    unsigned int curWr;

    unsigned int * queue;
    
    pthread_mutex_t mutex;
    pthread_cond_t condRd;
    pthread_cond_t condWr;


}Queue_Handle;

#endif
