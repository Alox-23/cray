#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <immintrin.h>
#include "player.h"
#include "map.h"
#include "texturemanager.h"
#include "renderqueue.h"
#include "renderobject.h"

#define RENDER_DISTANCE 300
#define FLOOR_THREADS 1 //game does not wait for threads to finish so more threads wont help 0 to disable multithreading for floorcasting

typedef struct{
  float m;
  float c;
  float mxb;
  float mnb;
}FogSetting;

typedef struct{
  SDL_Surface* floor_surface;
  SDL_Texture* texture_buffer;
  Uint32* texture_pixels;
  int texture_pitch;

  int id;

  float player_pos_z;
  float player_pos_x;
  float player_pos_y;
  float player_plane_x;
  float player_plane_y;
  float player_dir_x;
  float player_dir_y;

  Map* map_p;

  int width, height, start_y, end_y;

  SDL_atomic_t should_exit;
  SDL_atomic_t work_complete;
  SDL_sem* work_semaphore;
}FloorCastingThreadData;

typedef struct{
  float scale_2d;
  int width;
  int height;
  int raycasting_scale;

#if FLOOR_THREADS != 0
  FloorCastingThreadData floor_thread_data[FLOOR_THREADS];
  SDL_Thread* sdl_floor_threads[FLOOR_THREADS];
#else 
  FloorCastingThreadData floor_thread_data[1];
  SDL_Thread* sdl_floor_threads[1];
#endif
  SDL_Texture* background_texture;
  SDL_Surface* floor_surface;
  SDL_Renderer *sdl_renderer;
  SDL_Window *window;

  RenderQueue* render_queue;
  TextureManager* texture_manager;
}Renderer;

Renderer* renderer_create(Map *map);
void renderer_render_texture_atlas(Renderer* renderer);
float renderer_calc_fog_brightness(FogSetting s, double var);
void renderer_render_player_2d(Renderer *renderer, Player *player);
void renderer_render_map_2d(Renderer *renderer, Map *map);
void renderer_raycast(Renderer* renderer, Map *map, Player *player);
void renderer_render_floorcast_buffer(Renderer* renderer);
int renderer_create_floor_thread_data(Renderer* renderer, Map* map);
int renderer_floorcast_fixed_thread(void* data);
int renderer_floorcast_fixed_thread_h(void* data);
void renderer_sync_floorcast_thread_data(Renderer* renderer, Map* map, Player* player);
void renderer_thread_cleanup(Renderer* renderer);
void renderer_flush_queue(Renderer* renderer);
void renderer_render(Renderer *renderer, Player *player, Map *map);
void renderer_destroy(Renderer *renderer);

#endif
