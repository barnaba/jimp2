#include "mesh_ops.h"


void reduce(triangulateio *polygon);
void make_new_points_ids(triangulateio *polygon, int *new_numbers);
void reduce_pointlist(triangulateio *polygon, int *new_numbers);
void update_segment_refs(triangulateio *polygon, int *new_numbers);


void bounding_polygon(struct triangulateio *polygon,
    struct triangulateio *source){
  sh_hash hash;
  sh_node *segments;
  sh_node *current_segment;
  int size, i = 0;

  hash_init(&hash, source->numberoftriangles);
  triangulatio_to_hash(hash, source);
  segments = sh_as_list(hash, &size);

  polygon->numberofsegments = size;
  polygon->segmentlist = malloc(size * sizeof(int) * 2);

  for(current_segment= segments; current_segment != NULL; current_segment = current_segment->next, i++ ){
    polygon->segmentlist[2*i] = current_segment->id_a;
    polygon->segmentlist[2*i + 1] = current_segment->id_b;
  }

  polygon->numberofpoints = source->numberofpoints;
  polygon->pointlist = malloc(polygon->numberofpoints * sizeof(REAL) * DIMENSIONS);
  memcpy(polygon->pointlist, source->pointlist, polygon->numberofpoints * sizeof(REAL) * DIMENSIONS);
  reduce(polygon);

  // TODO - remove not needed points!

  free(hash.values);
  sh_free_list(segments);
}

/*Removes the nodes not referenced by any segment, and then */
/*changes the number of remaining nodes (and references in segments)*/
void reduce(triangulateio *polygon){
  int *new_numbers = malloc(polygon->numberofpoints * sizeof(int));

  make_new_points_ids(polygon, new_numbers);
  reduce_pointlist(polygon, new_numbers);
  update_segment_refs(polygon, new_numbers);

  free(new_numbers); return;
}

void make_new_points_ids(triangulateio *polygon, int *new_numbers){
  /*TODO: how about a bit array?*/
  int pointcount = 0;
  int *is_used = malloc(polygon->numberofpoints * sizeof(int));
  memset(is_used, '\0', polygon->numberofpoints * sizeof(int));

  /*mark used points*/
  for (int cur_segment = 0; cur_segment < polygon->numberofsegments; ++cur_segment){
    for(int twice = 0; twice < 2; twice++){
      int point_number = polygon->segmentlist[cur_segment*2 + twice];
      is_used[point_number] = 1;
    }
  }

  /*assign new point numbers*/
  for (int cur_point = 0; cur_point < polygon->numberofpoints; ++cur_point){
    if (1 == is_used[cur_point]){
      new_numbers[cur_point] = pointcount;
      pointcount++;
    } else {
      new_numbers[cur_point] = -1;
    }
  }
  free(is_used); 
  return;
}

void reduce_pointlist(triangulateio *polygon, int *new_numbers){
  int old_no, new_no;
  REAL *pointlist = polygon->pointlist;
  REAL *attributelist = polygon->pointattributelist;
  int atr_count = polygon->numberofpointattributes;
  int *markers = polygon->pointmarkerlist;

  for (int cur_point = 0; cur_point < polygon->numberofpoints; ++cur_point){
    if(-1 == new_numbers[cur_point])
      continue;

    old_no = cur_point;
    new_no = new_numbers[old_no];

    /*In place edit, so this has to be true, or data*/
    /*WILL BE overwritten*/

    assert(new_no <= old_no);

    /*move point coordinates*/
    pointlist[new_no * DIMENSIONS] = pointlist[old_no * DIMENSIONS];
    pointlist[new_no * DIMENSIONS + 1] = pointlist[old_no * DIMENSIONS + 1];

    /*move point attributes*/
    for (int i=0; i<atr_count; i++){
      attributelist[new_no * atr_count + i] = attributelist[old_no * atr_count + i];
    }

    /*move point markers*/
    if(markers != NULL){
      markers[new_no] = markers[old_no];
    }

  }

}

void update_segment_refs(triangulateio *polygon, int *new_numbers){
  int *seg_list = polygon->segmentlist;
  for(int seg_no = 0; seg_no < polygon->numberofsegments; seg_no++){
    for(int twice=0; twice < 2; twice++){
      seg_list[seg_no*2 + twice] = new_numbers[seg_list[seg_no*2 + twice]];
    }
  }
}

void bounding_polygon_hole(struct triangulateio *polygon,
    struct triangulateio *source){
  REAL x = 0;
  REAL y = 0;
  int triangle_id;

  //calculate barycenter for the hole
  for(int i=0; i<3; i++){
    triangle_id = source->trianglelist[i];
    x += source->pointlist[2 * triangle_id];
    y += source->pointlist[2 * triangle_id + 1];
  }

  polygon->numberofholes = 1;
  polygon->holelist = malloc(sizeof(REAL) * 2);
  polygon->holelist[0] = x/3.0;
  polygon->holelist[1] = y/3.0;
}

void add_hole(struct triangulateio *hull,
    struct triangulateio *bounding_polygon){
  /*for(int seg_no = 0; seg_no < bounding_polygon.numberofsegments; seg_no++ ){*/

  /*}*/
  return;
}

void mesh_Cat(struct triangulateio *dst, struct triangulateio *src){
  return;
}
