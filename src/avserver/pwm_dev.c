#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pwm_dev.h"

#define PWMDEV_PATH "/dev/pwm"
int pwm_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL\n",__func__);
        return -1;
    }
    fd = open(PWMDEV_PATH, O_RDWR, 0);
    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->pwm_fd = fd;
    return 0;
}
int pwm_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->pwm_fd);
}
static int pwm_set_freq(GlobalHandle * handle, int freq)
{
    return ioctl(handle->pwm_fd, PWM_IOCTL_SET_FREQ, freq);
}
static int pwm_stop(GlobalHandle * handle)
{
    return ioctl(handle->pwm_fd, PWM_IOCTL_STOP,0);
}
int beeper_on(GlobalHandle * handle, int enable)
{
    if(enable)
        return pwm_set_freq(handle, 2000);
    else
        return pwm_stop(handle);
}



