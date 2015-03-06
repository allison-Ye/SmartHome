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
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>
#include <linux/ioctl.h>

#define MOTIO 0x28

#define MOT_IOCTL_SET_DIRECTION _IOW(MOTIO,0x1,int)
#define MOT_IOCTL_SET_STOP  _IOW(MOTIO,0x2,int)

MODULE_AUTHOR("Cloud Sail");
MODULE_LICENSE("GPL");

#define SR_MAJOR 126
#define DEVICE_NAME "MOTOR"

#define IN1 S5PV210_GPA0(4)
#define IN2 S5PV210_GPC0(4)


int MOTOR_open(struct inode *, struct file *);
int MOTOR_release(struct inode *, struct file *);
int MOTOR_ioctl (struct inode *, struct file *, unsigned int, unsigned long);

struct file_operations MOTOR_fop =
{
    .owner = THIS_MODULE,
    .open = MOTOR_open,
    .ioctl = MOTOR_ioctl,
    .release = MOTOR_release,
};
static struct cdev cdev_MOTOR;

int MOTOR_open(struct inode * inode, struct file * file)
{
    s3c_gpio_cfgpin(IN1, S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(IN2, S3C_GPIO_SFN(1));
    gpio_set_value(IN1, 0);
    gpio_set_value(IN2, 0); 
    return 0;
}
int MOTOR_release(struct inode * inode , struct file * file)
{
    gpio_set_value(IN1, 0);
    gpio_set_value(IN2, 0); 
    return 0;
}
int MOTOR_ioctl (struct inode * inode, struct file * file, unsigned int cmd , unsigned long arg)
{
     int* dir = (int *)arg;
     int val;  
     printk(KERN_WARNING"%s\n",__func__);
     switch(cmd)
     {
         case MOT_IOCTL_SET_DIRECTION:
         {
             if(copy_from_user(&val,dir,sizeof(val)))
                 return -EFAULT;
             s3c_gpio_cfgpin(IN1, S3C_GPIO_SFN(1));
             s3c_gpio_cfgpin(IN2, S3C_GPIO_SFN(1));
             printk(KERN_WARNING"val = %d\n",val);   
             if(0 == val){
                 gpio_set_value(IN1, 1);
                 gpio_set_value(IN2, 0); 
             }
             else{
                 gpio_set_value(IN1, 0);
                 gpio_set_value(IN2, 1); 
             }
             break;
         }
         case MOT_IOCTL_SET_STOP:
         {
             gpio_set_value(IN1, 0);
             gpio_set_value(IN2, 0); 
             break;
         }
         default:
             break;
     }
     return 0;
}


static int __init MOTOR_init(void)
{
    int result;
    dev_t devno = 0;
    struct class *MO_class;
    struct class_device * MO_class_dev;
    result = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);

    if(result)
    {
        printk(KERN_NOTICE "Error %d register MOTOR\n",result);
        return result;
    }

    cdev_init(&cdev_MOTOR,&MOTOR_fop);

    result = cdev_add(&cdev_MOTOR,devno,1);
    if(result)
    {
        printk(KERN_NOTICE "Error %d adding MOTOR\n",result);
        return result;
    }

    MO_class = class_create(THIS_MODULE, "MO_class");
    if(IS_ERR(MO_class))
    {
        printk(KERN_WARNING"Create class error!\n");
        return -1;
    }
    MO_class_dev = device_create(MO_class, NULL, devno ,"%d","MOTOR");
    if(IS_ERR(MO_class_dev))
    {
        printk(KERN_WARNING"Create device class error!\n");
        return -1;
    }



    return 0;
}
static void __exit MOTOR_exit(void)
{
    printk(KERN_WARNING"hello Cloudsail!\n");
}

module_init(MOTOR_init);
module_exit(MOTOR_exit);
