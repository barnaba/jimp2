
#ifndef SIATKONATOR_COMMON_H
#define SIATKONATOR_COMMON_H
#include <stdio.h>

#define REAL double
#define DIMENSIONS 2

#ifdef VERBOSE
#define LOG_LEVEL 1
#else
#define LOG_LEVEL 2
#endif

#define INFO 2
#define DEBUG 1

#define SUCCESS 0
#define FILE_ERR 1
#define FORMAT_ERR 2
#define ARG_ERR 3
#define NOT_SUPPORTED 9001


#include "../vendor/triangle/triangle.h"
typedef struct triangulateio triangulateio;
#endif

void siatkonator_log(int level, const char *format, ...);
void report(triangulateio * io, int markers, int reporttriangles,
	    int reportneighbors, int reportsegments, int reportedges,
	    int reportnorms);
