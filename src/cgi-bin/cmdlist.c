#include <stdio.h>
#include "cmdlist.h"
int set_led1_enable(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    int val;
    if(handle == NULL)
        return -1;
    if(arg != NULL)
        val = atoi(str);
  
    return SetLed1Enable(handle,val);
}
int set_led2_enable(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    int val;
    if(handle == NULL)
        return -1;
    if(arg != NULL)
        val = atoi(str);
  
    return SetLed2Enable(handle,val);
}
int set_motor_dir(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    int val;
    if(handle == NULL)
        return -1;
    if(arg != NULL)
        val = atoi(str);
  
    return SetMotorDir(handle,val);
}
int set_motor_stop(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    int val;
    if(handle == NULL)
        return -1;
    if(arg != NULL)
        val = atoi(str);
  
    return SetMotorStop(handle);
}
int set_beeper_enable(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    int val;
    if(handle == NULL)
        return -1;
    if(arg != NULL)
        val = atoi(str);
  
    return SetBeeperEnable(handle, val);
}
int get_temp_data(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    float val;
    int ret;
    if(handle == NULL)
        return -1;
    ret = GetTempData(handle, &val);
    if(ret)
        return -1;
    if(str != NULL)
        sprintf(str,"[%.2f]\r\n<br>",val);
    return 0;
}
int get_humi_data(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    float val;
    int ret;
    if(handle == NULL)
        return -1;
    ret = GetHumiData(handle, &val);
    if(ret)
        return -1;
    if(str != NULL)
        sprintf(str,"[%.2f]\r\n<br>",val);
    return 0;
}
int get_lux_data(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    float val;
    int ret;
    if(handle == NULL)
        return -1;
    ret = GetLuxData(handle, &val);
    if(ret)
        return -1;
    if(str != NULL)
        sprintf(str,"[%.2f]\r\n<br>",val);
    return 0;
}
int get_alarm_status(API_HANDLE * handle,void *arg)
{
    char * str = (char *)arg;
    int val;
    int ret;
    if(handle == NULL)
        return -1;
    ret = GetAlarmStatus(handle, &val);
    if(ret)
        return -1;
    if(str != NULL)
        sprintf(str,"[%d]\r\n<br>",val);
    return 0;
}
extern cmdlist list[];
int cmd_list(API_HANDLE *handle, void *arg)
{
    int i = 0;
    while(list[i].cmd != NULL)
    {
        printf("%s\r\n<br>",list[i].cmd);
        i++;
    }
    return 0;
}
cmdlist list[] = {
    {"setled1"   ,  set_led1_enable},
    {"setled2"   ,  set_led2_enable},
    {"setmotdir" ,  set_motor_dir},
    {"setmotstop",  set_motor_stop},
    {"setbeepen" ,  set_beeper_enable},
////////////////////////////////////////////////////
    {"gettemp"   ,  get_temp_data},
    {"gethumi"   ,  get_humi_data},
    {"getlux"    ,  get_lux_data},
    {"getalarm"  ,  get_alarm_status},
    {"cmdlist"   ,  cmd_list},
    {NULL, NULL},
};
