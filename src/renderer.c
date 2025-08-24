#include "../include/renderer.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/texturemanager.h"

Renderer* renderer_create(){
  Renderer* renderer = malloc(sizeof(Renderer));
  if (!renderer) return NULL;
  
  renderer->scale_2d = 20;
  renderer->width = 400;
  renderer->height = 200;
  
  renderer->window = SDL_CreateWindow("SDL2 hello world", 100, 100, renderer->width, renderer->height, SDL_WINDOW_SHOWN);
  if(!renderer->window){
    printf("SDL_CreateWindow error: %s\n", SDL_GetError());
    return NULL;
  }

  renderer->sdl_renderer =  SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED);
  if(!renderer->sdl_renderer){
    printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
    return NULL;
  }

  renderer->texture_manager = texturemanager_create(5, renderer->sdl_renderer, 20, 20);
  if (!renderer->texture_manager){
    return NULL;
  }

  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture.jpeg");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture2.jpeg");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture3.png");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture4.jpeg");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture5.jpeg");
  
  return renderer;
}

void renderer_render_map_2d(Renderer *renderer, Map *map){
  if (!renderer || !map){
    printf("worng renderer or map parameter in render_map2d_Renderer");
    return;
  }

  SDL_Rect rect;
  for (size_t y = 0; y < map->height; y++){
    for (size_t x = 0; x < map->width; x++){
      int map_val = map_get_value(map, x, y);
      if (map_val){
        rect.x = x * renderer->texture_manager->texture_width;
        rect.y = y * renderer->texture_manager->texture_height;
        rect.w = renderer->texture_manager->texture_width;
        rect.h = renderer->texture_manager->texture_height;

        SDL_RenderCopy(renderer->sdl_renderer, texturemanager_get_texture(renderer->texture_manager, map_val), NULL, &rect);
      }
    }
  }
 
}

void renderer_render_player_2d(Renderer *renderer, Player *player){
  if (!renderer || !player){
    printf("Wrong player or renderer pointer parameter inside render_player_2d\n");
    return;
  }
  
  double offset = renderer->scale_2d/2 * player->player_size;

  SDL_SetRenderDrawColor(renderer->sdl_renderer, 0, 0, 255, 255); 
  player->rect.x = (player->pos.x) * renderer->scale_2d - offset;
  player->rect.y = (player->pos.y) * renderer->scale_2d - offset;
  player->rect.w = renderer->scale_2d * player->player_size;
  player->rect.h = renderer->scale_2d * player->player_size;
  SDL_RenderDrawRect(renderer->sdl_renderer, &player->rect); 
  int line_end_x = player->rect.x + player->dir.x * renderer->scale_2d + offset;
  int line_end_y = player->rect.y + player->dir.y * renderer->scale_2d + offset;
  int line_start_x = player->rect.x + offset;
  int line_start_y = player->rect.y + offset;
  SDL_RenderDrawLine(renderer->sdl_renderer, line_start_x, line_start_y, line_end_x, line_end_y);
}

void renderer_render(Renderer *renderer, Player *player, Map *map){
  SDL_SetRenderDrawColor(renderer->sdl_renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer->sdl_renderer);

  renderer_render_map_2d(renderer, map);
  renderer_render_player_2d(renderer, player);
  
  SDL_RenderPresent(renderer->sdl_renderer);
}

void renderer_destroy(Renderer *renderer){
  if(!renderer){
    return;
  }

  texturemanager_destroy(renderer->texture_manager);
  SDL_DestroyRenderer(renderer->sdl_renderer);
  SDL_DestroyWindow(renderer->window);
  free(renderer);
  renderer = NULL;
}
