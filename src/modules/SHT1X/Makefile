obj-m := sht1x.o                    #要生成的模块名      
moudules-objs:= sht1x.o         #生成这个模块名所需要的目标文件

KDIR := ../../../kernel/
PWD := $(shell pwd)
CC := arm-linux-gcc

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o  .cmd *.ko *.mod.c .tmp_versions modules.order demo Module.symvers 
install:
	cp sht1x.ko ../../../rootfs
