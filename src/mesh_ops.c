#include "mesh_ops.h"

void bounding_polygon(struct triangulateio *polygon,
    struct triangulateio *source){
  sh_hash hash; 
  sh_node **element;
  hash_init(&hash, source->numberoftriangles);
  int * current_triangle = source->trianglelist;
  while (current_triangle < source->trianglelist + source->numberoftriangles * 3){
    sh_try(hash, current_triangle[0], current_triangle[1]);
    sh_try(hash, current_triangle[1], current_triangle[2]);
    sh_try(hash, current_triangle[2], current_triangle[0]);
    current_triangle += 3;
  }

  free(hash.values);
}

void add_hole(struct triangulateio *hull,
    struct triangulateio *bounding_polygon){
  return;
}

void mesh_Cat(struct triangulateio *dst, struct triangulateio *src){
  return;
}

