#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <gy_dev.h>
#include <common.h>

#define GYDEV_PATH "/dev/gy"

int gy_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL\n",__func__);
        return -1;
    }
    fd = open(GYDEV_PATH, O_RDWR, 0);
    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->gy_fd = fd;
    return 0;
}
int gy_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->gy_fd);
}
static int gy_ioctl_get_lux(GlobalHandle * handle)
{
    int fd;
    int ret,val;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    fd = handle->gy_fd;
    if(fd < 0)
    {
        printf("%s:fd error!\n",__func__);
        return -1;
    }
    ret = ioctl(fd ,GY_IOCTL_GET_LUX, &val);
    if(ret < 0) 
    {
        perror("ioctl");
        return ret;
    }
    return val;
}
float get_lux(GlobalHandle * handle)
{
    return gy_ioctl_get_lux(handle) / 1.2;
}
