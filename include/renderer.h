#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "player.h"
#include "map.h"
#include "texturemanager.h"

typedef struct{
  double scale_2d;
  int width;
  int height;
  SDL_Renderer *sdl_renderer;
  SDL_Window *window; 
  TextureManager* texture_manager;
}Renderer;

Renderer* renderer_create();
void renderer_render_player_2d(Renderer *renderer, Player *player);
void renderer_render_map_2d(Renderer *renderer, Map *map);
void renderer_render(Renderer *renderer, Player *player, Map *map);
void renderer_destroy(Renderer *renderer);

#endif
