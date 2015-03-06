#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
//#include <jpeglib.h>
#include <linux/fb.h>
#include <pthread.h>

#include "captureThread.h"
#include "displayThread.h"
#include "mfc.h"
#include "mem_mng.h"

#include <msg_proc.h>
#include <common.h>
#include <sht_dev.h>

void *capture_thread(void *args);
void *encoder_thread(void *args);
void *stream_thread(void * args);
void * alarm_thread(void * arg);

int main(int argc, char** argv)
{   

    GlobalHandle * global_handle = NULL;
    int r,frame;
    r = sys_init();
    if(r)
    {
        printf("sys_init error!\n");
        return 0;
    }
    global_handle = (GlobalHandle *)malloc(sizeof(GlobalHandle));
    if(global_handle == NULL)
    {
        printf("malloc error!\n");
        return -1;
    }
    r = handle_init(global_handle);
    printf("handle_init!\n");
    if(r)
    {
        printf("%s:handle_init error!\n",__func__);
        return 0;
    }
    global_handle->width = 400; //encoder
    global_handle->height = 300; 
    global_handle->disp_width = 400; //display
    global_handle->disp_height = 300;
    r = sht_dev_init(global_handle);
    if(r)
    {
        printf("%s:sht_dev_init!\n",__func__);
        return 0;
    }
    r = leds_dev_init(global_handle);
    if(r)
    {
        printf("%s:leds_dev_init\n",__func__);
        return 0;
    }
    r = gy_dev_init(global_handle);
    if(r)
    {
        printf("%s:gy_dev_init\n",__func__);
        return 0;
    }
    r = motor_dev_init(global_handle);
    if(r)
    {
        printf("%s:motor_dev_init\n",__func__);
        return 0;
    }

    r = auctl_dev_init(global_handle);
    if(r)
    {
        printf("%s:auctl_dev_init\n",__func__);
        return 0;
    }
    r = rfid_dev_init(global_handle);
    if(r)
    {
        printf("%s:rfid_dev_init\n",__func__);
        return 0;
    }
    r = pwm_dev_init(global_handle);
    if(r)
    {
        printf("%s:pwm_dev_init\n",__func__);
        return 0;
    }
/////////////////////////////////////////////////
////////////////capture init///////////////////////
////////////////////////////////////////////////
    fb_init();
    capture_open();
    if(!capture_init(global_handle))
    {
        printf("capture_init error!\n");
        return 0;
    }
    captureStart();
//////////////////////////////////////////////
///////////////Encoder init/////////////////////
///////////////////////////////////////////////////
    r = initMFC(global_handle->mfc, global_handle->width, global_handle->height, 30);
    if(r != 1)
    {
        printf("initMFC error!\n");
        return -1;
    }

////////////////////////////////////////////////
///////////////Display init/////////////////////////
/////////////////////////////////////////////////////

    display_open();
    if(!display_init(global_handle))
    {
        printf("display_init error!\n");
        return 0;
    }
    displayStart();

/////////////////////////////////////////////
////////////////begin thread//////////////////
/////////////////////////////////////////////
    pthread_create(&global_handle->capture_thread_t, NULL ,
        capture_thread, (void *)global_handle);
    pthread_create(&global_handle->display_thread_t, NULL ,
        display_thread, (void *)global_handle);
    pthread_create(&global_handle->encoder_thread_t, NULL ,
        encoder_thread, (void *)global_handle);
    pthread_create(&global_handle->procset_thread_t, NULL, 
        proc_set, (void *)global_handle);
    pthread_create(&global_handle->procget_thread_t, NULL, 
        proc_get, (void *)global_handle);
    pthread_create(&global_handle->stream_thread_t, NULL, 
        stream_thread, (void *)global_handle);
    pthread_create(&global_handle->alarm_thread_t, NULL, 
        alarm_thread, (void *)global_handle);
    while(1)
    {
        sleep(1);
    }
    pthread_join(global_handle->capture_thread_t,NULL);
    pthread_join(global_handle->display_thread_t,NULL);
    pthread_join(global_handle->encoder_thread_t,NULL);
    pthread_join(global_handle->procset_thread_t, NULL);
    pthread_join(global_handle->procget_thread_t, NULL);
    handle_exit(global_handle);
    free(global_handle);
    printf("*****quit\n");

}

