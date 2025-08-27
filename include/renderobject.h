#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct {
  SDL_Rect dest_rect;
  SDL_Rect src_rect;
  double perp_dist;
  int texture_id;
} RenderObject;

int compare_render_object(const void* a, const void* b);

#endif
