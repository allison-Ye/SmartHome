obj-m := MOTOR.o                  #要生成的模块名      
modules-objs:= MOTOR.o         #生成这个模块名所需要的目标文件

KDIR := ../../../kernel/
PWD := $(shell pwd)
CC := arm-linux-gcc

default:
	make -C $(KDIR) M=$(PWD) modules
	$(CC) demo_motor.c -o demo_motor
clean:
	rm -rf *.o  .cmd *.ko *.mod.c .tmp_versions *.symvers *.order 
install:
	cp MOTOR.ko demo_motor  ../../../rootfs
