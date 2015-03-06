#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

struct i2c_client *ds1307_client;

int ds1307_read (u8 *data, u16 len)
{
    int ret = 0;

    struct i2c_msg msg[] = {
        [0] = {
            .addr = ds1307_client->addr,
            .flags = I2C_M_RD,
            .len = len,
            .buf = data
              }
        };

    if (i2c_transfer(ds1307_client->adapter, msg, 1) != 1)
        ret = -EIO;

    return ret;
}

int ds1307_write(u8 *data, u16 len)
{
    int ret = 0;

    if (i2c_master_send(ds1307_client, (const char *) data, len) != len)
        ret = -EIO;

    return ret;
}
int ds1307_ioctl (struct inode * inode, struct file * file, unsigned int cmd , unsigned long arg)
{
     printk(KERN_WARNING"%s\n",__func__);
}


struct class *ds1307_class;
struct class_device * ds1307_class_dev;
dev_t devno = 0;
static struct cdev cdev_ds1307;

struct file_operations ds1307_fop =
{
    .owner = THIS_MODULE,
    .ioctl = ds1307_ioctl,
};


/* i2c client ftn. */
static int __devinit ds1307_probe(struct i2c_client *client,
			const struct i2c_device_id *dev_id)
{
    int ret = 0;
    char buf = 0;
    char read_buf[7];
    int val = 0;
    ds1307_client = client;

    dev_info(&client->adapter->dev, "attached GY-30 "
		"into i2c adapter successfully\n");
    ret = alloc_chrdev_region(&devno, 0, 1, "GY-30");
    if(ret)
    {
        printk(KERN_WARNING"Error %d register GY-30\n",ret);
        return ret;
    }
    cdev_init(&cdev_ds1307, &ds1307_fop);
    ret = cdev_add(&cdev_ds1307, devno,1);

    if(ret)
    {
        printk(KERN_WARNING"Error %d adding GY-30\n",ret);
        return ret;
    }
    ds1307_class = class_create(THIS_MODULE, "ds1307_class");
    if(IS_ERR(ds1307_class))
    {
        printk(KERN_WARNING"Create class error!\n");
        return -1;
    }
    ds1307_class_dev = device_create(ds1307_class, NULL, devno, "%d","ds1307");
    if(IS_ERR(ds1307_class_dev))
    {
        printk(KERN_WARNING"Create device class error!\n");
        return -1;
    }

    

    while(1)
    {
    buf = 0x0;
    ds1307_write(&buf,1);
    ds1307_read(read_buf,7);
    printk(KERN_WARNING"sec:%d,min:%d,hour:%d,\n",
		read_buf[0],read_buf[1],read_buf[2]);
    msleep(100);
    }
//    while(1)
//    {
//        buf = 0x1;
//        ds1307_write(&buf,1);
//	buf = 0x10;
//	ds1307_write(&buf,1);
//	mdelay(120);
//	ds1307_read(read_buf,2);
//	val= ((read_buf[1]<<8) + read_buf[0]);
//	printk(KERN_WARNING"val = %d lx\n",val);
//    }
    return ret;
}

static int ds1307_remove(struct i2c_client *client)
{
	dev_info(&client->adapter->dev, "detached DS1307 "
		"from i2c adapter successfully\n");

	return 0;
}

static struct i2c_device_id ds1307_idtable[] = {
	{"DS1307", 0},
        {NULL,0}
};

MODULE_DEVICE_TABLE(i2c, ds1307_idtable);

static struct i2c_driver ds1307_driver = {
	.driver = {
		.name = "DS1307",
	},
	.id_table	= ds1307_idtable,
	.probe		= ds1307_probe,
	.remove		= __devexit_p(ds1307_remove),
};

static int __init ds1307_init(void)
{
	return i2c_add_driver(&ds1307_driver);
}

static void __exit ds1307_exit(void)
{
	i2c_del_driver(&ds1307_driver);
}

MODULE_AUTHOR("Cloud Sail");
MODULE_DESCRIPTION("Driver for GY30");

MODULE_LICENSE("GPL");

module_init(ds1307_init);
module_exit(ds1307_exit);
