#include "../common.h"
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define INPUTLINESIZE 1024

int ele_to_triangulateio(const char *ele_filename, triangulateio *input);

int read_ele(const char* filename, struct triangulateio* input);
int read_node(const char* filename, struct triangulateio* input);

//utility functions

char *readline(char *string, FILE *infile, int *error);
char *findfield(char *string);
