#include <limits.h>
#include <stdlib.h>

#include "markers.h"

// get lowest and highest marker for mesh
void get_pointmarkerlist_range(triangulateio *mesh, int *low, int *high){
  int *current;
  *high = INT_MIN;
  *low = INT_MAX;

  for(current=mesh->pointmarkerlist; current < mesh->pointmarkerlist + mesh->numberofpoints; current++){
    if (*current > *high){
      *high = *current;
    } 
    if(*current < *low){
      *low = *current;
    }
  }
}

// calculate offsets that will remove overlaps in markers
int *get_offsets(siatkonator_program *program){
  // `mesh_count + 1`, because there's poly file with markers!
  int *offsets = (int*) malloc((program->mesh_count + 1)*sizeof(int));
  int prev_end = -1;

  for(int i=0; i < program->mesh_count + 1; ++i){
    // start right after previous range
    offsets[i] = (prev_end + 1) - program->marker_range_start[i];
    prev_end = program->marker_range_end[i] + offsets[i];
  }

  return offsets;
}

// apply offsets to all markers in mesh
void offset_markers(triangulateio *mesh, int offset){
  for(int i=0; i < mesh->numberofpoints; ++i)
    mesh->pointmarkerlist[i] += offset;
}

void conflict_resolution_init(siatkonator_program *program, int highest_marker){
  int size = INT_MAX - highest_marker;
  program->conflicted_markers_offset = highest_marker + 1;
  //checking for prime just took too long here, and it won't be dense
  program->conflicted_markers_size = size; 
}

// calculate hash-like value for conflicted marker (occurs when two points with different markers are merged into one).
int resolve_marker_conflict(siatkonator_program program, int marker_a, int marker_b){
  return program.conflicted_markers_offset + (marker_a * 9973 + marker_b * 57) % program.conflicted_markers_size;
}

// orchestrates the process of marker overlap removal
void adjust_markers(siatkonator_program *program, triangulateio *hull, triangulateio *meshes){
  int *offsets = get_offsets(program);
  for(int i=0; i < program->mesh_count + 1; ++i){
    siatkonator_log(DEBUG, "marker range change for mesh %d: %d-%d --> %d-%d\n", i, program->marker_range_start[i], program->marker_range_end[i],program->marker_range_start[i] + offsets[i], program->marker_range_end[i] + offsets[i] );
  }

  for(int i=0; i < program->mesh_count; ++i){
    offset_markers(meshes + i, offsets[i]);
  }
  offset_markers(hull, offsets[program->mesh_count]);

  // last mesh will get highest offseted end of range
  conflict_resolution_init(program, program->marker_range_end[program->mesh_count] + offsets[program->mesh_count]);
  free(offsets);
}
