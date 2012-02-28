#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#include <argtable2.h>
#include <stdio.h>
#include <stdlib.h>
#include "../vendor/triangle/triangle.h"

void siatkonator_log(const char* msg){
  fprintf( stderr, "%s\n", msg);
}

int main(int argc, char ** argv){
  struct arg_dbl *surface = arg_dbl0("a", "surface","<VALUE>", "maximum surface of the triangle");
  struct arg_dbl *angle = arg_dbl0("q", "angle","<VALUE>", "minimum angle used for triangles");
  struct arg_file *input_ele_files = arg_filen("e","elements","<.ELE FILE>",0,argc,"input meshes");
  struct arg_file *input_poly_file = arg_file1(NULL,NULL,"<.POLY FILE>","input polygon");
  struct arg_file *output_ele_file = arg_file0("o","output","<.ELE FILE>","output file");
  struct arg_end *end = arg_end(20);
  int nerrors;
  void *argtable[] = {surface, angle, input_ele_files, input_poly_file, output_ele_file, end};
  char msg[512];

  if (arg_nullcheck(argtable) != 0)
    siatkonator_log("error: insufficient memory");

  nerrors = arg_parse(argc,argv,argtable);
  if (nerrors){
    siatkonator_log("Error parsing command line options");
    return 3;
  }

  if (nerrors==0){

    if (surface->count){
      sprintf(msg, "Maximum surface: %f", surface->dval[0]);
      siatkonator_log(msg);
    }

    if (angle->count){
      sprintf(msg, "Minimal angle: %f", angle->dval[0]);
      siatkonator_log(msg);
    }

    if (input_ele_files->count){
      for(int i=0; i<input_ele_files->count; i++){
        sprintf(msg, "Using mesh defined in %s", input_ele_files->filename[i]);
        siatkonator_log(msg);
      }
    }

    if (output_ele_file->count){
      sprintf(msg, "Results will be saved to %s", output_ele_file->filename[0]);
      siatkonator_log(msg);
    }
  }


  return 0;
}
