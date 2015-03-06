#include <stdio.h>
#include <stdlib.h>
#include <interface.h>
#include <msg.h>
int main()
{
    API_HANDLE * api_handle;
    int enable;
    char * env = NULL;
    api_handle = (API_HANDLE *)malloc(sizeof(API_HANDLE));
    
    if(api_handle == NULL)
    {
        printf(NAME":malloc error!\n");
        return 0;
    }

    cgi_header();
    env = getenv("QUERY_STRING");
    if(env == NULL)
    {
        printf("getenv error!\n");
        cgi_tail();
        api_exit(api_handle);
        return 0;
    }
    printf("env = %s\n\n",env);
    enable = atoi(env);
    api_init(api_handle, MSG_TYPE_TEST);
    SetLed1Enable(api_handle, enable);
    cgi_tail();
    api_exit(api_handle);
}
