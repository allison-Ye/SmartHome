#include <stdio.h>
#include <devs.h>
#include <common.h>
void * alarm_thread(void * arg)
{
    GlobalHandle * handle = (GlobalHandle *)arg;
    printf("%s\n",__func__);
    while(1)
    {
        int val = 0;
        handle->alarm_status = 0;
        printf("alarm_status = %d\n",handle->alarm_status);
        val = auctl_get_data(handle);
        if(val < 0)
        {
            printf("time out,continue!\n");
            continue;
        }else if(val > 0)
        {
            handle->alarm_status = val;
            printf("alarm_status = %d\n",handle->alarm_status);
            sleep(30);
            continue;
        }
    }
}
