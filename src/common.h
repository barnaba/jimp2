
#ifndef SIATKONATOR_COMMON_H
#define SIATKONATOR_COMMON_H 
#include <stdio.h>

#define REAL double
#define DIMENSIONS 2

#define SUCCESS 0
#define FILE_ERR 1
#define FORMAT_ERR 2
#define ARG_ERR 3
#define NOT_SUPPORTED 9001


#include "../vendor/triangle/triangle.h"
typedef struct triangulateio triangulateio;
#endif

void siatkonator_log(const char* format, ...);
void report(triangulateio *io, int markers, int reporttriangles, int reportneighbors, int reportsegments, int reportedges, int reportnorms);
