#ifndef _SHT_DEV_H_
#define _SHT_DEV_H_

#include <sys/ioctl.h>

#include <common.h>
#define SHTIO 0x35

#define SHT_IOCTL_GET_TEMPRAW  _IOR(SHTIO, 0x1, int)
#define SHT_IOCTL_GET_HUMIDITY _IOR(SHTIO, 0x2, int)

int sht_dev_init(GlobalHandle * handle);
void sht_dev_exit(GlobalHandle * handle);
int sht_get_tempC(GlobalHandle * handle, float *value);
int sht_get_humi(GlobalHandle * handle, float *value);


#endif
