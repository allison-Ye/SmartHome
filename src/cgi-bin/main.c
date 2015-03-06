#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include "interface.h"
#include "cmdlist.h"
#define DBG_NAME "CGI"
extern cmdlist list[];
int main()
{
    API_HANDLE * handle;
    int ret = -1;
    char * env = NULL;
    char *tmp;
    char cmd[128] = {0};
    char value[128] = {0};
    int i=0;
    handle = (API_HANDLE *)malloc(sizeof(API_HANDLE));
    if(handle == NULL)
    {
        printf(DBG_NAME":malloc error!\n");
        return 0;
    }
    ret = api_init(handle,MSG_TYPE_BOA);
    if(ret)
    {
        printf(NAME":api_init error!\n");
        return ret;
    }
    cgi_header();
    
    env = getenv("QUERY_STRING");
    if(env == NULL)
    {
        printf("getenv error!\n");
        cgi_tail();
        api_exit(handle);
        return 0;
    }
    tmp = strstr(env,"=");
    if(tmp == NULL)
    {
        strcpy(cmd,env);
        value[0] = '0';
    }
    else
    {
        strcpy(value, tmp+1);
        strncpy(cmd, env, tmp-env);
    } 
    while(list[i].cmd != NULL)
    {
        if(!strcmp(list[i].cmd, cmd))
            list[i].func(handle, (void *)value);
        i++;
    }   
    printf("cmd = %s\n\r<BR>",cmd);
    printf("value = %s\n\r<br>",value);
 
    cgi_tail();
    api_exit(handle);

}
