#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct {
  SDL_Rect rect;
  double perp_dist;
  int texture_id;
} RenderObject;

RenderObject* renderobject_create(SDL_Rect rect, double perp_dist, int texture_id);
void renderobject_destroy(RenderObject* render_object);

#endif
