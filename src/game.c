#include "../include/game.h"
#include "../include/player.h"

Game* game_create(){
  Game* game = malloc(sizeof(Game));
  if (!game) return NULL;

  if (SDL_Init(SDL_INIT_EVERYTHING)) return NULL;  
 
  game->renderer = renderer_create();
  if (!game->renderer) return NULL;

  game->player = player_create();
  if (!game->player) return NULL;
  
  game->map = map_create(10, 10);
  if (!game->map) return NULL;

  game->keystate = SDL_GetKeyboardState(NULL);
  
  game->state = 1;
  game->delta_time = 1;

  return game;
}

void game_update(Game *game){
  player_update(game->player, game->delta_time);
}

void game_destroy(Game *game, int exit_status){
  if (!game){
    return;
  }

  renderer_destroy(game->renderer);
  player_destroy(game->player);
  map_destroy(game->map);
  SDL_Quit();
  free(game);
  game = NULL;
  exit(exit_status);
}
