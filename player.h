#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct {
  Vector2 pos;
  Vector2 dir;
}Player;

Player* init_player();
void update_player(Player *player);
void destroy_player(Player* p);

#endif
