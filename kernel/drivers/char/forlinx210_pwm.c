#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <plat/regs-timer.h>
#include <mach/regs-irq.h>
#include <asm/mach/time.h>
#include <linux/clk.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

#include <plat/gpio-cfg.h>

#include <mach/gpio.h>
#include <mach/gpio-smdkc110.h>
#include <mach/regs-gpio.h>


#define DEVICE_NAME     "pwm"
#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0

#define GPD0CON            S5PV210_GPD0_BASE//(0xE02000A0) //zdgadd
#define S5PV210_TCON  S3C2410_TCON//zdgAdd
#define S5PV210_TCFG0 S3C2410_TCFG0//zdgAdd
#define S5PV210_TCFG1 S3C2410_TCFG1//zdgAdd
#define S5PV210_TCNTB(x) S3C2410_TCNTB(x)//zdgAdd
#define S5PV210_TCMPB(x) S3C2410_TCMPB(x)//zdgAdd
#define S5PV210_TCFG1_MUX1_MASK  15<0//zdgAdd
#define S5PV210_TCFG1_MUX1_DIV16  4<0

static struct semaphore lock;

/* freq:  pclk/50/16/65536 ~ pclk/50/16/1 
  * if pclk = 50MHz, freq is 1Hz to 62500Hz
  * human ear : 20Hz~ 20000Hz
  */
static void PWM_Set_Freq( unsigned long freq )
{
	unsigned long tcon=0;
	unsigned long tcnt=0;
	unsigned long tcfg1=0;
	unsigned long tcfg0=0;

	struct clk *clk_p=0;
	unsigned long pclk=1;

	unsigned tmp;
        
	freq=freq>0?freq:1;

        printk ("in module Freq is %d\n",freq);

	tmp = readl(GPD0CON);//PWM out use timer0 TOUT_0 here

          tmp &=~(0xfU);//  TOUT_0 here 
          tmp |=  (0x2U); //use TOUT_0 

	writel(tmp, GPD0CON);// Set hightest bit is '10' to pwm out

	tcon = __raw_readl(S5PV210_TCON);
	
	tcfg1 = __raw_readl(S5PV210_TCFG1);
	tcfg0 = __raw_readl(S5PV210_TCFG0);

	//prescaler = 50
	tcfg0 &= ~S3C2410_TCFG_PRESCALER0_MASK;//S3C_TCFG_PRESCALER0_MASK;
	tcfg0 |= (50 - 1); 

	//mux = 1/16
          tcfg1 &= ~S5PV210_TCFG1_MUX1_MASK ;//S3C_TCFG1_MUX1_MASK;
          tcfg1 |= S5PV210_TCFG1_MUX1_DIV16;//S3C_TCFG1_MUX1_DIV16;

	__raw_writel(tcfg1, S5PV210_TCFG1);
	__raw_writel(tcfg0, S5PV210_TCFG0);

	
	clk_p = clk_get(NULL, "pclk");
    pclk  = clk_get_rate(clk_p);
    tcnt  = pclk/50/16/freq;
	
    printk ("in module pclk is %dHz\n", pclk);
    printk ("in module tcntb is %d \n",tcnt);
	__raw_writel(tcnt, S3C2410_TCNTB(0));//use timer0  tcntb
	__raw_writel(tcnt/2, S3C2410_TCMPB(0));//use timer0 tcmpb
				
	
	
	tcon &= ~(0xf);
	tcon |= (0xb);		// set 3-0 bit of TCON is '1011'  . auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 0
	__raw_writel(tcon, S5PV210_TCON);
	
	tcon &= ~(2);	  //set set 11-8 bit of S3C_TCON is '1001' 
	__raw_writel(tcon, S5PV210_TCON);
}

void PWM_Stop( void )
{
	unsigned tmp;
	tmp = readl(GPD0CON);
	tmp &= ~(0xfU);// set GP0CON 
	tmp |=(0xeU);//set output mode
	writel(tmp, GPD0CON);
}

static int s5pv210_pwm_open(struct inode *inode, struct file *file)
{
	if (!down_trylock(&lock))
		return 0;
	else
        return -EBUSY;
}


static int s5pv210_pwm_close(struct inode *inode, struct file *file)
{
	up(&lock);
	return 0;
}


static long s5pv210_pwm_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
		case PWM_IOCTL_SET_FREQ:
			if (arg == 0)
				return -EINVAL;
			PWM_Set_Freq(arg);
			break;

		case PWM_IOCTL_STOP:
		default:
			PWM_Stop();
			break;
	}

	return 0;
}


static struct file_operations dev_fops = {
    .owner			= THIS_MODULE,
    .open			= s5pv210_pwm_open,
    .release		= s5pv210_pwm_close, 
    .unlocked_ioctl	= s5pv210_pwm_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret=0;

	init_MUTEX(&lock);

	ret = misc_register(&misc);

	printk (DEVICE_NAME"\t pwm initialized ok \n");

    	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FORLINX Inc.");
MODULE_DESCRIPTION("S5PV210 PWM Driver");
