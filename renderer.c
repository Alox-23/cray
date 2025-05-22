#include "renderer.h"

Renderer* init_renderer(){
  Renderer* renderer = malloc(sizeof(Renderer));
  if (!renderer) return NULL;

  renderer->window = SDL_CreateWindow("SDL2 hello world", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if(!renderer->window){
    printf("SDL_CreateWindow error: %s\n", SDL_GetError());
    return NULL;
  }

  renderer->sdl_renderer =  SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!renderer->sdl_renderer){
    printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
    return NULL;
  }

  return renderer;
}

void render(Renderer *renderer){
    SDL_SetRenderDrawColor(renderer->sdl_renderer, 255, 0, 0, 255);

    SDL_RenderClear(renderer->sdl_renderer);

    SDL_SetRenderDrawColor(renderer->sdl_renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer->sdl_renderer, 0, 0, 640, 480);

    SDL_RenderPresent(renderer->sdl_renderer);
}

void destroy_renderer(Renderer *renderer){
  SDL_DestroyWindow(renderer->window);
  SDL_DestroyRenderer(renderer->sdl_renderer);
  free(renderer);
  renderer = NULL;
}
