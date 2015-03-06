#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <auctl_dev.h>
#include <common.h>
#define AUCTLDEV_PATH "/dev/AudioCtrl"
int auctl_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    fd = open(AUCTLDEV_PATH, O_RDWR, 0);

    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->auctl_fd = fd;
    return 0;
}
void auctl_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->auctl_fd);
}
int auctl_get_data(GlobalHandle * handle)
{
    int val;
    int ret;
    ret = ioctl(handle->auctl_fd, AUCTL_IOCTL_GET_DATA,&val);
    if(ret < 0)
        return ret;
    return val;
}
