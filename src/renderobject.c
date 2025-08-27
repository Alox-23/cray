#include "../include/renderobject.h"

int compare_render_object(const void* a, const void* b){
  const RenderObject* obj_a = (const RenderObject*)a;
  const RenderObject* obj_b = (const RenderObject*)b;

  if (obj_a->perp_dist > obj_b->perp_dist) return -1;
  if (obj_a->perp_dist < obj_b->perp_dist) return 1;

  return 0;
}
