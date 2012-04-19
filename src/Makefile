CC = gcc
CFLAGS = -I/usr/local/include -Wall -ansi -std=gnu99 -DDEBUG
LDFLAGS = -L/usr/local/lib
LDLIBS = -largtable2 -lm
OBJ = mesh_ops.o siatkonator_io.o triangle.o siatkonator.o common.o node.o poly.o segments.o holes.o
DEPS = mesh_ops.h siatkonator_io.h

siatkonator: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o siatkonator $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: siatkonator_io/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

triangle.o: ../vendor/triangle/triangle.c
	$(CC) -c $(CFLAGS) -DSELF_CHECK -DTRILIBRARY -DLINUX -DCDT_ONLY -o $@ $<
