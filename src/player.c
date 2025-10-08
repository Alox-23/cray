#include "../include/player.h"
#include "../include/utils.h"

Player* player_create(){
  Player* p = malloc(sizeof(Player));
  if (!p) return NULL;
  
  p->pos.x = 5;
  p->pos.y = 5;
  p->pos_z = 0; //separate variable not store in Vector2 pos;

  p->dir.x = 1;
  p->dir.y = 0;

  p->vel_z = 0;

  p->plane.x = 0;
  p->plane.y = 0.66;

  p->player_size = 0.5;

  return p;
}

void player_handle_input(Player *player, const Uint8 *keystate){
  player->vel.x = 0;
  player->vel.y = 0;

  
  if (keystate[SDL_SCANCODE_E]){
    player->vel = scale(player->dir, SUPER_SPEED);
  }
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

  player->vel_z = 0;
  if (keystate[SDL_SCANCODE_F]){
    player->vel_z = -VEL_Z;
  }
  if (keystate[SDL_SCANCODE_R]){
    player->vel_z = VEL_Z;
  }

  player->a_vel = 0;
  if (keystate[SDL_SCANCODE_J]){
    player->a_vel = -ROTATION_SPEED;
  }
  if (keystate[SDL_SCANCODE_K]){
    player->a_vel = ROTATION_SPEED; 
  }
  if (keystate[SDL_SCANCODE_LEFT]){
    player->a_vel = -ROTATION_SPEED;
  }
  if (keystate[SDL_SCANCODE_RIGHT]){
    player->a_vel = ROTATION_SPEED; 
  }
}

void player_update(Player *player, double delta_time){
  player->pos = add(player->pos, scale(player->vel, delta_time));
  player->pos_z = player->pos_z + player->vel_z * delta_time;
  player->dir = rotate(player->dir, player->a_vel * delta_time);
  player->plane = rotate(player->plane, player->a_vel * delta_time);
  player->angle += player->a_vel * delta_time;

  player->rect.x = player->pos.x -5;
  player->rect.y = player->pos.y -5;
}

void player_destroy(Player* p){
  if (!p){
    return;
  }
  
  free(p);
  p = NULL;
}
