#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "renderer.h"
#include "player.h"
#include "map.h"

typedef struct{
  Player *player;
  Renderer *renderer;
  Map *map;
  const Uint8 *keystate;
  double delta_time;
  int state;
}Game;

void cleanup(Game *game, int exit_status);
void update_game(Game *game);
Game* init_game(); 

#endif
