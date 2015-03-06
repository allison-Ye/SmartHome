#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sht_dev.h>
#include <common.h>
#define SHTDEV_PATH "/dev/sht"
int sht_dev_init(GlobalHandle * handle)
{
    int fd;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    fd = open(SHTDEV_PATH, O_RDWR, 0);

    if(fd < 0)
    {
        printf("%s:",__func__);
        perror("open");
        return -1;
    }
    handle->sht_fd = fd;
    return 0;
}
void sht_dev_exit(GlobalHandle * handle)
{
    if(handle == NULL)
        return;
    close(handle->sht_fd);
}
static int sht_get_temperature(GlobalHandle * handle, int * val)
{
    int fd;
    int ret;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }

    fd = handle->sht_fd;
    if(fd < 0)
    {
        printf("%s:fd error\n",__func__);
        return -1;
    }
    ret = ioctl(fd, SHT_IOCTL_GET_TEMPRAW, val);
    if(ret < 0)
    {
        perror("ioctl");
    }
    return ret;
}
static int sht_get_humidity(GlobalHandle * handle, int * val)
{
    int fd;
    int ret;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }

    fd = handle->sht_fd;
    ret = ioctl(fd, SHT_IOCTL_GET_HUMIDITY, val);
    if(ret < 0)
    {
        printf("%s:",__func__);
        perror("ioctl");
    }
    return ret;
}

int sht_get_tempC(GlobalHandle * handle, float *value)
{
    int val;
    int ret;
    const float d1 = -40.0;
    const float d2 = 0.01;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    ret = sht_get_temperature(handle, &val);
    if(ret < 0)
    {
        printf("%s:sht_get_temperature error!\n",__func__);
        return ret;
    }
    *value = val * d2 + d1;
    return 0;
}
int sht_get_humi(GlobalHandle * handle, float *value)
{
    int val;
    int ret;
    float temp;
    float liner_humidity;
    float corrected_humidity;
    const float c1 = -4.0;
    const float c2 = 0.0405;
    const float c3 = -0.0000028;
    const float t1 = 0.01;
    const float t2 = 0.00008;
    if(handle == NULL)
    {
        printf("%s:handle is NULL!\n",__func__);
        return -1;
    }
    ret = sht_get_humidity(handle, &val);
    if(ret < 0)
    {
        printf("%s:sht_get_temperature error!\n",__func__);
        return ret;
    }
    ret = sht_get_tempC(handle, &temp);
    if(ret < 0)
    {
        printf("%s:sht_get_tempC error!\n",__func__);
        return ret;
    }
    liner_humidity = c1 + c2 * val + c3 * val * val;
    corrected_humidity = temp * (t1 + t2 * val) + liner_humidity;

    *value = corrected_humidity;
    return 0;
}

