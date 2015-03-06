#ifndef _COMMON_H_
#define _COMMON_H_

#include <pthread.h>
#include "queue.h"
#include "mfc.h"

#define QUEUE_MODE
typedef struct
{
    void *Y_addr;
    void *UV_addr;
    void *data_addr;
    unsigned char * h264header_addr;
    unsigned long header_size,data_size,total_size;
    pthread_mutex_t lock;
}encoder_parm;
/**************************
Global Handle
***************************/
typedef struct 
{
    int qid;   /* ID for message queue */
    int mid;   /* ID for share memory */
    unsigned char * share_memory; /* Pointer of share memory */

    unsigned int height;
    unsigned int width;   
    unsigned long length;    

    unsigned int disp_width;
    unsigned int disp_height;

    encoder_parm enc_parm; // paramer of encoder.
 
    pthread_t procset_thread_t; 
    pthread_t procget_thread_t; 
    pthread_t capture_thread_t;
    pthread_t encoder_thread_t;
    pthread_t display_thread_t;
    pthread_t stream_thread_t;
    pthread_t alarm_thread_t;

    Queue_Handle  capture_emptyque;
    Queue_Handle  capture_fullque;
    Queue_Handle  display_emptyque;
    Queue_Handle  display_fullque;
    Queue_Handle  encoder_emptyque;
    Queue_Handle  encoder_fullque; 

    mfc_handle * mfc;

    int sht_fd; /* SHT1X: temperature&humidity sensor. */
    int leds_fd; /*LED*/
    int mot_fd;  /*MOTOR*/
    int gy_fd; /*BH1750FIV sensor*/
    int auctl_fd; /*AudioCtrl*/
    int rfid_fd; /*RFID*/
    int pwm_fd;   /*BEEP*/

    int alarm_status; /*status of alarm : AudioCtrl:1 */
}GlobalHandle;

int handle_init(GlobalHandle * handle);
void handle_exit(GlobalHandle *handle);
#endif
