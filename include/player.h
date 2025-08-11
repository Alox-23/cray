#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

#define ROTATION_SPEED 6
#define SPEED 5

typedef struct {
  Vector2 pos;
  Vector2 dir;
  Vector2 vel;
  SDL_Rect rect;  
  double a_vel;
  double angle;
  double player_size;
}Player;

Player* player_create();
void player_handle_input(Player *player, const Uint8 *keystate); 
void player_update(Player *player, double delta_time);
void player_destroy(Player* p);

#endif
