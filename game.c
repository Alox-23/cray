#include "game.h"
#include "renderer.h"

struct Game* init_game(){
  struct Game* game = malloc(sizeof(struct Game));
  if (!game) return NULL;
  if (SDL_Init(SDL_INIT_EVERYTHING)){
    return NULL;
  }

  game->renderer = init_renderer();
  if (!game->renderer) return NULL;

  return game;
}

void cleanup(struct Game *game, int exit_status){
  renderer_cleanup(game->renderer);
  SDL_Quit();
  free(game);
  exit(exit_status);
}
