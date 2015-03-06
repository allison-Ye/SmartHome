CPP=arm-linux-g++
CC=arm-linux-gcc
LIB_DIR=./lib/
LIBS= $(LIB_DIR)libliveMedia.a $(LIB_DIR)libgroupsock.a $(LIB_DIR)libBasicUsageEnvironment.a $(LIB_DIR)libUsageEnvironment.a 
CFLAGS=-I./include
TARGET=H264CameraLiveSource.o H264LiveVideoServerMediaSubssion.o main.o \
	mem_mng.o

liveServer:$(TARGET)
	$(CPP) -o liveServer $(TARGET) $(LIBS)
main.o:main.cpp H264LiveVideoServerMediaSubssion.hh H264CameraLiveSource.hh
	$(CPP) -c main.cpp $(CFLAGS)
H264CameraLiveSource.o:H264CameraLiveSource.cpp
	$(CPP) -c H264CameraLiveSource.cpp $(CFLAGS)
H264LiveVideoServerMediaSubssion.o:H264LiveVideoServerMediaSubssion.cpp
	$(CPP) -c H264LiveVideoServerMediaSubssion.cpp $(CFLAGS)
mem_mng.o:mem_mng.c
	$(CC) -c  mem_mng.c
clean:
	rm -rf *.o liveServer
install:
	cp liveServer ../../rootfs
