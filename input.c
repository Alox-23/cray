#include "input.h"
#include "player.h"

void input_poll(Game *game){
  SDL_Event event;
  while (SDL_PollEvent(&event)){
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) game->state = 0;
  }
  player_handle_input(game->player, game->keystate, game->delta_time);
}
