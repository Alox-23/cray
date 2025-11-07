#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/game.h"
#include "../include/input.h"
#include "../include/renderer.h"

int main (){
  Game* game = game_create();
  if (!game) game_destroy(game, EXIT_FAILURE);
  
  Uint64 a;
  Uint64 b;
  
  double total = 0;
  Uint64 count = 0;

  Uint32 last_time = SDL_GetTicks();
  while(game->state){
    static Uint64 freq = 0;
    freq = SDL_GetPerformanceFrequency();
    a = SDL_GetPerformanceCounter();
    Uint32 current_time = SDL_GetTicks();
    game->delta_time = (current_time- last_time) / 1000.0f;
    last_time = current_time;
   
    Uint64 time_update_s = SDL_GetPerformanceCounter();
    input_poll(game);
    game_update(game);
    Uint64 time_update_e = SDL_GetPerformanceCounter();
    double time_update_f = (double)(time_update_e - time_update_s) /freq * 1000.0f;
    printf("UPDATE: %.3fms\n", time_update_f);


    Uint64 time_r_s = SDL_GetPerformanceCounter();
    renderer_render(game->renderer, game->player, game->map);
    Uint64 time_r_e = SDL_GetPerformanceCounter();
    double time_r_f = (double)(time_r_e - time_r_s) / freq * 1000.0f;
    printf("RENDER: %.3fms\n", time_r_f);
    
    b=SDL_GetPerformanceCounter();
    double frame_time = (double)(b-a) / freq * 1000;
    total = total + frame_time;
    count++;
  }
  printf("TIME(ms): %.3f\nRTIME(s): %.1f\nFPS     : %.0f\n", total/count, total / 1000, 1.0f / (total/1000/count));
  game_destroy(game, EXIT_SUCCESS);
  return 0;
}
