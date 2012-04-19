#include "ele.h"

int read_ele_file(const char *ele_filename, triangulateio *input, int firstnode){
  int error_value = 0;
  FILE *ele_file;

  ele_file = fopen(ele_filename, "r");
  if (ele_file == (FILE *) NULL) {
    printf("  Error:  Cannot access file %s.\n", ele_filename);
    return FILE_ERR;
  }

  error_value = read_elements_header(ele_file, input);
  if (error_value != SUCCESS)
    return error_value;

  allocate_elements(input);

  error_value = read_elements(ele_file, input, firstnode);
  if (error_value != SUCCESS)
    return error_value;

  return SUCCESS;
}

int read_elements_header(FILE * ele_file, triangulateio *input){
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;

  stringptr = readline(inputline, ele_file, &error_value);
  if (error_value != SUCCESS)
    return error_value;

  input->numberoftriangles = (int) strtol(stringptr, &stringptr, 0);

  //skipping the field with number of nodes, because we hope it's always 3
  stringptr = findfield(stringptr);

  stringptr = findfield(stringptr);
  if (*stringptr == '\0') {
    input->numberoftriangleattributes = 0;
  } else {
    input->numberoftriangleattributes = (int) strtol(stringptr, &stringptr, 0);
  }

  return SUCCESS;

}

void allocate_elements(triangulateio *input){
  if (input->numberoftriangles == 0)
    return;

  input->trianglelist =
      (int *) malloc(input->numberoftriangles * 3 * sizeof(int));
  input->triangleattributelist =
      (REAL *) malloc(input->numberoftriangleattributes * sizeof(REAL));

  assert(input->trianglelist != NULL);
  assert(input->triangleattributelist != NULL);
}

int read_elements(FILE * ele_file, triangulateio *input, int firstnode){
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;
  int triangle_id;
  int *current_triangle;
  REAL *current_attribute;
  int i;

  if (input->numberoftriangles == 0)
    return SUCCESS;

  for (i = 0; i < input->numberoftriangles; i++) {
    current_triangle = input->trianglelist + (i * 3);
    current_attribute =
	input->triangleattributelist + (i * input->numberoftriangles);

    stringptr = readline(inputline, ele_file, &error_value);
    if (error_value != SUCCESS)
      return error_value;
    triangle_id = (int) strtol(stringptr, &stringptr, 0);
    if (triangle_id != firstnode + i) {
      fprintf(stderr,
	      "Error:  triangle %d is out of order (expected %d).\n",
	      triangle_id, firstnode + i + 1);
      return FORMAT_ERR;
    }

    for (int thrice=0; thrice<3; ++thrice){
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        fprintf(stderr, "Error:  triangle %d doesn't have enough nodes.\n",
          firstnode + i);
        return FORMAT_ERR;
      }
      current_triangle[thrice] = (REAL) strtol(stringptr, &stringptr, 0);
    }

    /* Read the triangle attributes. */
    for (int j = 0; j < input->numberoftriangleattributes; j++) {
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        current_attribute[j] = 0.0;
      } else {
        current_attribute[j] = (REAL) strtod(stringptr, &stringptr);
      }
    }

  }

  siatkonator_log("read %d triangles \n", i);

  return 0;
}
