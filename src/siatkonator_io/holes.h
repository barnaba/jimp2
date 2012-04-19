#include "../common.h"
#include "siatkonator_io.h"
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>

int read_holes_header(FILE *input_file, triangulateio *input);
void allocate_holes(triangulateio *input);
int read_holes(FILE *input_file, triangulateio *input);
