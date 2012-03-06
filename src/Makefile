CC = gcc
CFLAGS = -I/usr/local/include -Wall -ansi -std=gnu99
LDFLAGS = -L/usr/local/lib
LDLIBS = -largtable2
OBJ = siatkonator.o mesh_ops.o siatkonator_io.o triangle.o
DEPS = mesh_ops.h siatkonator_io.h

siatkonator: $(OBJ)
	$(CC) $(LDFLAGS) siatkonator.o -o siatkonator $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

triangle.o: ../vendor/triangle/triangle.c
	$(CC) -c $(CFLAGS) -DTRILIBRARY -o $@ $<
