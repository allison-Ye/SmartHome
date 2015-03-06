#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#define GYIO 0x36
#define GY_IOCTL_GET_LUX    _IOR(GYIO, 0x1, int)

int main()
{
    int fd = open("/dev/gy",O_RDWR, 0);
    char buf[10] = {0};
    time_t tm;
    struct tm * tm_val;
    if(fd < 0)
    {
        perror("Open GY 30  fail!");
        return 0;
    }
    while(1)
    {
       int val;
       ioctl(fd, GY_IOCTL_GET_LUX, &val);
       printf("SI: %d lux\n",val);
       sleep(3);
    }
    close(fd);
    return 0;

}
