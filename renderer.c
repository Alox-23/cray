#include "renderer.h"

struct Renderer* init_renderer(){
  struct Renderer* renderer = malloc(sizeof(struct Renderer));
  if (!renderer) return NULL;

  renderer->window = SDL_CreateWindow("SDL2 hello world", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if(!renderer->window){
    printf("SDL_CreateWindow error: %s\n", SDL_GetError());
    return NULL;
  }

  renderer->renderer =  SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!renderer->renderer){
    printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
    return NULL;
  }

  return renderer;
}

void render(struct Renderer *r){
    SDL_SetRenderDrawColor(r->renderer, 255, 0, 0, 255);

    SDL_RenderClear(r->renderer);

    SDL_RenderDrawLine(r->renderer, 0, 0, 640, 480);

    SDL_RenderPresent(r->renderer);
}

void renderer_cleanup(struct Renderer *renderer){
  SDL_DestroyWindow(renderer->window);
  SDL_DestroyRenderer(renderer->renderer);
  free(renderer);
}
