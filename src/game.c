#include "../include/game.h"
#include "../include/player.h"

Game* init_game(){
  Game* game = malloc(sizeof(Game));
  if (!game) return NULL;

  if (SDL_Init(SDL_INIT_EVERYTHING)) return NULL;  
 
  game->renderer = init_Renderer();
  if (!game->renderer) return NULL;

  game->player = init_player();
  if (!game->player) return NULL;
  
  game->map = init_map(10, 10);
  if (!game->map) return NULL;

  game->keystate = SDL_GetKeyboardState(NULL);
  
  game->state = 1;
  game->delta_time = 1;

  return game;
}

void update_game(Game *game){
  update_player(game->player, game->delta_time);
}

void cleanup(Game *game, int exit_status){
  destroy_Renderer(game->renderer);
  destroy_player(game->player);
  destroy_map(game->map);
  SDL_Quit();
  free(game);
  game = NULL;
  exit(exit_status);
}
