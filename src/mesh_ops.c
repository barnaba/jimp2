#include "mesh_ops.h"

void bounding_polygon(struct triangulateio* mesh, struct triangulateio*  polygon);
void add_hole(struct triangulateio* mesh, struct triangulateio* bounding_polygon);
void mesh_Cat(struct triangulateio* dst, struct triangulateio* src);

