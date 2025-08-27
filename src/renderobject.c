#include "../include/renderobject.h"


RenderObject* renderobject_create(SDL_Rect rect, double perp_dist, int texture_id){
  Renderobject* render_object = malloc(sizeof(RenderObject));
  if (!render_object){
    printf("Creation of RenderObject failed!");
    return NULL;
  }
 
  render_object->rect = rect;
  render_object->perp_dist = perp_dist;
  render_object->texture_id = texture_id;

  return render_object;
}

int compare_render_object(const void* a, const void* b){
  const RenderObject* obj_a = (const RenderEntity*)a;
  const RenderObject* obj_b = (const RenderEntity*)b;

  if (ent_a->distance > ent_b->distance) return -1;
  if (ent_a->distance < ent_b->distance) return 1;

  return 0;
}

void renderobject_update(RenderObect *render_object, SDL_Rect rect, double perp_dist, int texture_id){
  if (!render_object){
    printf("Invalid render_object pointer passed to renderobject_update\n");
    return;
  }
  
  render_object->rect = rect;
  render_object->perp_dist = perp_dist;
  render_object->texture_id = texture_id;
}

void renderobject_destroy(RenderObject* render_object){
  if (!render_object){
    printf("Invalid render_object pointer passed to renderobject_destroy\n");
    return;
  }

  free(render_object);
}
