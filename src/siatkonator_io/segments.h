//#include "../common.h"
#include "siatkonator_io.h"
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>

int read_segments_header(FILE * input_file, triangulateio * input,
			 int *markers);
void allocate_segments(triangulateio * input);
int read_segments(FILE * infile, triangulateio * input, int markers,
		  int firstnode);
