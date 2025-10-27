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
    a = SDL_GetPerformanceCounter();
    Uint32 current_time = SDL_GetTicks();
    game->delta_time = (current_time- last_time) / 1000.0f;
    last_time = current_time;
   
    input_poll(game);
    game_update(game);
    renderer_render(game->renderer, game->player, game->map);

    b=SDL_GetPerformanceCounter();
    static Uint64 freq = 0;
    freq = SDL_GetPerformanceFrequency();
    double frame_time = (double)(b-a) / freq * 1000;
    total = total + frame_time;
    count++;
  }
  printf("TIME(ms): %.3f\nRTIME(s): %.1f\nFPS     : %.0f\n", total/count, total / 1000, 1.0f / (total/1000/count));
  game_destroy(game, EXIT_SUCCESS);
  return 0;
}
