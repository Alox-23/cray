#include "player.h"
#include "input.h"
#include "utils.h"

Player* init_player(){
  Player* p = malloc(sizeof(Player));
  if (!p) return NULL;
  
  p->pos.x = 50;
  p->pos.y = 50;

  p->dir.x = 1;
  p->dir.y = 0;

  return p;
}

void player_handle_input(Player *player, const Uint8 *keystate){
  player->vel.x = 0;
  player->vel.y = 0;

  if (keystate[SDL_SCANCODE_W]){
    player->vel = scale(player->dir, SPEED);
  }
  if (keystate[SDL_SCANCODE_A]){
    player->vel = scale(rotate(player->dir, -M_PI/2), SPEED);
  }
  if (keystate[SDL_SCANCODE_S]){
    player->vel = scale(rotate(player->dir, -M_PI), SPEED);
  }
  if (keystate[SDL_SCANCODE_D]){
    player->vel = scale(rotate(player->dir, -3*M_PI/2), SPEED);
  }

  player->a_vel = 0;
  if (keystate[SDL_SCANCODE_LEFT]){
    player->a_vel = -ROTATION_SPEED;
  }
  if (keystate[SDL_SCANCODE_RIGHT]){
    player->a_vel = ROTATION_SPEED; 
  }
}

void update_player(Player *player, double delta_time){
  player->pos = add(player->pos, scale(player->vel, delta_time));
  player->dir = rotate(player->dir, player->a_vel * delta_time);
  player->angle += player->a_vel;
}

void destroy_player(Player* p){
  if (!p){
    return;
  }
  else{
    free(p);
    p = NULL;
  }
}
