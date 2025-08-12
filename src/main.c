#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/game.h"
#include "../include/input.h"
#include "../include/renderer.h"

int main (){
  Game* game = game_create();
  if (!game) game_destroy(game, EXIT_FAILURE);
  
  Uint32 a;
  Uint32 b;
  
  Uint32 total = 0;
  Uint32 count = 0;

  Uint32 last_time = SDL_GetTicks();
  while(game->state){
    a = SDL_GetTicks();
    Uint32 current_time = SDL_GetTicks();
    game->delta_time = (current_time- last_time) / 1000.0f;
    last_time = current_time;
   
    input_poll(game);
    game_update(game);
    renderer_render(game->renderer, game->player, game->map);
    
    b=SDL_GetTicks();
    total = total + (b-a);
    count++;
    //usleep(16000);
  }
  printf("AVG time per frame, total time, number of frames: %i, %i, %i\n", total/count, total, count);
  game_destroy(game, EXIT_SUCCESS);
  return 0;
}
