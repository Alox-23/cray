#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "player.h"

typedef struct{
  SDL_Renderer *sdl_renderer;
  SDL_Window *window; 
}Renderer;

Renderer* init_renderer();
void render_player_2d(Renderer *renderer, Player *player);
void render(Renderer *renderer, Player *player);
void destroy_renderer(Renderer *renderer);

#endif
