#include "common.h"
#include "stdlib.h"
#include "string.h"
#include "siatkonator_hash.h"

void bounding_polygon(struct triangulateio *mesh,
		      struct triangulateio *polygon);
void add_hole(struct triangulateio *mesh,
	      struct triangulateio *bounding_polygon);
void mesh_Cat(struct triangulateio *dst, struct triangulateio *src);
void bounding_polygon_hole(struct triangulateio *polygon,
    struct triangulateio *source);
