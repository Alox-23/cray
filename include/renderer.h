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

Renderer* init_Renderer();
void render_player_2d_Renderer(Renderer *renderer, Player *player);
void render_map_2d_Renderer(Renderer *renderer, Map *map);
void render_Renderer(Renderer *renderer, Player *player, Map *map);
void destroy_Renderer(Renderer *renderer);

#endif
