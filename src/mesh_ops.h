#include "common.h"
#include "stdlib.h"
#include "string.h"
#include "siatkonator_hash.h"

void bounding_polygon(struct triangulateio *mesh,
		      struct triangulateio *polygon);
void bounding_polygon_hole(struct triangulateio *mesh,
		      struct triangulateio *polygon);
void add_bounds_as_segments(struct triangulateio *mesh,
	      struct triangulateio *bounding_polygon);
void mesh_cat(struct triangulateio *dst, struct triangulateio *src);
void add_bounding_segments(struct triangulateio *polygon,
    struct triangulateio *source);
void remove_duplicates(struct triangulateio *out, double eps);
