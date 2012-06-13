#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "io/siatkonator_io.h"
#include "io/poly.h"
#include "io/node.h"
#include "io/ele.h"
#include "mesh_ops.h"
#include "markers.h"

#define MAXFILENAMELEN 500
#define EPSILON 0.005

void initialize_triangulateio(triangulateio *mid);
FILE * open_for_writing(struct arg_file *output_file, const char * extension);


int main(int argc, char **argv)
{

  /*
   * Argtable variables
   */

  struct arg_lit *help = arg_lit0("h", "help", "print usage");
  struct arg_dbl *surface = arg_dbl0("a", "surface", "<VALUE>",
				     "maximum surface of the triangle");
  struct arg_dbl *angle = arg_dbl0("q", "angle", "<VALUE>",
				   "minimum angle used for triangles");
  struct arg_dbl *epsilon = arg_dbl0("d", "eps", "<VALUE>",
				   "epsilon used for comparing node coordinates");
  struct arg_file *input_ele_files =
      arg_filen("e", "elements", "<.ELE FILE>", 0, argc, "input meshes");
  struct arg_file *input_poly_file =
      arg_file1(NULL, NULL, "<.POLY FILE>", "input polygon");
  struct arg_file *output_file =
      arg_file0("o", "output", "<.ELE FILE>", "output file");
  struct arg_end *end = arg_end(20);
  int nerrors;
  void *argtable[] =
      { help, surface, angle, input_ele_files, output_file,
    input_poly_file, end
  };
  int error_value = 0;
  FILE *ele_out, *node_out;
  siatkonator_program self;
  self.eps = EPSILON;

  /*
   * TriangulateIO instances
   */

  triangulateio hull;
  triangulateio *meshes;
  triangulateio *bounds;
  triangulateio mid, out, vorout;
  initialize_triangulateio(&hull);
  char* optstring = "";

  /*
   * Argument parsing
   */

  if (arg_nullcheck(argtable) != 0) {
    siatkonator_log(INFO, "error: insufficient memory");
    return ARG_ERR;
  }

  nerrors = arg_parse(argc, argv, argtable);
  if (nerrors > 0) {
    arg_print_errors(stderr, end, "siatkonator");
    fprintf(stderr, "Error parsing command line options.");
    return ARG_ERR;
  }

  if (help->count) {
    fprintf(stderr, "usage: siatkanator ");
    arg_print_syntaxv(stderr, argtable, "\n");
    return SUCCESS;
  }

  if (nerrors == 0) {
    if (surface->count) {
      siatkonator_log(INFO, "Maximum surface: %f", surface->dval[0]);
      asprintf(&optstring, "%sa%g", optstring, surface->dval[0]); 
    }

    if (angle->count) {
      siatkonator_log(INFO, "Minimal angle: %f", angle->dval[0]);
      asprintf(&optstring, "%sq%g", optstring, angle->dval[0]); 
    }

    if (epsilon->count) {
      siatkonator_log(INFO, "epsilon: %f", epsilon->dval[0]);
      self.eps = epsilon->dval[0];
    }

    if (output_file->count) {
      siatkonator_log(INFO, "Results will be saved to %s.ele and %s.node",
		      output_file->filename[0], output_file->filename[0]);
    }

  }
  asprintf(&optstring, "p%szV", optstring);

  /*
   * Getting input from files
   */

  siatkonator_log(INFO, "*** reading hull: %s\n", input_poly_file->filename[0]);
  error_value = read_poly_file(input_poly_file->filename[0], &hull);
  if (error_value != SUCCESS)
    return error_value;
  siatkonator_log(DEBUG, "--------\n");

  self.mesh_count = input_ele_files->count;
  
  if (self.mesh_count) {
    // `mesh_count + 1`, because there's poly file with markers!
    self.marker_range_start = (int *) malloc((self.mesh_count+1) * sizeof(int));
    self.marker_range_end = (int *) malloc((self.mesh_count+1) * sizeof(int));

    meshes = malloc(self.mesh_count * sizeof(triangulateio));
    bounds = malloc(sizeof(triangulateio));

    get_pointmarkerlist_range(&hull, self.marker_range_start + self.mesh_count, self.marker_range_end + self.mesh_count);

    for (int i=0; i < self.mesh_count; ++i){
      initialize_triangulateio(meshes+i);
      initialize_triangulateio(bounds);

      siatkonator_log(INFO, "*** reading %s\n", input_ele_files->filename[i]);

      error_value = ele_to_triangulateio(input_ele_files->filename[i], meshes + i);

      if (error_value != SUCCESS)
        goto fail;

      if ((meshes+i)->numberofpointattributes != hull.numberofpointattributes){
        siatkonator_log(INFO, "*** Error - number of point attributes is different!");
        goto fail;
      }

      siatkonator_log(DEBUG, "--------\n");
      siatkonator_log(DEBUG, "*** result of reading %s\n", input_ele_files->filename[i]);
      report(meshes + i, 1, 1, 0, 0, 0, 0);
      siatkonator_log(DEBUG, "--------\n");
      siatkonator_log(DEBUG, "*** generating bounding polygon from %s\n", input_ele_files->filename[i]);

      bounding_polygon(bounds, meshes + i);
      bounding_polygon_hole(&hull, meshes + i);

      report(bounds, 0, 1, 0, 1, 0, 0);
      siatkonator_log(DEBUG, "--------\n");

      add_bounding_segments(&hull, bounds);
      get_pointmarkerlist_range(meshes+i, self.marker_range_start+i, self.marker_range_end+i);
      siatkonator_log(DEBUG, "markers in range: %d - %d \n", self.marker_range_start[i], self.marker_range_end[i]);
    }
  }
  adjust_markers(&self, &hull, meshes);

  initialize_triangulateio(&mid);
  initialize_triangulateio(&vorout);
  initialize_triangulateio(&out);

  siatkonator_log(DEBUG, "*** triangulation input:\n");
  report(&hull, 1, 0, 0, 1, 0, 0);
  siatkonator_log(DEBUG, "--------\n");
  siatkonator_log(DEBUG, "*** using option string: \"%s\"\n", optstring);
  triangulate(optstring, &hull, &out, (struct triangulatio*) NULL);
  siatkonator_log(DEBUG, "*** triangulation result:\n");
  report(&out, 0, 1, 0, 0, 0, 0);
  siatkonator_log(DEBUG, "--------\n");

  if (input_ele_files->count) {
    for (int i=0; i<input_ele_files->count; ++i){
      mesh_cat(&out, meshes + i);
    }
  }
  siatkonator_log(DEBUG, "*** Concatenation result:\n");
  report(&out, 0, 1, 0, 0, 0, 0);
  siatkonator_log(DEBUG, "--------\n");

  remove_duplicates(self, &out);

  siatkonator_log(DEBUG, "*** Dup removal result:\n");
  report(&out, 0, 1, 0, 0, 0, 0);
  siatkonator_log(DEBUG, "--------\n");


  /*
   * write the results
   */

  node_out = open_for_writing(output_file, ".node");
  write_node(node_out, &out);
  if (node_out != stdout){
    fclose(node_out);
  }

  ele_out  = open_for_writing(output_file, ".ele");
  write_ele(ele_out, &out);
  if (ele_out != stdout){
    fclose(ele_out);
  }


fail:
  if (input_ele_files->count) {
    // TODO deeper free!
    free(meshes);
    // TODO deeper free!
    free(bounds);
  }
  /*free(optstring);*/
  return error_value;
}

void initialize_triangulateio(triangulateio *mid){
  mid->pointlist = (REAL *) NULL;
  mid->pointattributelist = (REAL *) NULL;
  mid->pointmarkerlist = (int *) NULL;
  mid->trianglelist = (int *) NULL;
  mid->triangleattributelist = (REAL *) NULL;
  mid->trianglearealist = (REAL *) NULL;
  mid->neighborlist = (int  *) NULL;
  mid->segmentlist = (int  *) NULL;
  mid->segmentmarkerlist = (int  *) NULL;
  mid->holelist = (REAL *) NULL;
  mid->regionlist = (REAL *) NULL;
  mid->edgelist = (int  *) NULL;
  mid->edgemarkerlist = (int  *) NULL;
  mid->normlist = (REAL *) NULL;
  mid->numberofpoints = 0;
  mid->numberofsegments = 0;
  mid->numberofregions = 0;
  mid->numberofcorners = 0;
  mid->numberoftriangleattributes= 0;
  mid->numberoftriangles = 0;
  return;
}

FILE * open_for_writing(struct arg_file *output_file, const char * extension){
  FILE * file;
  char *filename = "";
  char buffer[MAXFILENAMELEN] = "";
  if (output_file->count) {
    filename = strncat(buffer, output_file->filename[0], MAXFILENAMELEN - strlen(extension) - 1);
    filename = strcat(filename, extension);
    file = fopen(filename, "w");
    if (file == NULL){
      siatkonator_log(INFO, "%s: Can't open %s for writing, falling back to stdout!\n", extension, filename);
      file = stdout;
    }
    siatkonator_log(INFO, "%s: Results will be saved to %s\n", extension, filename);
  } else {
    file = stdout;
    siatkonator_log(INFO, "%s: Results will be printed to stdout;\n", extension, filename);
  }
  return file;
}

