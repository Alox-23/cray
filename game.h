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
  int state;
}Game;

void cleanup(Game *game, int exit_status);
void update_game(Game *game);
Game* init_game(); 

#endif
