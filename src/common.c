#include "common.h"
#include <stdarg.h>

void siatkonator_log(const char *format, ...)
{
#ifdef DEBUG
  va_list argptr;
  va_start(argptr, format);
  vfprintf(stderr, format, argptr);
  va_end(argptr);
#endif
}

void report(triangulateio * io, int markers, int reporttriangles,
	    int reportneighbors, int reportsegments, int reportedges,
	    int reportnorms)
{
#ifdef DEBUG
  int i, j;

  for (i = 0; i < io->numberofpoints; i++) {
    fprintf(stderr, "Point %4d:", i);
    for (j = 0; j < 2; j++) {
      fprintf(stderr, "  %.6g", io->pointlist[i * 2 + j]);
    }
    if (io->numberofpointattributes > 0) {
      fprintf(stderr, "   attributes");
    }
    for (j = 0; j < io->numberofpointattributes; j++) {
      fprintf(stderr, "  %.6g",
	      io->pointattributelist[i * io->numberofpointattributes + j]);
    }
    if (markers) {
      fprintf(stderr, "   marker %d\n", io->pointmarkerlist[i]);
    } else {
      fprintf(stderr, "\n");
    }
  }
  fprintf(stderr, "\n");

  if (reporttriangles || reportneighbors) {
    for (i = 0; i < io->numberoftriangles; i++) {
      if (reporttriangles) {
	fprintf(stderr, "Triangle %4d points:", i);
	for (j = 0; j < io->numberofcorners; j++) {
	  fprintf(stderr, "  %4d",
		  io->trianglelist[i * io->numberofcorners + j]);
	}
	if (io->numberoftriangleattributes > 0) {
	  fprintf(stderr, "   attributes");
	}
	for (j = 0; j < io->numberoftriangleattributes; j++) {
	  fprintf(stderr, "  %.6g", io->triangleattributelist[i *
							      io->numberoftriangleattributes
							      + j]);
	}
	fprintf(stderr, "\n");
      }
      if (reportneighbors) {
	fprintf(stderr, "Triangle %4d neighbors:", i);
	for (j = 0; j < 3; j++) {
	  fprintf(stderr, "  %4d", io->neighborlist[i * 3 + j]);
	}
	fprintf(stderr, "\n");
      }
    }
    fprintf(stderr, "\n");
  }

  if (reportsegments) {
    for (i = 0; i < io->numberofsegments; i++) {
      fprintf(stderr, "Segment %4d points:", i);
      for (j = 0; j < 2; j++) {
	fprintf(stderr, "  %4d", io->segmentlist[i * 2 + j]);
      }
      if (markers) {
	fprintf(stderr, "   marker %d\n", io->segmentmarkerlist[i]);
      } else {
	fprintf(stderr, "\n");
      }
    }
    fprintf(stderr, "\n");
  }

  if (reportedges) {
    for (i = 0; i < io->numberofedges; i++) {
      fprintf(stderr, "Edge %4d points:", i);
      for (j = 0; j < 2; j++) {
	fprintf(stderr, "  %4d", io->edgelist[i * 2 + j]);
      }
      if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
	for (j = 0; j < 2; j++) {
	  fprintf(stderr, "  %.6g", io->normlist[i * 2 + j]);
	}
      }
      if (markers) {
	fprintf(stderr, "   marker %d\n", io->edgemarkerlist[i]);
      } else {
	fprintf(stderr, "\n");
      }
    }
    fprintf(stderr, "\n");
  }

  for (i = 0; i < io->numberofholes; i++) {
    fprintf(stderr, "Hole %4d:", i);
    for (j = 0; j < 2; j++) {
      fprintf(stderr, "  %.6g", io->holelist[i * 2 + j]);
    }
    fprintf(stderr, "\n");
  }
#endif
}
