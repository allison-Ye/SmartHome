#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include<sys/ioctl.h>
#define MOTIO 0x28

#define MOT_IOCTL_SET_DIRECTION _IOR(MOTIO, 0X1, int)
int direction = -1;

void setDirection(int val){
	if (val == 0)
		direction = 0;
	else 
		direction = 1;
}


int main()
{
	int fd = open("/dev/MOTOR",O_RDWR,0);
	char buf[10] = {0};
//	time_t tm;
//	struct tm* tm_val;
	if(fd < 0)
	{
		perror("Open MOTOR fail!");
		return 0;
	}
	while(1)
	{
		printf("Please input nu:\n");
		scanf("%d",&direction);
		setDirection(direction);
		ioctl(fd, MOT_IOCTL_SET_DIRECTION,&direction);
	}
	close(fd);
	return 0;
}
