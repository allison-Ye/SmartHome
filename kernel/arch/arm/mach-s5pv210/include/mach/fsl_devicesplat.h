//linux/arch/arm/mach-s5pv210/include/mach/fsl_devicesplatdata.h
//create
/*
 *  mma7660.c - Linux kernel modules for 3-Axis Orientation/Motion
 *  Detection Sensor
 *
 *  Copyright (C) 2009-2010 Freescale Semiconductor Ltd.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef FSL_DEVICE_H
#define FSL_DEVICE_H

struct mxc_mma7660_platform_data {
        char *reg_dvdd_io;
        char *reg_avdd;
        void (*gpio_pin_get) (void);
        void (*gpio_pin_put) (void);
        int irq;
};
#endif /*FSL_DEVICE_H*/