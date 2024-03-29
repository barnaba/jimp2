#include "mesh_ops.h"
#include "markers.h"
#include <math.h>


void reduce(triangulateio *polygon);
void make_new_points_ids(triangulateio *polygon, int *new_numbers, int *pointcount);
void reduce_pointlist(siatkonator_program *program, triangulateio *polygon, int *new_numbers);
void update_segment_refs(triangulateio *polygon, int *new_numbers);
void update_triangle_refs(triangulateio *polygon, int *new_numbers);
void move_point(triangulateio *mesh, int dst, int src);


void bounding_polygon(struct triangulateio *polygon,
    struct triangulateio *source){
  sh_hash hash;
  sh_node *segments;
  sh_node *current_segment;
  int size, i = 0;

  hash_init(&hash, source->numberoftriangles*3);
  triangulatio_to_hash(hash, source);
  segments = sh_as_list(hash, &size);

  polygon->numberofsegments = size;
  polygon->segmentlist = malloc(size * sizeof(int) * 2);

  for(current_segment= segments; current_segment != NULL; current_segment = current_segment->next, i++ ){
    polygon->segmentlist[2*i] = current_segment->id_a;
    polygon->segmentlist[2*i + 1] = current_segment->id_b;
  }

  polygon->numberofpoints = source->numberofpoints;
  polygon->numberofpointattributes = source->numberofpointattributes;
  polygon->pointlist = malloc(polygon->numberofpoints * sizeof(REAL) * DIMENSIONS);
  polygon->pointattributelist = (REAL *) malloc(polygon->numberofpoints * polygon->numberofpointattributes * sizeof(REAL));
  polygon->pointmarkerlist = (int *) malloc(polygon->numberofpoints * sizeof(int));

  memcpy(polygon->pointlist, source->pointlist, polygon->numberofpoints * sizeof(REAL) * DIMENSIONS);
  memcpy(polygon->pointattributelist, source->pointattributelist, polygon->numberofpoints * polygon->numberofpointattributes * sizeof(REAL));
  memcpy(polygon->pointmarkerlist, source->pointmarkerlist, polygon->numberofpoints * sizeof(int));
  reduce(polygon);

  free(hash.values);
  sh_free_list(segments);
}

/*Removes the nodes not referenced by any segment, and then */
/*changes the number of remaining nodes (and references in segments)*/
void reduce(triangulateio *polygon){
  int *new_numbers = malloc(polygon->numberofpoints * sizeof(int));
  int new_pointcount = 0;

  make_new_points_ids(polygon, new_numbers, &new_pointcount);
  reduce_pointlist((siatkonator_program*) NULL, polygon, new_numbers);
  update_segment_refs(polygon, new_numbers);
  polygon->numberofpoints = new_pointcount;

  free(new_numbers); return;
}

// mark all points referenced by segments
// then calculate new point numbers for them (used when removing other points).
void make_new_points_ids(triangulateio *polygon, int *new_numbers, int *pointcount){
  /*TODO: how about a bit array?*/
  *pointcount = 0;
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
      new_numbers[cur_point] = *pointcount;
      (*pointcount)++;
    } else {
      new_numbers[cur_point] = -1;
    }
  }
  free(is_used);
  return;
}

// make points more sparse according to new_numbers array.
// new_numbers[point_number] specifies new number of that point.
// -1 means that point is to be discarded.
void reduce_pointlist(siatkonator_program *program, triangulateio *polygon, int *new_numbers){
  int old_no, new_no;

  for (int cur_point = 0; cur_point < polygon->numberofpoints; ++cur_point){
    if(-1 == new_numbers[cur_point])
      continue;
    old_no = cur_point;
    new_no = new_numbers[old_no];

    /*In place edit, so this has to be true, or data*/
    /*WILL BE overwritten*/
    assert(new_no <= old_no);
    move_point(polygon, new_no, old_no);
    if(program != NULL && polygon->pointmarkerlist[old_no] != polygon->pointmarkerlist[new_no])
      polygon->pointmarkerlist[new_no] = resolve_marker_conflict(*program, polygon->pointmarkerlist[old_no], polygon->pointmarkerlist[new_no]);
  }

}


void move_point(triangulateio *mesh, int dst, int src){
    /*move point coordinates*/
    mesh->pointlist[dst * DIMENSIONS] = mesh->pointlist[src * DIMENSIONS];
    mesh->pointlist[dst * DIMENSIONS + 1] = mesh->pointlist[src * DIMENSIONS + 1];

    /*move point attributes*/
    for (int i=0; i<mesh->numberofpointattributes; i++){
      mesh->pointattributelist[dst * mesh->numberofpointattributes + i] = mesh->pointattributelist[src * mesh->numberofpointattributes + i];
    }

    /*move point markers*/
    if(mesh->pointmarkerlist != NULL){
      mesh->pointmarkerlist[dst] = mesh->pointmarkerlist[src];
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

void update_triangle_refs(triangulateio *polygon, int *new_numbers){
  int *tri_list = polygon->trianglelist;
  for(int tri_no = 0; tri_no < polygon->numberoftriangles; tri_no++){
    for(int thrice=0; thrice < 3; thrice++){
      tri_list[tri_no*3 + thrice] = new_numbers[tri_list[tri_no*3 + thrice]];
    }
  }
}

// adds hole in the polygon. Hole's coordinates are the barycenter of the first
// triangle from source polygon

// TODO: oops, this assumes that first three points are part of the triangle...
// Should work on convex polygon anyway

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

  polygon->numberofholes += 1;
  if (polygon->holelist != NULL)
    polygon->holelist = (REAL *) realloc(polygon->holelist, polygon->numberofholes * 2 * sizeof(REAL));
  else
    polygon->holelist = (REAL *) malloc(polygon->numberofholes * 2 * sizeof(REAL));

  polygon->holelist[2*polygon->numberofholes - 2] = x/3.0;
  polygon->holelist[2*polygon->numberofholes - 1] = y/3.0;
}

void add_bounding_segments(struct triangulateio *hull,
    struct triangulateio *bounding_polygon){

  int point_offset = hull->numberofpoints;
  int segment_offset = hull->numberofsegments;
  int point_count = hull->numberofpoints + bounding_polygon->numberofpoints;
  int segment_count = hull->numberofsegments + bounding_polygon->numberofsegments;

  // helper variable used in loops utilizing offsets
  int offseted_no = 0;


  // realloc point- and segmentlists to accomodate new points and segments
  hull->pointlist = (REAL *) realloc(hull->pointlist, point_count * 2 * sizeof(REAL));
  hull->pointattributelist = (REAL *) realloc(hull->pointattributelist, point_count * hull->numberofpointattributes * sizeof(REAL));
  hull->pointmarkerlist = (int *) realloc(hull->pointmarkerlist, point_count * sizeof(int));
  hull->segmentlist = (int *) realloc(hull->segmentlist, segment_count * 2 * sizeof(int));

  // add each point to the hull
  for(int point_no = 0; point_no < bounding_polygon->numberofpoints; point_no++){
    offseted_no = point_no + point_offset;
    hull->pointlist[2*offseted_no] = bounding_polygon->pointlist[2*point_no];
    hull->pointlist[2*offseted_no+1] = bounding_polygon->pointlist[2*point_no+1];
    for(int i=0; i<bounding_polygon->numberofpointattributes; ++i){
      hull->pointattributelist[offseted_no * hull->numberofpointattributes + i] = bounding_polygon->pointattributelist[point_no * bounding_polygon->numberofpointattributes + i];
    }
    hull->pointmarkerlist[offseted_no] = bounding_polygon->pointmarkerlist[point_no];
  }

  // add each segment to the hull
  for(int seg_no = 0; seg_no < bounding_polygon->numberofsegments; seg_no++ ){
    offseted_no = seg_no + segment_offset;
    hull->segmentlist[offseted_no*2] = bounding_polygon->segmentlist[2*seg_no] + point_offset;
    hull->segmentlist[offseted_no*2 + 1] = bounding_polygon->segmentlist[2*seg_no + 1] + point_offset;
  }

  // update point and segment count
  hull->numberofpoints = point_count;
  hull->numberofsegments = segment_count;

  return;
}

void mesh_cat(struct triangulateio *dst, struct triangulateio *src){
  int point_count = dst->numberofpoints + src->numberofpoints;
  int triangle_count = dst->numberoftriangles + src->numberoftriangles;
  int point_offset = dst->numberofpoints;
  int triangle_offset = dst->numberoftriangles;
  int offseted_no;

  // realloc pointlists to accomodate new points and segments
  dst->pointlist = (REAL *) realloc(dst->pointlist, point_count * 2 * sizeof(REAL));
  dst->pointattributelist = (REAL *) realloc(dst->pointattributelist, point_count * dst->numberofpointattributes * sizeof(REAL));
  dst->pointmarkerlist = (int *) realloc(dst->pointmarkerlist, point_count * sizeof(int));
  dst->trianglelist = (int *) realloc(dst->trianglelist, triangle_count * 3 * sizeof(int));

  memcpy(dst->pointlist + dst->numberofpoints * 2, src->pointlist, src->numberofpoints * 2 * sizeof(REAL));
  if(src->pointattributelist != NULL)
    memcpy(dst->pointattributelist + dst->numberofpoints * dst->numberofpointattributes, src->pointattributelist, src->numberofpoints * dst->numberofpointattributes * sizeof(REAL));
  if(src->pointmarkerlist != NULL)
    memcpy(dst->pointmarkerlist + dst->numberofpoints, src->pointmarkerlist, src->numberofpoints * sizeof(int));

  for(int tri_no=0; tri_no < src->numberoftriangles; ++tri_no){
    offseted_no = tri_no + triangle_offset;
    for (int i=0; i<3; i++)
      dst->trianglelist[3*offseted_no + i] = src->trianglelist[3*tri_no + i] + point_offset;
  }
  dst->numberoftriangles = triangle_count;
  dst->numberofpoints = point_count;
  return;
}

void remove_duplicates(siatkonator_program program, struct triangulateio *mesh){
  int *new_numbers = malloc(mesh->numberofpoints * sizeof(int));
  char *original_of = malloc(mesh->numberofpoints * sizeof(char));
  int new_pointcount = 0;
  REAL x,y;

  for(int i=0; i<mesh->numberofpoints; i++)
    original_of[i] = -1;

  for(int point=0; point < mesh->numberofpoints; ++point){
    new_numbers[point] = point;
  }

  /*make list of duplicate nodes:
   *from = originals, to = dups*/
  for(int point=0; point < mesh->numberofpoints; ++point){
    x = mesh->pointlist[point*2];
    y = mesh->pointlist[point*2+1];
    for(int other=point+1; other < mesh->numberofpoints; ++other){
      if(fabs(x - mesh->pointlist[other*2]) < program.eps && fabs(y - mesh->pointlist[other*2+1]) < program.eps){
        if (original_of[point] == -1)
          original_of[other] = point;
        else
          original_of[other] = original_of[point];
      }
    }
  }

  for(int point=0; point < mesh->numberofpoints; ++point){
    if (original_of[point] == -1){
      new_numbers[point] = new_pointcount;
      new_pointcount++;
    } else {
      new_numbers[point] = original_of[point];
    }
  }

  reduce_pointlist(&program, mesh, new_numbers);
  update_segment_refs(mesh, new_numbers);
  update_triangle_refs(mesh, new_numbers);
  mesh->numberofpoints = new_pointcount;

  free(new_numbers); return;
}
