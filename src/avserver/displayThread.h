#ifndef _DISPLAYTHREAD_H_
#define _DISPLAYTHREAD_H_

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
#include "common.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#ifdef __cplusplus
extern "C" {
#endif
void display_open(void);
int display_init(GlobalHandle * handle);
void displayStart(void);
void *display_thread(void *args);

#ifdef __cplusplus
}
#endif

#endif // CAMERAINIT_H
