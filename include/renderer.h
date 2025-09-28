#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "player.h"
#include "map.h"
#include "texturemanager.h"
#include "renderqueue.h"
#include "renderobject.h"

#define RENDER_DISTANCE 100

typedef struct{
  double scale_2d;
  int width;
  int height;
  int raycasting_scale;
  SDL_Renderer *sdl_renderer;
  SDL_Window *window;
  RenderQueue* render_queue;
  TextureManager* texture_manager;
}Renderer;

Renderer* renderer_create();
void renderer_render_texture_atlas(Renderer* renderer);
void renderer_render_player_2d(Renderer *renderer, Player *player);
void renderer_render_map_2d(Renderer *renderer, Map *map);
void renderer_raycast(Renderer* renderer, Map *map, Player *player);
void renderer_flush_queue(Renderer* renderer);
void renderer_render(Renderer *renderer, Player *player, Map *map);
void renderer_destroy(Renderer *renderer);

#endif
