#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "game.h"
#include "map.h"
#include "input.h"
#include "renderer.h"

int main (){
  Game* game = init_game();
  if (!game) cleanup(game, EXIT_FAILURE);

  while(game->state){  
    input_poll(game);
    update_game(game);
    render(game->renderer, game->player);
    usleep(16000);
  } 
  cleanup(game, EXIT_SUCCESS);
  return 0;
}
