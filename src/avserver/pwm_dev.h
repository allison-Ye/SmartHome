#ifndef _PWM_DEV_H
#define _PWM_DEV_H
#include <sys/ioctl.h>
#include <common.h>

#define PWM_IOCTL_SET_FREQ              1
#define PWM_IOCTL_STOP                  0
int pwm_dev_init(GlobalHandle * handle);
int pwm_dev_exit(GlobalHandle * handle);
int beeper_on(GlobalHandle * handle, int enable);

#endif
