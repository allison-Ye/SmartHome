#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <motor_dev.h>
#include <common.h>
#define MOTORDEV_PATH "/dev/MOTOR"

int motor_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    fd = open(MOTORDEV_PATH, O_RDWR, 0);

    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->mot_fd = fd;
    return 0;
}
void motor_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->mot_fd);
}
int motor_set_dir(GlobalHandle * handle, int dir)
{
    int fd = handle->mot_fd;
    return ioctl(fd, MOT_IOCTL_SET_DIRECTION,&dir);
}
int motor_stop(GlobalHandle * handle)
{
    int fd = handle->mot_fd;
    return ioctl(fd, MOT_IOCTL_SET_STOP);
}
