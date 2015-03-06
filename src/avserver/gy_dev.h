#ifndef _GY_DEV_H_
#define _GY_DEV_H_

#include <sys/ioctl.h>
#include <common.h>

#define GYIO 0x36
#define GY_IOCTL_GET_LUX    _IOR(GYIO, 0x1, int)

#ifdef __cplusplus
extern "C" {
#endif
int gy_dev_init(GlobalHandle * handle);
int gy_dev_exit(GlobalHandle * handle);
float get_lux(GlobalHandle * handle);
#ifdef __cplusplus
}
#endif


#endif
