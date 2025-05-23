#include "game.h"

Game* init_game(){
  Game* game = malloc(sizeof(Game));
  if (!game) return NULL;

  if (SDL_Init(SDL_INIT_EVERYTHING)) return NULL;  

  game->renderer = init_renderer();
  if (!game->renderer) return NULL;

  game->player = init_player();
  if (!game->player) return NULL;

  return game;
}

void update_game(Game *game){
  update_player(game->player);
}

void cleanup(Game *game, int exit_status){
  destroy_renderer(game->renderer);
  destroy_player(game->player);
  SDL_Quit();
  free(game);
  game = NULL;
  exit(exit_status);
}
