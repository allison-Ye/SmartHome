#ifndef CAMERAINIT_H
#define CAMERAINIT_H

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

#include "mem_mng.h"
#include "common.h"
#define CLEAR(x) memset (&(x), 0, sizeof (x))

#ifdef __cplusplus
extern "C" {
#endif

void errno_exit(const char* s);
int xioctl(int fd, int request, void* argp);
void deviceOpen(void);
int deviceInit(void);
void captureStart(void);
void *capture_thread(void *args);
unsigned char* imageProcess(const void* p,unsigned char* dst);
void deviceUninit(void);
int fb_init(void);
unsigned char* frameRead(GlobalHandle * handle);
#ifdef __cplusplus
}
#endif

#endif // CAMERAINIT_H
