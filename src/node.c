#include "node.h"

int read_nodes_header(FILE *input_file, triangulateio *input, int *markers){
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int mesh_dim;
  int error_value = 0;

  stringptr = readline(inputline, input_file, &error_value);
  if (error_value != SUCCESS) return error_value;

  input->numberofpoints = (int) strtol(stringptr, &stringptr, 0);

  stringptr = findfield(stringptr);
  if (*stringptr == '\0') {
    mesh_dim = 2;
  } else {
    mesh_dim = (int) strtol(stringptr, &stringptr, 0);
  }

  stringptr = findfield(stringptr);
  if (*stringptr == '\0') {
    input->numberofpointattributes = 0;
  } else {
    input->numberofpointattributes = (int) strtol(stringptr, &stringptr, 0);
  }

  stringptr = findfield(stringptr);
  if (*stringptr == '\0') {
    *markers = 0;
  } else {
    *markers = (int) strtol(stringptr, &stringptr, 0);
  }

  if (input->numberofpoints == 0) {
    /*
     *if poly file claims 0 vertices it means that vertices should be read
     *from .node file
     */
    printf("  Error: External node file is not supported!\n");
    return NOT_SUPPORTED;
  }

  assert(mesh_dim == 2);
  assert(*markers == 0 || *markers == 1);
  assert(input->numberofpoints >= 3);

  return SUCCESS;
}

void allocate_nodes(triangulateio *input){
  input->pointlist = (REAL *) malloc(input->numberofpoints * 2 * sizeof(REAL));
  input->pointattributelist = (REAL *) malloc(input->numberofpoints *
                                          input->numberofpointattributes *
                                          sizeof(REAL));
  input->pointmarkerlist = (int *) malloc(input->numberofpoints *
      sizeof(int));

  assert(input->pointmarkerlist != NULL);
  assert(input->pointattributelist != NULL);
}

int read_nodes(FILE *infile, triangulateio *input, int *markers, int *firstnode){
  char *stringptr;
  char inputline[INPUTLINESIZE];
  int error_value = 0;
  int vertex_id;
  REAL *current_point;
  REAL *current_attribute;
  int *current_marker;
  int i;

  for (i = 0; i < input->numberofpoints; i++) {
    current_point = input->pointlist + (i * DIMENSIONS);
    current_attribute = input->pointattributelist + (i * input->numberofpointattributes );
    current_marker = input->pointmarkerlist + i;

    stringptr = readline(inputline, infile, &error_value);
    if (error_value != SUCCESS) return error_value;
    vertex_id = (int) strtol(stringptr, &stringptr, 0); 
    if (i == 0) {
      *firstnode = vertex_id;
      assert((*firstnode == 0) || (*firstnode == 1));
    } else if (vertex_id != *firstnode + i){
      fprintf(stderr, "Error:  Vertex %d is out of order (expected %d).\n", vertex_id, *firstnode + i + 1 );
      return FORMAT_ERR;
    }

    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      fprintf(stderr, "Error:  Vertex %d has no x coordinate.\n", *firstnode + i);
      return FORMAT_ERR;
    }
    current_point[0] = (REAL) strtod(stringptr, &stringptr); //x

    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      fprintf(stderr, "Error:  Vertex %d has no y coordinate.\n", *firstnode + i);
      return FORMAT_ERR;
    }
    current_point[1] = (REAL) strtod(stringptr, &stringptr); //y

    /* Read the vertex attributes. */
    for (int j = 0; j < input->numberofpointattributes; j++) {
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        current_attribute[j] = 0.0;
      } else {
        current_attribute[j] = (REAL) strtod(stringptr, &stringptr);
      }
    }

    if (*markers) {
      /* Read a vertex marker. */
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        *current_marker = 0;
      } else {
        *current_marker = (int) strtol(stringptr, &stringptr, 0);
      }
    } else {
      /* If no markers are specified in the file, they default to zero. */
      *current_marker = 0;
    }
    /* Determine the smallest and largest x and y coordinates. */

  }

  siatkonator_log("read %d nodes\n", i);

  return 0;
}

