#include <stdio.h>
#include <stdlib.h>
#include <interface.h>
#include <msg.h>
int main()
{
    API_HANDLE * api_handle;
    float val;
    api_handle = (API_HANDLE *)malloc(sizeof(API_HANDLE));

    if(api_handle == NULL)
    {
        printf(NAME":malloc error!\n");
        return 0;
    }

    printf("Content type: text/html\n\n");
    printf("<html>\n");
    printf("<head><title>An html page from a cgi</title></head>\n");
    printf("<body bgcolor=\"#666666\"></body>\n");

    api_init(api_handle, MSG_TYPE_TEST);
    GetTempData(api_handle, &val);
    printf("\r\nvalue:%0.2fC\r\n",val);
    printf("</html>\n");
    fflush(stdout);
    api_exit(api_handle);
}
