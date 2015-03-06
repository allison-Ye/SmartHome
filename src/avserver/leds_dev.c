#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <common.h>

#define LEDS_PATH "/dev/leds"

int leds_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    fd = open(LEDS_PATH, O_RDWR, 0);

    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->leds_fd = fd;
    return 0;
}
void leds_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->leds_fd);
}
int leds_enable(GlobalHandle *handle, int index, int enable)
{
    int fd;
    int ret;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    fd = handle->leds_fd;
    if(fd < 0)
    {
        printf("%s: fd error\n",__func__);
        return -1;
    }
    ret = ioctl(fd , enable > 0?1:0, index);
    if(ret)
    {
        perror("ioctl");
    }
    return ret;
}
