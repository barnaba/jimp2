CC = gcc
CFLAGS = -I/usr/local/include -Ivendor/argtable2-13/src -Wall -ansi -D_GNU_SOURCE -std=gnu99 -DVERBOSE -g
LDFLAGS = -Lvendor/argtable2-13/src/
LDLIBS = -largtable2 -lm
ODIR = obj
SDIR = src
BDIR = bin

ORIG_OBJ = mesh_ops.o \
			siatkonator_io.o \
			triangle.o \
			siatkonator.o \
			common.o node.o \
			poly.o \
			segments.o \
			holes.o \
			ele.o \
			markers.o \
			siatkonator_hash.o

OBJ = $(patsubst %,$(ODIR)/%,$(ORIG_OBJ))
HEADERS = $(wildcard src/*.h src/io/*.h)

siatkonator: $(OBJ) vendor/argtable2-13/src/libargtable2.a
	$(CC) $(LDFLAGS) $(OBJ) -o $(BDIR)/siatkonator $(LDLIBS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/%.o: $(SDIR)/io/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/triangle.o: vendor/triangle/triangle.c
	$(CC) -c -DSELF_CHECK -DTRILIBRARY -DLINUX -o $@ $<


vendor/argtable2-13/src/libargtable2.a: vendor/argtable2-13/src/argtable2.h
	cmake vendor/argtable2-13
	make -C vendor/argtable2-13

clean:
	rm -rf obj/*
