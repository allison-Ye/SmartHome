obj-m := gy30.o                    #要生成的模块名      
modules-objs:= gy30.o         #生成这个模块名所需要的目标文件

KDIR := ../../../kernel/
PWD := $(shell pwd)
CC := arm-linux-gcc

default:
	make -C $(KDIR) M=$(PWD) modules
#	$(CC) demo.c -o demo

clean:
	rm -rf *.o  .cmd *.ko *.mod.c .tmp_versions demo modules.order  Module.symvers
install:
	cp gy30.ko ../../../rootfs
