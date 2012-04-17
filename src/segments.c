#include "segments.h"

int read_segments_header(FILE * input_file, triangulateio * input,
			 int *markers)
{
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;

  stringptr = readline(inputline, input_file, &error_value);
  if (error_value != SUCCESS)
    return error_value;

  input->numberofsegments = (int) strtol(stringptr, &stringptr, 0);

  stringptr = findfield(stringptr);
  if (*stringptr == '\0') {
    *markers = 0;
  } else {
    *markers = (int) strtol(stringptr, &stringptr, 0);
  }

  assert(*markers == 0 || *markers == 1);
  return SUCCESS;
}

void allocate_segments(triangulateio * input)
{
  if (input->numberofsegments == 0)
    return SUCCESS;

  input->segmentlist =
      (int *) malloc(input->numberofsegments * 2 * sizeof(int));
  input->segmentmarkerlist =
      (int *) malloc(input->numberofsegments * sizeof(int));

  assert(input->segmentlist != NULL);
  assert(input->segmentmarkerlist != NULL);
}

int read_segments(FILE * infile, triangulateio * input, int markers,
		  int firstnode)
{
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;
  int *current_segment;
  int *current_marker;
  int i;

  if (input->numberofsegments == 0)
    return SUCCESS;

  for (i = 0; i < input->numberofsegments; i++) {
    current_segment = input->segmentlist + (i * 2);
    current_marker = input->segmentmarkerlist + i;

    stringptr = readline(inputline, infile, &error_value);
    if (error_value != SUCCESS)
      return error_value;

    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      fprintf(stderr, "Error:  Segment %d has no endpoints.\n", i);
      return FORMAT_ERR;
    }
    current_segment[0] =
	(int) strtol(stringptr, &stringptr, 0) - firstnode;

    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      fprintf(stderr,
	      "Error:  Segment %d is missing its second endpoint.\n", i);
      return FORMAT_ERR;
    }
    current_segment[1] =
	(int) strtol(stringptr, &stringptr, 0) - firstnode;

    if (markers) {
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
	*current_marker = 0;
      } else {
	*current_marker = (int) strtol(stringptr, &stringptr, 0);
      }
    }
  }

  siatkonator_log("read %d segments\n", i);

  return 0;
}
