#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define GYIO 0x36
#define GY_IOCTL_GET_LUX    _IOR(GYIO, 0x1, int)

struct i2c_client *gy_client;

int gy_read (u8 *data, u16 len)
{
    int ret = 0;

    struct i2c_msg msg[] = {
        [0] = {
            .addr = gy_client->addr,
            .flags = I2C_M_RD,
            .len = len,
            .buf = data
              }
        };

    if (i2c_transfer(gy_client->adapter, msg, 1) != 1)
        ret = -EIO;

    return ret;
}

int gy_write(u8 *data, u16 len)
{
    int ret = 0;

    if (i2c_master_send(gy_client, (const char *) data, len) != len)
        ret = -EIO;

    return ret;
}
int gy_get_lux()
{
    char buf = 0;
    char read_buf[2];
    int val;   
 
    buf = 0x1;
    gy_write(&buf,1);
    buf = 0x10;
    gy_write(&buf,1);
    //mdelay(120);
    msleep(120);
      
    gy_read(read_buf,2);
    val= ((read_buf[0]<<8) + read_buf[1]);
    printk(KERN_WARNING"%s:val = %d lux\n",__func__,val);
    return val;
}
int gy_ioctl (struct inode * inode, struct file * file, unsigned int cmd , unsigned long arg)
{
     printk(KERN_WARNING"%s\n",__func__);

     void __user *argp = (void __user *)arg;
     switch(cmd)
     {
         case GY_IOCTL_GET_LUX:
         {
             int val = gy_get_lux();
             if(copy_to_user(argp, &val, sizeof(val)))
                 return -EFAULT;
             break;
         }
         default:
             break;
    }
    return 0;
}


struct class *gy_class;
struct class_device * gy_class_dev;
dev_t devno = 0;
static struct cdev cdev_gy;

struct file_operations gy_fop =
{
    .owner = THIS_MODULE,
    .ioctl = gy_ioctl,
};


/* i2c client ftn. */
static int __devinit gy_probe(struct i2c_client *client,
			const struct i2c_device_id *dev_id)
{
    int ret = 0;
    char buf = 0;
    char read_buf[2];
    int val = 0;
    gy_client = client;

    dev_info(&client->adapter->dev, "attached GY-30 "
		"into i2c adapter successfully\n");
    ret = alloc_chrdev_region(&devno, 0, 1, "GY-30");
    if(ret)
    {
        printk(KERN_WARNING"Error %d register GY-30\n",ret);
        return ret;
    }
    cdev_init(&cdev_gy, &gy_fop);
    ret = cdev_add(&cdev_gy, devno,1);

    if(ret)
    {
        printk(KERN_WARNING"Error %d adding GY-30\n",ret);
        return ret;
    }
    gy_class = class_create(THIS_MODULE, "gy_class");
    if(IS_ERR(gy_class))
    {
        printk(KERN_WARNING"Create class error!\n");
        return -1;
    }
    gy_class_dev = device_create(gy_class, NULL, devno, "%d","gy");
    if(IS_ERR(gy_class_dev))
    {
        printk(KERN_WARNING"Create device class error!\n");
        return -1;
    }

  
    buf = 0x7;
    gy_write(&buf,1);
    buf = 0x1;
    gy_write(&buf,1);

//    while(1)
//    {
//        buf = 0x1;
//        gy_write(&buf,1);
//	buf = 0x10;
//	gy_write(&buf,1);
//	mdelay(120);
//	gy_read(read_buf,2);
//	val= ((read_buf[1]<<8) + read_buf[0]);
//	printk(KERN_WARNING"val = %d lx\n",val);
//    }
    return ret;
}

static int gy_remove(struct i2c_client *client)
{
	dev_info(&client->adapter->dev, "detached GY-30 "
		"from i2c adapter successfully\n");

	return 0;
}

static struct i2c_device_id gy_idtable[] = {
	{"GY-30", 0},
        {NULL,0}
};

MODULE_DEVICE_TABLE(i2c, gy_idtable);

static struct i2c_driver gy_driver = {
	.driver = {
		.name = "GY-30",
	},
	.id_table	= gy_idtable,
	.probe		= gy_probe,
	.remove		= __devexit_p(gy_remove),
};

static int __init gy_init(void)
{
	return i2c_add_driver(&gy_driver);
}

static void __exit gy_exit(void)
{
	i2c_del_driver(&gy_driver);
}

MODULE_AUTHOR("Cloud Sail");
MODULE_DESCRIPTION("Driver for GY30");

MODULE_LICENSE("GPL");

module_init(gy_init);
module_exit(gy_exit);
