#include "../include/renderobject.h"

int compare_render_object(const void* a, const void* b){
  const RenderObject* obj_a = (const RenderEntity*)a;
  const RenderObject* obj_b = (const RenderEntity*)b;

  if (ent_a->distance > ent_b->distance) return -1;
  if (ent_a->distance < ent_b->distance) return 1;

  return 0;
}
