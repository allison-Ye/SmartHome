obj-m := AudioCtrl.o                    #要生成的模块名      
modules-objs:= AudioCtrl.o         #生成这个模块名所需要的目标文件

KDIR := ../../../kernel/
PWD := $(shell pwd)
CC := arm-linux-gcc

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf demo *.o  .cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers
install:
	cp AudioCtrl.ko ../../../rootfs
