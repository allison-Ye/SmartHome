#ifndef _MOTOR_DEV_H_
#define _MOTOR_DEV_H_

#include <sys/ioctl.h>

#include <common.h>
#define MOTIO 0x28

#define MOT_IOCTL_SET_DIRECTION _IOW(MOTIO,0x1,int)
#define MOT_IOCTL_SET_STOP  _IOW(MOTIO,0x2,int)

int motor_dev_init(GlobalHandle * handle);
void motor_dev_exit(GlobalHandle * handle);
int motor_set_dir(GlobalHandle * handle, int dir);

#endif
