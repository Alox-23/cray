#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "map.h"

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

Player* init_player();
void player_handle_input(Player *player, const Uint8 *keystate); 
void update_player(Player *player, double delta_time, Map *map);
void destroy_player(Player* p);

#endif
