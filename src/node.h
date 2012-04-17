#include "common.h"
#include "siatkonator_io.h"
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>

int read_node_file(const char *filename, triangulateio * input,
		   int *firstnode);
int read_nodes_header(FILE * input_file, triangulateio * input,
		      int *markers);
void allocate_nodes(triangulateio * input);
int read_nodes(FILE * infile, triangulateio * input, int *markers,
	       int *firstnode);
