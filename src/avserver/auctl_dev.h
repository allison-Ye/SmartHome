#ifndef _SHT_DEV_H_
#define _SHT_DEV_H_

#include <sys/ioctl.h>
#include <common.h>

#define AUCTLIO 0x40
#define AUCTL_IOCTL_GET_DATA _IOR(AUCTLIO, 0x1, int)
int auctl_dev_init(GlobalHandle * handle);
void auctl_dev_exit(GlobalHandle * handle);
int auctl_get_data(GlobalHandle * handle);
#endif
