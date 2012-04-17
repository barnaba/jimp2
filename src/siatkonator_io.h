#include "common.h"

#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <assert.h>
#define INPUTLINESIZE 1024

//writing

int read_ele(const char* filename, struct triangulateio* input);

//reading

int write_ele(const char* filename, struct triangulateio* output);
int write_node(const char* filename, struct triangulateio* output);

//utility functions

char *readline(char *string, FILE *infile, int *error);
char *findfield(char *string);
