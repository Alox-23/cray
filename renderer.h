#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct{
  SDL_Renderer *sdl_renderer;
  SDL_Window *window; 
}Renderer;

Renderer* init_renderer();
void render(Renderer *renderer);
void destroy_renderer(Renderer *renderer);

#endif
