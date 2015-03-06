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

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>
#include <linux/completion.h>


#define AUCTLIO 0x40

#define AUCTL_IOCTL_GET_DATA _IOR(AUCTLIO, 0x1, int)


MODULE_AUTHOR("Cloud Sail");
MODULE_LICENSE("GPL");

struct completion data_ready;

#define DEVICE_NAME "AudioCtrl"
int AudioCtrl_open(struct inode *, struct file *);
int AudioCtrl_release(struct inode *, struct file *);
int AudioCtrl_ioctl (struct inode *, struct file *, unsigned int, unsigned long);



struct file_operations AudioCtrl_fop =
{
    .owner = THIS_MODULE,
    .open = AudioCtrl_open,
    .ioctl = AudioCtrl_ioctl,
    .release = AudioCtrl_release,
};
static struct cdev cdev_AudioCtrl;

irqreturn_t AudioCtrl_irq_handler(int irq)
{
    printk(KERN_WARNING"%s\n",__func__);
    disable_irq_nosync(irq);
    complete(&data_ready);
    return 0;
}
irqreturn_t AudioCtrl_irq_handler1(int irq)
{
    printk(KERN_WARNING"%s\n",__func__);
    return 0;
}

int AudioCtrl_open(struct inode * inode, struct file * file)
{
    return 0;
}
int AudioCtrl_release(struct inode * inode , struct file * file)
{
    printk(KERN_WARNING"%s\n",__func__);
    return 0;
}
int AudioCtrl_ioctl (struct inode * inode, struct file * file, unsigned int cmd , unsigned long arg)
{
    switch(cmd)
    {
        case AUCTL_IOCTL_GET_DATA:
        {
            int val = 1;
            int ret;
            void __user  * argp = (void __user  *)arg;
            enable_irq(IRQ_EINT_GROUP(7,3));
            ret = wait_for_completion_timeout(&data_ready, 30 * HZ);
            if(ret == 0)
            {
                disable_irq(IRQ_EINT_GROUP(7,3));
                return -ETIMEDOUT;
            }
            if(copy_to_user(argp, &val, sizeof(int)))
                return -EFAULT;
            break;
        }
        default:
             return -EINVAL;
         
    }
    return 0;
}


static int __init AudioCtrl_init(void)
{
    int result;
    dev_t devno = 0;
    struct class *SR_class;

    result = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);

    if(result)
    {
         printk(KERN_NOTICE "Error %d register AudioCtrl\n",result);
         return result;
    }
    cdev_init(&cdev_AudioCtrl,&AudioCtrl_fop);

    result = cdev_add(&cdev_AudioCtrl,devno,1);
    if(result)
    {
        printk(KERN_NOTICE "Error %d adding AudioCtrl\n",result);
	return result;
    }
    SR_class = class_create(THIS_MODULE, "AuCtrl_class");
    if(IS_ERR(SR_class))
    {
        printk(KERN_WARNING"Create class error!\n");
        return -1;
    }
    device_create(SR_class, NULL, devno, "%d","AudioCtrl");

    s3c_gpio_cfgpin(S5PV210_GPD1(3), S3C_GPIO_SFN(0xf));
    s3c_gpio_setpull(S5PV210_GPD1(3), S3C_GPIO_PULL_NONE);
    set_irq_type(IRQ_EINT_GROUP(7,3), IRQ_TYPE_EDGE_BOTH);
    if (result = request_irq(IRQ_EINT_GROUP(7,3), AudioCtrl_irq_handler, IRQ_TYPE_EDGE_BOTH | IRQF_DISABLED,"AudioCtrl", NULL)) {
        pr_err("%s::failed to install %s irq,ret = %d\n", __func__, "AudioCtrl",result);
        return -EIO;
    }
    disable_irq(IRQ_EINT_GROUP(7,3));
    init_completion(&data_ready);


    return 0;
}
static void __exit AudioCtrl_exit(void)
{
}

module_init(AudioCtrl_init);
module_exit(AudioCtrl_exit);
