#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "poly.h"
#include "node.h"


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

  /*
   * TriangulateIO instances
   */

  triangulateio hull;
  triangulateio *meshes;

  /*
   * Argument parsing
   */

  if (arg_nullcheck(argtable) != 0) {
    siatkonator_log("error: insufficient memory");
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
      siatkonator_log("Maximum surface: %f", surface->dval[0]);
    }

    if (angle->count) {
      siatkonator_log("Minimal angle: %f", angle->dval[0]);
    }

    if (input_ele_files->count) {
      for (int i = 0; i < input_ele_files->count; i++) {
	siatkonator_log("Using mesh defined in %s",
			input_ele_files->filename[i]);
      }
    }

    if (output_ele_file->count) {
      siatkonator_log("Results will be saved to %s",
		      output_ele_file->filename[0]);
    }
  }

  /*
   * Getting input from files
   */

  read_poly_file(input_poly_file->filename[0], &hull);


  return SUCCESS;
}
