CC = gcc
CFLAGS = -I/usr/local/include -Wall -ansi -std=gnu99
LDFLAGS = -L/usr/local/lib
LDLIBS = -largtable2

siatkonator.o: siatkonator.c
	$(CC) -c $(CFLAGS) -o siatkonator.o siatkonator.c

siatkonator: siatkonator.o
	$(CC) $(LDFLAGS) siatkonator.o -o siatkonator $(LDLIBS)
