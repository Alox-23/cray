#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/game.h"
#include "../include/input.h"
#include "../include/renderer.h"

int main (){
  Game* game = init_game();
  if (!game) cleanup(game, EXIT_FAILURE);
  
  Uint32 last_time = SDL_GetTicks();
  while(game->state){
    Uint32 current_time = SDL_GetTicks();
    game->delta_time = (current_time- last_time) / 1000.0f;
    last_time = current_time;
   
    input_poll(game);
    update_game(game);
    render_Renderer(game->renderer, game->player, game->map);
    usleep(16000);
  }
  cleanup(game, EXIT_SUCCESS);
  return 0;
}
