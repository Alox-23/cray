#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "player.h"
#include "map.h"
#include "texturemanager.h"
#include "renderqueue.h"
#include "renderobject.h"

#define RENDER_DISTANCE 100

typedef struct{
  float m;
  float c;
  float mxb;
  float mnb;
}FogSetting;

typedef struct{
  double scale_2d;
  int width;
  int height;
  int raycasting_scale;
  float floor_tile_scale;
  SDL_Surface* floor_surface;
  SDL_Texture* background_texture;
  SDL_Renderer *sdl_renderer;
  SDL_Window *window;
  RenderQueue* render_queue;
  TextureManager* texture_manager;
}Renderer;

Renderer* renderer_create();
void renderer_render_texture_atlas(Renderer* renderer);
float renderer_calc_fog_brightness(FogSetting s, double var);
void renderer_render_player_2d(Renderer *renderer, Player *player);
void renderer_render_map_2d(Renderer *renderer, Map *map);
void renderer_raycast(Renderer* renderer, Map *map, Player *player);
void renderer_floorcast(Renderer* renderer, Map *map, Player *player);
void renderer_flush_queue(Renderer* renderer);
void renderer_render(Renderer *renderer, Player *player, Map *map);
void renderer_destroy(Renderer *renderer);

#endif
