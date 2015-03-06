#ifndef _CMDLIST_H_
#define _CMDLIST_H_
#include "interface.h"
typedef struct
{
    char *cmd;
    int (*func)(API_HANDLE *, void *);
}cmdlist;


#endif
