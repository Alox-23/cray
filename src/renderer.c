#include "../include/renderer.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/texturemanager.h"
#include "../include/renderobject.h"
#include "../include/renderqueue.h"
#include <stdlib.h>

Renderer* renderer_create(){
  Renderer* renderer = malloc(sizeof(Renderer));
  if (!renderer) return NULL;
  
  renderer->scale_2d = 20;
  renderer->width = 800;
  renderer->height = 500;
  
  renderer->window = SDL_CreateWindow("SDL2 hello world", 100, 100, renderer->width, renderer->height, SDL_WINDOW_SHOWN);
  if(!renderer->window){
    printf("SDL_CreateWindow error: %s\n", SDL_GetError());
    return NULL;
  }

  //renderer->sdl_renderer =  SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  renderer->sdl_renderer =  SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED);
  if(!renderer->sdl_renderer){
    printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
    return NULL;
  }

  renderer->texture_manager = texturemanager_create(5, renderer->sdl_renderer, 20, 20);
  if (!renderer->texture_manager){
    return NULL;
  }

  renderer->render_queue = renderqueue_create(2048);
  if (!renderer->render_queue){
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

void renderer_raycast(Renderer* renderer, Map *map, Player *player){
  if (!renderer || !map || !player){
    printf("Invalid pointer passed to renderer_raycast\n");
    return;
  } 

  //raycasting logic
  Vector2 ray_dir = {0, 0};
  Vector2 side_dist;
  Vector2 delta_dist;
  double perp_wall_dist = 0;
  RenderObject* obj;
  double camera_x;
  int map_x;
  int map_y;
  int step_x;
  int step_y;
  bool hit;
  int side;
  int line_height;
  double collision_x;
  double collision_y;
  int texture_id;
  double wall_x;
  int texture_x;
  for (int x = 0; x < renderer->width; x++){
    camera_x = 2 * x / (double)renderer->width - 1;
    ray_dir.x = player->dir.x + player->plane.x * camera_x;
    ray_dir.y = player->dir.y + player->plane.y * camera_x;

    map_x = (int)player->pos.x;
    map_y = (int)player->pos.y;

    delta_dist.x = (ray_dir.x == 0) ? 1e30 : fabs(1 / ray_dir.x);
    delta_dist.y = (ray_dir.y == 0) ? 1e30 : fabs(1 / ray_dir.y);

    hit = false;

    if (ray_dir.x < 0){
      step_x = -1;
      side_dist.x = (player->pos.x - map_x) * delta_dist.x;
    }
    else{
      step_x = 1;
      side_dist.x = (map_x + 1 - player->pos.x) * delta_dist.x;
    }
    if (ray_dir.y < 0){
      step_y = -1;
      side_dist.y = (player->pos.y - map_y) * delta_dist.y;
    }
    else{
      step_y = 1;
      side_dist.y = (map_y + 1 - player->pos.y) * delta_dist.y;
    }

    for (int i = 0; i < RENDER_DISTANCE; i++){
      if (side_dist.x < side_dist.y){
        side_dist.x += delta_dist.x;
        map_x += step_x;
        side = 0;
      }
      else{
        side_dist.y += delta_dist.y;
        map_y += step_y;
        side = 1;
      }

      if (map_get_value(map, map_x, map_y) > 0){
        hit = true;
        break;
      }
    }
    
    if (!hit) continue;
    
    if (side == 0){
      perp_wall_dist = side_dist.x - delta_dist.x;
    }
    else{
      perp_wall_dist = side_dist.y - delta_dist.y;
    }

    line_height = renderer->height / (perp_wall_dist + 0.00001);
  
    collision_x = player->pos.x + perp_wall_dist * ray_dir.x;
    collision_y = player->pos.y + perp_wall_dist * ray_dir.y;
    
    texture_id = map_get_value(map, map_x, map_y);

    wall_x; //where exacly on the tile did the ray hit relative to the tiles left-most value
    if (side == 0) wall_x = player->pos.y + perp_wall_dist * ray_dir.y;
    else wall_x = player->pos.x + perp_wall_dist * ray_dir.x;
    wall_x -= floor(wall_x);

    texture_x = (int)(wall_x * renderer->texture_manager->texture_width);
    if (side == 0 && ray_dir.x > 0) texture_x = renderer->texture_manager->texture_width - texture_x - 1;
    if (side == 1 && ray_dir.y < 0) texture_x = renderer->texture_manager->texture_width - texture_x - 1;

    obj = renderqueue_get_object(renderer->render_queue);
    if (!obj) break;

    obj->texture_id = texture_id;
    obj->dest_rect.x = x;
    obj->dest_rect.y = renderer->height / 2 - line_height / 2;
    obj->dest_rect.w = 1;
    obj->dest_rect.h = line_height;
    obj->src_rect.x = texture_x;
    obj->src_rect.y = 0;
    obj->src_rect.w = 1;
    obj->src_rect.h = renderer->texture_manager->texture_height;
    obj->perp_dist = perp_wall_dist;
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

void renderer_flush_queue(Renderer * renderer){
  if (!renderer){
    printf("Invalid renderer pointer inside of render_render_queue\n");
    return;
  }
  
  renderqueue_sort(renderer->render_queue);
 
  RenderObject* obj;
  for (int i = 0; i < renderer->render_queue->count; i++){
    obj = &renderer->render_queue->render_object_array[i];
    SDL_RenderCopy(renderer->sdl_renderer, texturemanager_get_texture(renderer->texture_manager, obj->texture_id), &obj->src_rect, &obj->dest_rect);
  }

  renderqueue_clear(renderer->render_queue);
}

void renderer_render(Renderer *renderer, Player *player, Map *map){
  SDL_SetRenderDrawColor(renderer->sdl_renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer->sdl_renderer);

  renderer_raycast(renderer, map, player);
  //renderer_render_map_2d(renderer, map);
  //renderer_render_player_2d(renderer, player);

  renderer_flush_queue(renderer);

  SDL_RenderPresent(renderer->sdl_renderer);
}

void renderer_destroy(Renderer *renderer){
  if(!renderer){
    return;
  }

  texturemanager_destroy(renderer->texture_manager);
  renderqueue_destroy(renderer->render_queue);
  SDL_DestroyRenderer(renderer->sdl_renderer);
  SDL_DestroyWindow(renderer->window);
  free(renderer);
  renderer = NULL;
}
