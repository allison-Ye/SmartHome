#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"
int create_queue(Queue_Handle * handle)
{
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;
    int status = 0;
    handle->curRd = 0;
    handle->curWr = 0;
    handle->count = 0;
    handle->len = 3;
    handle->queue = (unsigned int *)malloc(sizeof(unsigned int) * handle->len);
    
    if(handle->queue == NULL)
    {
        printf("%s:malloc err!\n",__func__);
        return -1;
    }
    status |= pthread_mutexattr_init(&mutex_attr);
    status |= pthread_condattr_init(&cond_attr);
    status |= pthread_mutex_init(&handle->mutex, &mutex_attr);
    status |= pthread_cond_init(&handle->condRd, &cond_attr);
    status |= pthread_cond_init(&handle->condWr, &cond_attr);
    
    if(status)
    {
        printf("pthread err!\n");
        return -1;
    }
    pthread_condattr_destroy(&cond_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    
    return status;
}
int exit_queue(Queue_Handle * handle)
{
    if(handle->queue != NULL)
    {
        free(handle->queue);
        handle->queue = NULL;
    }
    pthread_cond_destroy(&handle->condRd); 
    pthread_cond_destroy(&handle->condWr);
    pthread_mutex_destroy(&handle->mutex);
    return 0; 
}

int put_queue(Queue_Handle * handle, unsigned int value)
{
    int status = 0;
    pthread_mutex_lock(&handle->mutex);
    
    while(1)
    {
        if(handle->count < handle->len)
        {
            handle->queue[handle->curWr] = value;
            handle->curWr++;
            handle->curWr = handle->curWr % handle->len;
            handle->count++;
            pthread_cond_signal(&handle->condRd);
            break;
        }
        else
        {
            status = pthread_cond_wait(&handle->condWr, &handle->mutex);
        }
    }
    pthread_mutex_unlock(&handle->mutex);
    
    return status;
}
int get_queue(Queue_Handle * handle, unsigned int *value)
{
    int status = 0;
    
    pthread_mutex_lock(&handle->mutex);
    
    while(1)
    {
        if(handle->count > 0)
        {
            if(value != NULL)
                *value=handle->queue[handle->curRd];
            handle->curRd ++;
            handle->curRd = handle->curRd % handle->len;
            handle->count--;
            pthread_cond_signal(&handle->condWr);
            break;
        }
        else
        {
            status = pthread_cond_wait(&handle->condRd, &handle->mutex);
        }

    }
    pthread_mutex_unlock(&handle->mutex);
}


