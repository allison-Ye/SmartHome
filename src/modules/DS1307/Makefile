obj-m := ds1307.o                    #要生成的模块名      
modules-objs:= ds1307.o         #生成这个模块名所需要的目标文件

KDIR := ../../../kernel/
PWD := $(shell pwd)
CC := arm-linux-gcc

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o  .cmd *.ko *.mod.c .tmp_versions demo modules.order  Module.symvers
install:
	cp ds1307.ko ../../../rootfs
