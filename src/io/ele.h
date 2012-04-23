#include "../common.h"
#include "siatkonator_io.h"
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>

#define NODES_PER_TRIANGLE 3

int read_ele_file(const char *ele_filename, triangulateio *input, int firstnode);
int read_elements_header(FILE * ele_file, triangulateio *input);
void allocate_elements(triangulateio *input);
int read_elements(FILE * ele_file, triangulateio *input, int firstnode);

int write_ele(FILE *file, struct triangulateio* output);

