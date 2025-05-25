#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

#define ROTATION_SPEED 6
#define SPEED 100

typedef struct {
  Vector2 pos;
  Vector2 dir;
  Vector2 vel;
  
  double a_vel;
  double angle;
}Player;

Player* init_player();
void player_handle_input(Player *player, const Uint8 *keystate); 
void update_player(Player *player, double delta_time);
void destroy_player(Player* p);

#endif
