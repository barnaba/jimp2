#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "io/siatkonator_io.h"
#include "io/poly.h"
#include "io/node.h"

void initialize_mid(triangulateio *mid);
void initialize_vorout(triangulateio *vorout);
void initialize_out(triangulateio *out);


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
  struct arg_file *input_ele_files =
      arg_filen("e", "elements", "<.ELE FILE>", 0, argc, "input meshes");
  struct arg_file *input_poly_file =
      arg_file1(NULL, NULL, "<.POLY FILE>", "input polygon");
  struct arg_file *output_ele_file =
      arg_file0("o", "output", "<.ELE FILE>", "output file");
  struct arg_end *end = arg_end(20);
  int nerrors;
  void *argtable[] =
      { help, surface, angle, input_ele_files, output_ele_file,
    input_poly_file, end
  };
  int error_value = 0;

  /*
   * TriangulateIO instances
   */

  triangulateio hull;
  triangulateio *meshes;
  triangulateio mid, out, vorout;

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
    }

    if (angle->count) {
      siatkonator_log(INFO, "Minimal angle: %f", angle->dval[0]);
    }

    if (output_ele_file->count) {
      siatkonator_log(INFO, "Results will be saved to %s",
		      output_ele_file->filename[0]);
    }
  }

  /*
   * Getting input from files
   */

  siatkonator_log(INFO, "*** reading hull: %s\n", input_poly_file->filename[0]);
  error_value = read_poly_file(input_poly_file->filename[0], &hull);
  if (error_value != SUCCESS)
    return error_value;
  siatkonator_log(DEBUG, "--------\n");

  hull.numberofregions = 0; //TODO wczytywanie regionów

  if (input_ele_files->count) {
    meshes = malloc(input_ele_files->count * sizeof(triangulateio));
    for (int i=0; i<input_ele_files->count; ++i){
      siatkonator_log(INFO, "*** reading %s\n", input_ele_files->filename[i]);
      error_value = ele_to_triangulateio(input_ele_files->filename[i], meshes + i);
      if (error_value != SUCCESS)
        return error_value;
      siatkonator_log(DEBUG, "--------\n");
      siatkonator_log(DEBUG, "*** result of reading %s\n", input_ele_files->filename[i]);
      report(meshes + i, 0, 1, 0, 0, 0, 0);
      siatkonator_log(DEBUG, "--------\n");
    }
  }

  initialize_mid(&mid);
  initialize_vorout(&vorout);
  initialize_out(&out);

  triangulate("pczAevn", &hull, &mid, &vorout); //TODO wziąć pod uwagę opcje podane przez usera
  mid.trianglearealist = (REAL *) malloc(mid.numberoftriangles * sizeof(REAL));
  mid.trianglearealist[0] = 3.0;
  mid.trianglearealist[1] = 1.0;
  triangulate("prazBP", &mid, &out, (struct triangulateio *) NULL);
  siatkonator_log(DEBUG, "*** triangulation result:\n");
  report(&out, 0, 1, 0, 0, 0, 0);
  siatkonator_log(DEBUG, "--------\n");

  return SUCCESS;
}

void initialize_mid(triangulateio *mid){
  /*copy from tricall*/
  mid->pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of point attributes is zero: */
  mid->pointattributelist = (REAL *) NULL;
  mid->pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
  mid->trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  mid->triangleattributelist = (REAL *) NULL;
  mid->neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
  /* Needed only if segments are output (-p or -c) and -P not used: */
  mid->segmentlist = (int *) NULL;
  /* Needed only if segments are output (-p or -c) and -P and -B not used: */
  mid->segmentmarkerlist = (int *) NULL;
  mid->edgelist = (int *) NULL;             /* Needed only if -e switch used. */
  mid->edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
  mid->numberofholes = 0;
  return;
}

void initialize_vorout(triangulateio *vorout){
  /*copy from tricall*/
  vorout->pointlist = (REAL *) NULL;        /* Needed only if -v switch used. */
  /* Needed only if -v switch used and number of attributes is not zero: */
  vorout->pointattributelist = (REAL *) NULL;
  vorout->edgelist = (int *) NULL;          /* Needed only if -v switch used. */
  vorout->normlist = (REAL *) NULL;         /* Needed only if -v switch used. */
  vorout->numberofholes = 0;
  return;
}

void initialize_out(triangulateio *out){
  out->pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of attributes is zero: */
  out->pointattributelist = (REAL *) NULL;
  out->trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  out->triangleattributelist = (REAL *) NULL;
  out->numberofholes = 0;
  return;
}
