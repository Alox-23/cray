#include "input.h"
#include "player.h"

void input_poll(Game *game){
  SDL_Event event;
  while (SDL_PollEvent(&event)){
    if (event.type == SDL_QUIT) game->state = 0;

    player_handle_input(game->player, &event);
  }
}
