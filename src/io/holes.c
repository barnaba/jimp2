#include "holes.h"

int read_holes_header(FILE *input_file, triangulateio *input){
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;

  stringptr = readline(inputline, input_file, &error_value);
  if (error_value != SUCCESS)
    return error_value;

  input->numberofholes = (int) strtol(stringptr, &stringptr, 0);
  return SUCCESS;
}

void allocate_holes(triangulateio *input){
  if (input->numberofholes == 0)
    return;

  input->holelist =
    (REAL *) malloc(input->numberofholes * 2 * sizeof(REAL));
  assert(input->holelist != NULL);
}


int read_holes(FILE *input_file, triangulateio *input){
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;
  REAL *current_hole;
  int i;

  if (input->numberofholes == 0)
    return SUCCESS;

  current_hole = input->holelist;
  for (i = 0; i < input->numberofholes; i++) {

    stringptr = readline(inputline, input_file, &error_value);
    if (error_value != SUCCESS)
      return error_value;

    /*twice - for x and y*/
    for(int twice=0; twice<2; ++twice){

      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        fprintf(stderr, "Error: Hole %d is missing coordinate.\n", i);
        return FORMAT_ERR;
      }
      *current_hole = (REAL) strtod(stringptr, &stringptr);	//x or y
      ++current_hole;
    }
  }

  siatkonator_log(DEBUG, "read %d holes\n", i);

  return 0;
}

