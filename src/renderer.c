#include "../include/renderer.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/texturemanager.h"
#include "../include/renderobject.h"
#include "../include/renderqueue.h"
#include <stdlib.h>

Renderer* renderer_create(Map *map){
  if (!map) return NULL;
  Renderer* renderer = malloc(sizeof(Renderer));
  if (!renderer) return NULL;
  
  renderer->scale_2d = 8;
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

  renderer->texture_manager = texturemanager_create(renderer->sdl_renderer, 32, 32);
  if (!renderer->texture_manager){
    return NULL;
  }

  renderer->render_queue = renderqueue_create(16384);
  if (!renderer->render_queue){
    return NULL;
  }

  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture.jpeg");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture2.jpeg");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture3.png");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture4.jpeg");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/texture5.jpeg");

  /*
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/default.png");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/default.png");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/default.png");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/default.png");
  texturemanager_add_texture(renderer->texture_manager, renderer->sdl_renderer, "assets/default.png");
  */ 

  if (!renderer_create_floor_thread_data(renderer, map)){
    printf("Failed to create floor thread data\n");
    return NULL;
  }

  return renderer;
}

void renderer_destroy(Renderer *renderer){
  if(!renderer){
    return;
  }

  #if FLOOR_THREADS != 0
  renderer_thread_cleanup(renderer);
  #endif
  SDL_DestroyTexture(renderer->background_texture);
  SDL_UnlockSurface(renderer->floor_surface);
  SDL_FreeSurface(renderer->floor_surface);
  texturemanager_destroy(renderer->texture_manager);
  renderqueue_destroy(renderer->render_queue);
  SDL_DestroyRenderer(renderer->sdl_renderer);
  SDL_DestroyWindow(renderer->window);
  free(renderer);
  renderer = NULL;
}

void renderer_render(Renderer *renderer, Player *player, Map *map){
  SDL_SetRenderDrawColor(renderer->sdl_renderer, 150, 150, 220, 255);
  SDL_RenderClear(renderer->sdl_renderer);

  Uint64 a1 = SDL_GetPerformanceCounter();
  renderer_sync_floorcast_thread_data(renderer, map, player);
  Uint64 b1 = SDL_GetPerformanceCounter();
  
  Uint64 a3 = SDL_GetPerformanceCounter();
  //renderer_raycast(renderer, map, player);
  Uint64 b3 = SDL_GetPerformanceCounter();

  Uint64 a2 = SDL_GetPerformanceCounter();
  renderer_render_floorcast_buffer(renderer);
  Uint64 b2 = SDL_GetPerformanceCounter();
  
  double t1 = (double)(b1-a1) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
  double t2 = (double)(b2-a2) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
  double t3 = (double)(b3-a3) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

  //printf("Time for SYNC: %.3fms\n", t1);
  //printf("Time for REND: %.3fms\n", t2);
  //printf("Time for RAYC: %.3fms\n", t3);
  
  //renderer_flush_queue(renderer);
 
  //renderer_render_map_2d(renderer, map);
  //renderer_render_player_2d(renderer, player);

  SDL_RenderPresent(renderer->sdl_renderer);

}

void renderer_thread_cleanup(Renderer* renderer){
  if (!renderer){
    printf("invalid renderer passes to thread cleanup - cleanup not done\n");
    return;
  }

  for (int i = 0; i < FLOOR_THREADS; i++){
    SDL_AtomicSet(&renderer->floor_thread_data[i].should_exit, 1);
    SDL_SemPost(renderer->floor_thread_data[i].work_semaphore);
  }
  
  for (int i = 0; i < FLOOR_THREADS; i++) {
    SDL_WaitThread(renderer->sdl_floor_threads[i], NULL);
    renderer->sdl_floor_threads[i] = NULL;
  }
  
  for (int i = 0; i < FLOOR_THREADS; i++){
    SDL_DestroySemaphore(renderer->floor_thread_data[i].work_semaphore);
    renderer->floor_thread_data[i].work_semaphore = NULL;
  }
}

int renderer_create_floor_thread_data(Renderer* renderer, Map* map){
  renderer->background_texture = SDL_CreateTexture(renderer->sdl_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, renderer->width, renderer->height);
  if (!renderer->background_texture){
    printf("Failed to create background_texture SDL_Texture: %s\n", SDL_GetError());
    return 0;
  }

  SDL_Surface* csurface = IMG_Load("assets/x256/textures/Metal/Metal_01-256x256.png");
  renderer->floor_surface = SDL_ConvertSurfaceFormat(csurface, SDL_PIXELFORMAT_RGBA32, 0);
  SDL_FreeSurface(csurface);
  if (!renderer->floor_surface){
    printf("Failed to load floor SDL_Surface: %s\n", SDL_GetError());
    return 0;
  }

  if (SDL_LockSurface(renderer->floor_surface)){
    printf("Failed to lock renderer->floor_surface: %s\n", SDL_GetError());
    return 0;
  }

  int num_threads = FLOOR_THREADS;
  #if FLOOR_THREADS == 0 
  num_threads = 1;
  #endif

  int floor_start = renderer->height / 2;
  int floor_height = renderer->height/2; //the actual height of the block of pixel xy values proccesed
  int rows_per_thread = floor_height / num_threads;
 
  for (int i = 0; i < num_threads; i++){
    renderer->floor_thread_data[i].floor_surface = renderer->floor_surface;
    renderer->floor_thread_data[i].width = renderer->width;
    renderer->floor_thread_data[i].height = renderer->height;
    renderer->floor_thread_data[i].start_y = floor_start + i * rows_per_thread;
    renderer->floor_thread_data[i].end_y = floor_start + (i+1) * rows_per_thread;
    renderer->floor_thread_data[i].id = i;

    printf("FloorThread with id:%d works on rows %d-%d!\n", i, renderer->floor_thread_data[i].start_y, renderer->floor_thread_data[i].end_y);
  
    renderer->floor_thread_data[i].texture_buffer = renderer->background_texture; 
    renderer->floor_thread_data[i].map_p = map;

    #if FLOOR_THREADS != 0
    renderer->floor_thread_data[i].work_semaphore = SDL_CreateSemaphore(0);
    SDL_AtomicSet(&renderer->floor_thread_data[i].should_exit, 0);
    SDL_AtomicSet(&renderer->floor_thread_data[i].work_complete, 1);
 
    char thread_name[32];
    snprintf(thread_name, sizeof(thread_name), "FloorThread%d", i);
    
    renderer->sdl_floor_threads[i] = SDL_CreateThread(renderer_floorcast_fixed_thread_h, thread_name, &renderer->floor_thread_data[i]);
    #endif
  }
  
  return 1;
}

void renderer_sync_floorcast_thread_data(Renderer* renderer, Map* map, Player *player){
  if (!renderer || !map || !player){
    printf("Incoirect pointer parameters in render_sync_floorcast_thread_data()\n");
    return;
  }

  Uint32* texture_pixels;
  int texture_pitch;
  if (SDL_LockTexture(renderer->background_texture, NULL, (void**)&texture_pixels, &texture_pitch) != 0){
    printf("Failed to lock texture, skipping frame SDL_Error: %s\n", SDL_GetError());
    return;
  }

  int num_threads = FLOOR_THREADS;
  #if FLOOR_THREADS == 0 
  num_threads = 1;
  #endif

  for (int i = 0; i < num_threads; i++){
    renderer->floor_thread_data[i].player_pos_z = player->pos_z;
    renderer->floor_thread_data[i].player_pos_y = player->pos.y;
    renderer->floor_thread_data[i].player_pos_x = player->pos.x;
      
    renderer->floor_thread_data[i].player_plane_x = player->plane.x;
    renderer->floor_thread_data[i].player_plane_y = player->plane.y;
      
    renderer->floor_thread_data[i].player_dir_x = player->dir.x;
    renderer->floor_thread_data[i].player_dir_y = player->dir.y;

    renderer->floor_thread_data[i].texture_pixels = texture_pixels;
    renderer->floor_thread_data[i].texture_pitch = texture_pitch;
  
    renderer->floor_thread_data[i].map_p = map;

    if (!renderer->floor_thread_data[i].map_p){
      printf("Invalid map pointer passed to floorcasting thread during syncing\n");
      return;
    }

    #if FLOOR_THREADS != 0
    SDL_AtomicSet(&renderer->floor_thread_data[i].work_complete, 0);
    SDL_SemPost(renderer->floor_thread_data[i].work_semaphore);
    #endif
  }

  #if FLOOR_THREADS == 0 
  renderer_floorcast_fixed_thread(&renderer->floor_thread_data[0]);
  #endif
}


void renderer_render_floorcast_buffer(Renderer* renderer){
  if (!renderer){
    printf("Incorrect renderer pointer to render_render_florcast_buffer\n");
  }
  
  SDL_UnlockTexture(renderer->background_texture);
  SDL_RenderCopy(renderer->sdl_renderer, renderer->background_texture, NULL, NULL);
}

void renderer_flush_queue(Renderer* renderer) {
  if (!renderer || !renderer->texture_manager) return;
  
  SDL_Texture* atlas = texturemanager_get_atlas(renderer->texture_manager);
  if (!atlas) return;
 
  renderqueue_sort(renderer->render_queue);

  SDL_Rect final_src_rect;
  RenderObject* entity;
  for (int i = 0; i < renderer->render_queue->count; i++) {
    entity = &renderer->render_queue->render_object_array[i];

    FogSetting r_settings = {0.01, 1, 1, 0.3};
    FogSetting g_settings = {0.01, 1, 1, 0.3};
    FogSetting b_settings = {0.01, 1, 1, 0.3};
    /*
    float time = SDL_GetTicks() * 0.001f;
    FogSetting r_settings = {0.5 + 0.3*sinf(time), 2, 0.9, 0.1};
    FogSetting g_settings = {0.5 + 0.3*sinf(time + 2.0f), 3, 0.9, 0.1};  
    FogSetting b_settings = {0.5 + 0.3*sinf(time + 4.0f), 4, 0.9, 0.1};
    */
    float rb = renderer_calc_fog_brightness(r_settings, entity->perp_dist);
    float gb = renderer_calc_fog_brightness(g_settings, entity->perp_dist);
    float bb = renderer_calc_fog_brightness(b_settings, entity->perp_dist);
    SDL_Color final_color = {
      .r = 255 * rb,
      .g = 255 * gb,
      .b = 255 * bb, 
      .a = 255
    };

    SDL_SetTextureColorMod(atlas, final_color.r, final_color.g, final_color.b);

    final_src_rect = texturemanager_get_texcoord(renderer->texture_manager, entity->texture_id);
    final_src_rect.x += entity->src_rect.x;
    final_src_rect.y += entity->src_rect.y;
    final_src_rect.w = entity->src_rect.w;
    final_src_rect.h = entity->src_rect.h;
    
    SDL_RenderCopy(renderer->sdl_renderer, atlas, &final_src_rect, &entity->dest_rect);
  }
  renderqueue_clear(renderer->render_queue);
}

float renderer_calc_fog_brightness(FogSetting s, double var){
  float fb = s.c / (1 + var*s.m);
  if (fb > s.mxb) return s.mxb;
  if (fb < s.mnb) return s.mnb;
  return fb;
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

void renderer_render_map_2d(Renderer *renderer, Map *map){
  if (!renderer || !map){
    printf("worng renderer or map parameter in render_map2d_Renderer");
    return;
  }

  SDL_Rect rect;
  SDL_Rect src;
  for (size_t y = 0; y < map->height; y++){
    for (size_t x = 0; x < map->width; x++){
      int map_val = MAP_GET_VALUE(map->buffer, 0, x, y);
      if (map_val){
        rect.x = x * renderer->scale_2d;
        rect.y = y * renderer->scale_2d;
        rect.w = renderer->scale_2d;
        rect.h = renderer->scale_2d;
        
        src = texturemanager_get_texcoord(renderer->texture_manager, map_val-1);

        SDL_RenderCopy(renderer->sdl_renderer, texturemanager_get_atlas(renderer->texture_manager), &src, &rect);
      }
    }
  }
}

void renderer_render_texture_atlas(Renderer* renderer){
  if (!renderer){
    return;
  }

  SDL_Rect rect = {
    .x = renderer->width - 300,
    .y = renderer->height - 300,
    .w = 300,
    .h = 300,
  };

  SDL_RenderCopy(renderer->sdl_renderer, texturemanager_get_atlas(renderer->texture_manager), NULL, &rect);
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
    for (int z_level = 0; z_level < map->depth; z_level++){
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

        if (MAP_GET_VALUE(map->buffer, z_level, map_x, map_y) > 0){
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
      
      texture_id = MAP_GET_VALUE(map->buffer, z_level, map_x, map_y)-1;

      wall_x; //where exacly on the tile did the ray hit relative to the tiles left-most value
      if (side == 0) wall_x = player->pos.y + perp_wall_dist * ray_dir.y;
      else wall_x = player->pos.x + perp_wall_dist * ray_dir.x;
      wall_x -= floor(wall_x);

      texture_x = (int)(wall_x * renderer->texture_manager->texture_width);
      if (side == 0 && ray_dir.x > 0) texture_x = renderer->texture_manager->texture_width - texture_x - 1;
      if (side == 1 && ray_dir.y < 0) texture_x = renderer->texture_manager->texture_width - texture_x - 1;
      obj = renderqueue_get_object(renderer->render_queue);
      if (!obj) break;

      double vertical_offset = renderer->height * (z_level - player->pos_z + 0.5f) / (perp_wall_dist + 0.00001) - z_level;
      
      obj->texture_id = texture_id;
      obj->alpha_value = 1;
      obj->dest_rect.x = x;
      obj->dest_rect.y = (renderer->height - line_height) / 2 - vertical_offset;
      obj->dest_rect.w = 1;
      obj->dest_rect.h = line_height;
      obj->src_rect.x = texture_x;
      obj->src_rect.y = 0;
      obj->src_rect.w = 1;
      obj->src_rect.h = renderer->texture_manager->texture_height;
      obj->perp_dist = perp_wall_dist;
    }
  }
}

int renderer_floorcast_fixed_thread(void *data) {
  FloorCastingThreadData* thread_data = (FloorCastingThreadData*)data;

  // Fixed-point precision (16.16)
  #define FIXED_SHIFT 16
  #define FIXED_SCALE (1 << FIXED_SHIFT)
  #define FLOAT_TO_FIXED(f) ((int)((f) * FIXED_SCALE))
  #define FIXED_MUL(a, b) (((int64_t)(a) * (b)) >> FIXED_SHIFT)
  #define FIXED_TO_INT(f) ((f) >> FIXED_SHIFT)
  #define FIXED_FRAC(f) ((f) & (FIXED_SCALE - 1))

  printf("FloorThread with id:%d, has Launched!\n", thread_data->id);
 
  #if FLOOR_THREADS != 0
  while (!SDL_AtomicGet(&thread_data->should_exit)){
    SDL_SemWait(thread_data->work_semaphore);
    
    if (SDL_AtomicGet(&thread_data->should_exit)) break;
  #endif
    Uint32* texture_pixels = thread_data->texture_pixels;
    int texture_pitch = thread_data->texture_pitch; 

    const int pixels_per_row = texture_pitch / sizeof(Uint32);
    
    // Precompute floating-point values first for accuracy
    const float ray_dir_x0 = thread_data->player_dir_x - thread_data->player_plane_x;
    const float ray_dir_y0 = thread_data->player_dir_y - thread_data->player_plane_y;
    const float ray_dir_x1 = thread_data->player_dir_x + thread_data->player_plane_x;
    const float ray_dir_y1 = thread_data->player_dir_y + thread_data->player_plane_y;
    const float ray_diff_x = ray_dir_x1 - ray_dir_x0;
    const float ray_diff_y = ray_dir_y1 - ray_dir_y0;
    
    // Convert to fixed-point
    const int pos_x = FLOAT_TO_FIXED(thread_data->player_pos_x);
    const int pos_y = FLOAT_TO_FIXED(thread_data->player_pos_y);
    const int fixed_ray_dir_x0 = FLOAT_TO_FIXED(ray_dir_x0);
    const int fixed_ray_dir_y0 = FLOAT_TO_FIXED(ray_dir_y0);
    const int fixed_ray_diff_x = FLOAT_TO_FIXED(ray_diff_x);
    const int fixed_ray_diff_y = FLOAT_TO_FIXED(ray_diff_y);
    
    int pos_z_scaled = FLOAT_TO_FIXED(thread_data->player_pos_z * thread_data->height);
    const int fixed_inv_width = FIXED_SCALE / thread_data->width;  // Note: This assumes width <= 65536

    // Texture info
    const int tex_width = thread_data->floor_surface->w;
    const int tex_height = thread_data->floor_surface->h;
    const int tex_pitch = thread_data->floor_surface->pitch / 4;
    const Uint32* tex_pixels = (Uint32*)thread_data->floor_surface->pixels;
    const int tex_width_mask = tex_width - 1;
    const int tex_height_mask = tex_height - 1;

    for (int y = thread_data->start_y; y < thread_data->end_y; y++) {
      int p = y - thread_data->height / 2;

      if (p == 0) p=1;
      
      // FIXED: Use proper fixed-point division (or avoid it)
      // Since p is small, we can use reciprocal multiplicatioon
      const int row_distance = FIXED_MUL(pos_z_scaled, FIXED_SCALE / p);
      
      // FIXED: Correct fixed-point multiplication chain
      const int floor_step_x = FIXED_MUL(FIXED_MUL(row_distance, fixed_ray_diff_x), fixed_inv_width);
      const int floor_step_y = FIXED_MUL(FIXED_MUL(row_distance, fixed_ray_diff_y), fixed_inv_width);
      
      // FIXED: Correct position calculation
      int floor_x = pos_x + FIXED_MUL(row_distance, fixed_ray_dir_x0);
      int floor_y = pos_y + FIXED_MUL(row_distance, fixed_ray_dir_y0);

      Uint32* dest_row = texture_pixels + y * pixels_per_row;
      
      for (int x = 0; x < thread_data->width; x++) {
        // Extract fractional parts
        const int frac_x = FIXED_FRAC(floor_x);
        const int frac_y = FIXED_FRAC(floor_y);
        
        // Convert to texture coordinates (0 to tex_width-1)
        const int texture_x = FIXED_MUL(frac_x, tex_width);
        const int texture_y = FIXED_MUL(frac_y, tex_height);

        //map coordinates
        int map_grid_x = FIXED_TO_INT(floor_x);
        int map_grid_y = FIXED_TO_INT(floor_y);

        dest_row[x] = tex_pixels[(texture_y & tex_height_mask) * tex_width + (texture_x & tex_width_mask)];

        floor_x += floor_step_x;
        floor_y += floor_step_y;
      }
    }
  #if FLOOR_THREADS != 0
    SDL_AtomicSet(&thread_data->work_complete, 1);
  }
  printf("Thread%d, has exited!\n", thread_data->id);
  #endif

  #undef FIXED_SHIFT
  #undef FIXED_SCALE
  #undef FLOAT_TO_FIXED
  #undef FIXED_MUL
  #undef FIXED_TO_INT
  #undef FIXED_FRAC
}

int renderer_floorcast_fixed_thread_h(void *data) {
  FloorCastingThreadData* thread_data = (FloorCastingThreadData*)data;

  // Fixed-point precision (16.16)
  #define FIXED_SHIFT 16
  #define FIXED_SCALE (1 << FIXED_SHIFT)
  #define FLOAT_TO_FIXED(f) ((int)((f) * FIXED_SCALE))
  #define FIXED_MUL(a, b) (((int64_t)(a) * (b)) >> FIXED_SHIFT)
  #define FIXED_TO_INT(f) ((f) >> FIXED_SHIFT)
  #define FIXED_FRAC(f) ((f) & (FIXED_SCALE - 1))

  printf("FloorThread with id:%d, has Launched!\n", thread_data->id);
 
  #if FLOOR_THREADS != 0
  while (!SDL_AtomicGet(&thread_data->should_exit)){
    SDL_SemWait(thread_data->work_semaphore);
    
    if (SDL_AtomicGet(&thread_data->should_exit)) break;
  #endif
    
    const int pixels_per_row = thread_data->texture_pitch / sizeof(Uint32);
    
    const int tex_width = thread_data->floor_surface->w;
    const int tex_height = thread_data->floor_surface->h;
    const int tex_pitch = thread_data->floor_surface->pitch / 4;
    const Uint32* tex_pixels = (Uint32*)thread_data->floor_surface->pixels;
    const int tex_width_mask = tex_width - 1;
    const int tex_height_mask = tex_height - 1;
    
    float ray_dir_x0;
    float ray_dir_y0;
    float ray_dir_x1;
    float ray_dir_y1;
    int p;
    float base_pos_z;
    float row_distance;
    float floor_step_x;
    float floor_step_y;
    float floor_x;
    float floor_y;
    int texture_x;
    int texture_y;
    
    for (int y = thread_data->start_y; y < thread_data->end_y; y++){
      ray_dir_x0 = thread_data->player_dir_x - thread_data->player_plane_x;
      ray_dir_y0 = thread_data->player_dir_y - thread_data->player_plane_y;
      ray_dir_x1 = thread_data->player_dir_x + thread_data->player_plane_x;
      ray_dir_y1 = thread_data->player_dir_y + thread_data->player_plane_y;

      p = y - thread_data->height / 2;
     
      // Base pos_z value (player height)
      base_pos_z = thread_data->player_pos_z * thread_data->height;

      Uint32* dest_row = thread_data->texture_pixels + y * pixels_per_row;

      for (int x = 0; x < thread_data->width; x++){
        // Calculate the current floor position for this pixel
        float t = (float)x / thread_data->width;
        float current_ray_dir_x = ray_dir_x0 + t * (ray_dir_x1 - ray_dir_x0);
        float current_ray_dir_y = ray_dir_y0 + t * (ray_dir_y1 - ray_dir_y0);
        
        // Get the cell coordinates for height lookup
        int cell_x = (int)floor_x;
        int cell_y = (int)floor_y;
        
        // Get the height at this cell from the height map

        float cell_height = 0;
        if (cell_x > 2 && cell_y > 2){
          float cell_height = -2;
          // Adjust pos_z based on cell height
          float adjusted_pos_z = base_pos_z + cell_height * thread_data->height;
          
          // Recalculate row_distance with adjusted height
          row_distance = adjusted_pos_z / p;
          
          // Recalculate floor position with adjusted row_distance
          floor_x = thread_data->player_pos_x + row_distance * current_ray_dir_x;
          floor_y = thread_data->player_pos_y + row_distance * current_ray_dir_y;
          
          // Calculate texture coordinates
          texture_x = (int)(tex_width * (floor_x - (int)floor_x));
          texture_y = (int)(tex_height * (floor_y - (int)floor_y));
          
          // Ensure texture coordinates are within bounds
          texture_x = texture_x & tex_width_mask;
          texture_y = texture_y & tex_height_mask;
          
          Uint32 color = tex_pixels[100];
          
          dest_row[x] = color; 
        }
        else{
          // Adjust pos_z based on cell height
          float adjusted_pos_z = base_pos_z + cell_height * thread_data->height;
          
          // Recalculate row_distance with adjusted height
          row_distance = adjusted_pos_z / p;
          
          // Recalculate floor position with adjusted row_distance
          floor_x = thread_data->player_pos_x + row_distance * current_ray_dir_x;
          floor_y = thread_data->player_pos_y + row_distance * current_ray_dir_y;
          
          // Calculate texture coordinates
          texture_x = (int)(tex_width * (floor_x - (int)floor_x));
          texture_y = (int)(tex_height * (floor_y - (int)floor_y));
          
          // Ensure texture coordinates are within bounds
          texture_x = texture_x & tex_width_mask;
          texture_y = texture_y & tex_height_mask;
          
          Uint32 color = tex_pixels[texture_y * tex_width + texture_x];
          
          dest_row[x] = color; 
        }  
      }
      
      // Update floor_x and floor_y for the next row using the original calculation
      // This maintains the original algorithm's progression
      row_distance = base_pos_z / p;
      floor_step_x = row_distance * (ray_dir_x1 - ray_dir_x0) / thread_data->width;
      floor_step_y = row_distance * (ray_dir_y1 - ray_dir_y0) / thread_data->width;
      
      if (y == thread_data->start_y) {
        // Initialize floor position for the first row
        floor_x = thread_data->player_pos_x + row_distance * ray_dir_x0;
        floor_y = thread_data->player_pos_y + row_distance * ray_dir_y0;
      } else {
        // Update floor position for next row
        floor_x += floor_step_x * thread_data->width;
        floor_y += floor_step_y * thread_data->width;
      }
    }
  #if FLOOR_THREADS != 0
    SDL_AtomicSet(&thread_data->work_complete, 1);
  }
  printf("Thread%d, has exited!\n", thread_data->id);
  #endif

  #undef FIXED_SHIFT
  #undef FIXED_SCALE
  #undef FLOAT_TO_FIXED
  #undef FIXED_MUL
  #undef FIXED_TO_INT
  #undef FIXED_FRAC
}
