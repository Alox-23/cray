#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "player.h"
#include "map.h"

typedef struct{
  double scale_2d;
  int width;
  int height;
  SDL_Renderer *sdl_renderer;
  SDL_Window *window; 
}Renderer;

Renderer* init_renderer();
void render_player_2d(Renderer *renderer, Player *player);
void render_map_2d(Renderer *renderer, Map *map);
void render(Renderer *renderer, Player *player, Map *map);
void destroy_renderer(Renderer *renderer);

#endif
