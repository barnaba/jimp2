#include "common.h"

void get_pointmarkerlist_range(triangulateio *mesh, int *low, int *high);
void adjust_markers(siatkonator_program *program, triangulateio *hull, triangulateio *meshes);
int resolve_marker_conflict(siatkonator_program program, int marker_a, int marker_b);
