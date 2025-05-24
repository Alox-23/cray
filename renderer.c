#include "renderer.h"
#include "player.h"

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

void render_player_2d(Renderer *renderer, Player *player){
  if (!renderer || !player){
    printf("Wrong player or renderer pointer parameter inside render_player_2d\n");
    return;
  }
 
  SDL_SetRenderDrawColor(renderer->sdl_renderer, 0, 0, 255, 255);
  SDL_Rect rect;
  rect.x = player->pos.x -5;
  rect.y = player->pos.y -5;
  rect.w = 10;
  rect.h = 10;
  SDL_RenderDrawRect(renderer->sdl_renderer, &rect); 
  
  int scaler = 10;
  SDL_RenderDrawLine(renderer->sdl_renderer, player->pos.x, player->pos.y, player->pos.x + player->dir.x * scaler, player->pos.y + player->dir.y * scaler);
}

void render(Renderer *renderer, Player *player){
  SDL_SetRenderDrawColor(renderer->sdl_renderer, 255, 0, 0, 255);

  SDL_RenderClear(renderer->sdl_renderer);

  render_player_2d(renderer, player);

  SDL_RenderPresent(renderer->sdl_renderer);
}

void destroy_renderer(Renderer *renderer){
  SDL_DestroyWindow(renderer->window);
  SDL_DestroyRenderer(renderer->sdl_renderer);
  free(renderer);
  renderer = NULL;
}
