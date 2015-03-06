#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include <pthread.h>

#define DEBUG_INFO

#ifdef DEBUG_INFO
#define printf(...) printf(...)
#else
#define printf(...)
#endif


#define NAME "API"
typedef struct
{
    int qid;
    int mid;
    int msgtype;
    unsigned char * share_memory;
    pthread_mutex_t lock;
}API_HANDLE;
#ifdef __cplusplus
extern "C" {
#endif
void cgi_header();
void cgi_tail();

int api_init(API_HANDLE * handle, int msgtype);
void api_exit(API_HANDLE *handle);

int GetTempData(API_HANDLE  * handle, float * val);
int GetHumiData(API_HANDLE  * handle, float * val);
int GetLuxData(API_HANDLE  * handle, float * val);
int GetAlarmStatus(API_HANDLE  * handle, int * val);
int GetRFIDData(API_HANDLE  * handle, unsigned char * id);
int SetLed1Enable(API_HANDLE  * handle, int enable);
int SetLed2Enable(API_HANDLE  * handle, int enable);
int SetMotorDir(API_HANDLE  * handle, int dir);
int SetMotorStop(API_HANDLE  * handle);
int SetBeeperEnable(API_HANDLE  * handle, int enable);
#ifdef __cplusplus
}
#endif
#endif
