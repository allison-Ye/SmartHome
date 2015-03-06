#CC=gcc
CC=arm-linux-gcc
AR=arm-linux-ar
LIBS=-lpthread
CFLAGS=-I../inc/ -I.
demo:demo.c libinterface.a
	$(CC) -o demo demo.c libinterface.a $(LIBS) $(CFLAGS)
libinterface.a:interface.o
	$(AR) rcs libinterface.a interface.o
interface.o:interface.c interface.h
	$(CC) -c interface.c $(CFLAGS)

install:
	cp *.a ../lib/
clean:
	rm -rf demo *.o
