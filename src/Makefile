CC = gcc
CFLAGS = -I/usr/local/include -Wall -ansi -std=gnu99
LDFLAGS = -L/usr/local/lib
LDLIBS = -largtable2 -lm
OBJ = mesh_ops.o siatkonator_io.o triangle.o siatkonator.o common.o node.o poly.o
DEPS = mesh_ops.h siatkonator_io.h

siatkonator: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o siatkonator $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -DDEBUG -o $@ $<

triangle.o: ../vendor/triangle/triangle.c
	$(CC) -c $(CFLAGS) -DSELF_CHECK -DTRILIBRARY -DLINUX -DCDT_ONLY -o $@ $<
