#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <stdlib.h>

typedef struct {
  Vector2 pos;
  double angle;
}Player;

Player* init_player();

void destroy_player(Player* p);

#endif
