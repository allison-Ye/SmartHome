CC=arm-linux-gcc
CFLAGS=-I../inc -I.
LIBS=libinterface.a
cmd:main.o cmdlist.o libinterface.a
	$(CC) -o cmd main.o cmdlist.o $(LIBS)
main.o:main.c
	$(CC) -c main.c $(CFLAGS)
cmdlist.o:cmdlist.c cmdlist.h
	$(CC) -c cmdlist.c $(CFLAGS)
clean:
	rm cmd  *.o -rf
install:
	cp cmd ../../rootfs/var/www/cgi-bin/
