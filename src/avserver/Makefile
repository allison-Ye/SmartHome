CC=arm-linux-gcc
CPP=arm-linux-g++
LIBS=-lpthread
CFLAGS=-I. -I../inc
TARGET=captureThread.o encoderThread.o main.o mem_mng.o mfc.o \
	SsbSipMfcEncAPI.o displayThread.o leds_dev.o \
	msg_proc_set.o msg_proc_get.o \
	sht_dev.o gy_dev.o motor_dev.o auctl_dev.o rfid_dev.o pwm_dev.o \
        common.o queue.o  \
        streamThread.o alarmThread.o
avserver:$(TARGET)
	$(CC) -o avserver $(TARGET) $(LIBS)
captureThread.o:captureThread.c captureThread.h
	$(CC) -c captureThread.c
encoderThread.o:encoderThread.c
	$(CC) -c encoderThread.c
displayThread.o:displayThread.c
	$(CC) -c displayThread.c
streamThread.o:streamThread.c
	$(CC) -c streamThread.c
alarmThread.o:alarmThread.c
	$(CC) -c alarmThread.c $(CFLAGS)
main.o:main.c
	$(CC) -c main.c $(CFLAGS)
mem_mng.o:mem_mng.c
	$(CC) -c mem_mng.c
mfc.o:mfc.c
	$(CC) -c mfc.c
SsbSipMfcEncAPI.o:SsbSipMfcEncAPI.c
	$(CC) -c SsbSipMfcEncAPI.c
common.o:common.c
	$(CC) -c common.c $(CFLAGS)
msg_proc_set.o:msg_proc_set.c
	$(CC) -c msg_proc_set.c $(CFLAGS)
msg_proc_get.o:msg_proc_get.c
	$(CC) -c msg_proc_get.c $(CFLAGS)
leds_dev.o:leds_dev.c
	$(CC) -c leds_dev.c $(CFLAGS)
sht_dev.o:sht_dev.c
	$(CC) -c sht_dev.c $(CFLAGS)
gy_dev.o:gy_dev.c gy_dev.h
	$(CC) -c gy_dev.c $(CFLAGS)
motor_dev.o:motor_dev.c motor_dev.h
	$(CC) -c motor_dev.c $(CFLAGS)
auctl_dev.o:auctl_dev.c auctl_dev.h
	$(CC) -c auctl_dev.c $(CFLAGS)
rfid_dev.o:rfid_dev.c rfid_dev.h
	$(CC) -c rfid_dev.c $(CFLAGS)
pwm_dev.o:pwm_dev.c pwm_dev.h
	$(CC) -c pwm_dev.c $(CFLAGS)
queue.o:queue.c queue.h
	$(CC) -c queue.c

install:
	cp avserver /home/alex/SmartHome/rootfs
clean:
	rm *.o avserver -rf
