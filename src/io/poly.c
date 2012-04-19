#include "poly.h"
int read_poly_file(const char *filename, triangulateio * input)
{
  int error_value = 0;
  FILE *polyfile;
  int markers;
  int firstnode;

  siatkonator_log("reading %s: \n", basename((char *) filename));

  polyfile = fopen(filename, "r");
  if (polyfile == (FILE *) NULL) {
    printf("  Error:  Cannot access file %s.\n", filename);
    return FILE_ERR;
  }

  error_value = read_nodes_header(polyfile, input, &markers);
  if (error_value != SUCCESS)
    return error_value;

  allocate_nodes(input);

  error_value = read_nodes(polyfile, input, &markers, &firstnode);
  if (error_value != SUCCESS)
    return error_value;


  error_value = read_segments_header(polyfile, input, &markers);
  if (error_value != SUCCESS)
    return error_value;

  allocate_segments(input);
  error_value = read_segments(polyfile, input, markers, firstnode);
  if (error_value != SUCCESS)
    return error_value;

  error_value = read_holes_header(polyfile, input);
  if (error_value != SUCCESS)
    return error_value;

  allocate_holes(input);
  error_value = read_holes(polyfile, input);
  if (error_value != SUCCESS)
    return error_value;


  report(input, markers, 0, 0, 1, 0, 0);
  return SUCCESS;
}
