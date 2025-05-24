#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

#define ROTATION_SPEED 0.2

typedef struct {
  Vector2 pos;
  Vector2 dir;
  double angle;
}Player;

Player* init_player();
void player_handle_input(Player *player, SDL_Event *event); 
void update_player(Player *player);
void destroy_player(Player* p);

#endif
