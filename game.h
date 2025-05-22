#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "renderer.h"
#include "player.h"

typedef struct{
  Player *player;
  Renderer *renderer;
  SDL_Event event;
}Game;

void cleanup(Game *game, int exit_status);
Game* init_game(); 

#endif
