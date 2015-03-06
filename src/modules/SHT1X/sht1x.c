#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <asm/irq.h>
#include <mach/hardware.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/ioctl.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>
#define SHTIO 0x35

#define SHT_IOCTL_GET_TEMPRAW _IOR(SHTIO, 0x1, int)
#define SHT_IOCTL_GET_HUMIDITY _IOR(SHTIO,0x2, int)

MODULE_AUTHOR("Cloud Sail");
MODULE_LICENSE("GPL");

#define SR_MAJOR 251
#define DEVICE_NAME "sht"
int sht_open(struct inode *, struct file *);
int sht_release(struct inode *, struct file *);
int sht_ioctl (struct inode *, struct file *, unsigned int, unsigned long);


#define VDD  S5PV210_GPA0(2)
#define SCLK S5PV210_GPA0(6)
#define SDAT S5PV210_GPA0(5)


struct file_operations sht_fop =
{
    .owner = THIS_MODULE,
    .open = sht_open,
    .ioctl = sht_ioctl,
    .release = sht_release,
};
static struct cdev cdev_sht;

#define LSBFIRST 0
#define MSBFIRST 1
#define HIGH 1
#define LOW 0
void shiftOut(int dataPin, int clockPin, int bitOrder, int val)
{
      uint8_t i;

      for (i = 0; i < 8; i++) 
      {
          if (bitOrder == LSBFIRST)
              gpio_set_value(dataPin, !!(val & (1 << i)));
          else     
              gpio_set_value(dataPin, !!(val & (1 << (7 - i))));
          gpio_set_value(clockPin, 1);
          gpio_set_value(clockPin, 0);           
      }
}

int  shiftIn(int dataPin, int clockPin, int num)
{
     int ret= 0;
     int i;
     for(i = 0; i < num; i++)
     {
         gpio_set_value(clockPin, 1);
         msleep(1);
         ret = ret * 2 + gpio_get_value(dataPin);
         gpio_set_value(clockPin, 0);

     }
     return ret;
}


void sht_sendcmd(int cmd)
{
    int ack;
    s3c_gpio_cfgpin(SCLK, S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(1));
    gpio_set_value(SDAT, 1);
    gpio_set_value(SCLK, 1);
    gpio_set_value(SDAT, 0);
    gpio_set_value(SCLK, 0);
    gpio_set_value(SCLK, 1);
    gpio_set_value(SDAT, 1);
    gpio_set_value(SCLK, 0);

    shiftOut(SDAT, SCLK, MSBFIRST, cmd);

    gpio_set_value(SCLK, HIGH);

    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(0));

    ack = gpio_get_value(SDAT);

    if(ack != 0)
    {
        printk(KERN_WARNING"ack Error 0!\n");
    }
	
    gpio_set_value(SCLK, 0);  
    msleep(1);
    ack = gpio_get_value(SDAT);

    if(ack != 1)
    {
        printk(KERN_WARNING"ack Error 1!\n");
    }
}

void sht_waitresult()
{
    int i,ack = 0;
    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(0));

    msleep(200);
    while(1)
    {
        ack = gpio_get_value(SDAT);
        if(ack == 0)
	    break;
    }
}

int sht_getdata16()
{
    int val;
    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(0));
    s3c_gpio_cfgpin(SCLK, S3C_GPIO_SFN(1));

    val = shiftIn(SDAT, SCLK, 8);
    val *= 256; 

    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(1));

    gpio_set_value(SDAT, 1);
    gpio_set_value(SDAT, 0);
    gpio_set_value(SCLK, 1);
    gpio_set_value(SCLK, 0);
    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(0));

    val |= shiftIn(SDAT, SCLK, 8);

    return val;
}

void sht_skipcrc()
{
    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(SCLK, S3C_GPIO_SFN(1));

    gpio_set_value(SDAT, 1);
    gpio_set_value(SCLK, 1);
    gpio_set_value(SCLK, 0);

}

int sht_readTempRaw()
{
    int val;
    int cmd = 0x03;
	
    sht_sendcmd(cmd);
    sht_waitresult();
    val = sht_getdata16();
    sht_skipcrc();

    return val;
}

int sht_readHumidity()
{
    int val;
    int cmd = 0x5;

    sht_sendcmd(cmd);
    sht_waitresult();
    val = sht_getdata16();
    sht_skipcrc();

    return val;
}

void sht_gpioconfig()
{
    s3c_gpio_cfgpin(VDD, S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(SCLK, S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(SDAT, S3C_GPIO_SFN(1));
}
int sht_open(struct inode * inode, struct file * file)
{
    int val;
    //printk(KERN_WARNING"%s\n",__func__);
    return 0;
}
int sht_release(struct inode * inode , struct file * file)
{
    //printk(KERN_WARNING"%s\n",__func__);
    return 0;
}
int sht_ioctl (struct inode * inode, struct file * file, unsigned int cmd , unsigned long arg)
{
    void __user *argp = (void __user *)arg;
    sht_gpioconfig();
    switch(cmd)
    {
        case SHT_IOCTL_GET_TEMPRAW:
        {
            int val = sht_readTempRaw();
            printk(KERN_WARNING"tempraw = %d\n",val);
            if(copy_to_user(argp, &val, sizeof(val)))
                return -EFAULT;
            break;
        }
        case SHT_IOCTL_GET_HUMIDITY:
        {
            int val = sht_readHumidity();
            printk(KERN_WARNING"Humidity = %d\n",val);
            if(copy_to_user(argp, &val, sizeof(val)))
                return -EFAULT;
            break;
        }
        default:
            break;
    }
	 
     return 0;
}


struct class  * sht_class;
struct class_device * sht_class_dev; 
dev_t devno = 0;
static int __init sht_init(void)
{
    int result;
    //result = register_chrdev_region(devno,1,DEVICE_NAME);
    result = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);

    if(result)
    {
        printk(KERN_NOTICE "Error %d register sht\n",result);
        return result;
    }

    
    cdev_init(&cdev_sht,&sht_fop);

    result = cdev_add(&cdev_sht,devno,1);
    if(result)
    {
        printk(KERN_NOTICE "Error %d adding sht\n",result);
        return result;
    }
    sht_class = class_create(THIS_MODULE,"sht_class");
    if(IS_ERR(sht_class))
    {
        printk(KERN_WARNING"Create class error!\n");
        return -1;
    }
    sht_class_dev = device_create(sht_class, NULL,devno,"%d","sht");
    if(IS_ERR(sht_class_dev))
    {
        printk(KERN_WARNING"Create device class error!\n");
        return -1;
    }

    return 0;
}
static void __exit sht_exit(void)
{
    device_destroy(sht_class, MKDEV(devno, 0));
    class_destroy(sht_class);
}
module_init(sht_init);
module_exit(sht_exit);

