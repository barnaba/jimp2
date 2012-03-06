#include "common.h"

int read_ele(char* filename, struct triangulateio* input);
int read_node(char* filename, struct triangulateio* input);
int read_poly(char* filename, struct triangulateio* input);

int write_ele(char* filename, struct triangulateio* output);
int write_node(char* filename, struct triangulateio* output);
