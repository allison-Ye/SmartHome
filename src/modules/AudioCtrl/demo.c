#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#define AUCTLIO 0x40

#define AUCTL_IOCTL_GET_DATA _IOR(AUCTLIO, 0x1, int)

int main()
{
    int fd = open("/dev/AudioCtrl",O_RDWR, 0);
    int val = 0;
    char buf[10] = {0};
    time_t tm;
    struct tm * tm_val;
    if(fd < 0)
    {
        perror("Open AudioCtrl fail!");
        return 0;
    }
    while(1)
    {
        ioctl(fd, AUCTL_IOCTL_GET_DATA, &val);
        time(&tm);
        tm_val = localtime(&tm);
        if(val == 1)
        {
            printf("WARN!,%d-%d-%d %d:%d:%d\n",
                tm_val->tm_year+1900,tm_val->tm_mon + 1,tm_val->tm_mday,
                tm_val->tm_hour,tm_val->tm_min,tm_val->tm_sec);
        }
    }

    close(fd);
    return 0;

}
