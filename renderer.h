#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

struct Renderer {
  SDL_Renderer *renderer;
  SDL_Window *window; 
};

struct Renderer* init_renderer();
void render(struct Renderer *r);
void renderer_cleanup(struct Renderer *renderer);

#endif
