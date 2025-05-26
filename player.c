#include "player.h"
#include "utils.h"
#include "map.h"

Player* init_player(){
  Player* p = malloc(sizeof(Player));
  if (!p) return NULL;
  
  p->pos.x = 5;
  p->pos.y = 5;

  p->dir.x = 1;
  p->dir.y = 0;

  p->player_size = 0.5;

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

void update_player(Player *player, double delta_time, Map *map){
  player->vel = scale(player->vel, delta_time);
  
  size_t map_x = (size_t)player->pos.x + player->vel.x*1.6 + 0.5;
  size_t map_y = (size_t)player->pos.y + player->vel.y*1.6 + 0.5;
   
  if (!get_map_value(map, map_x, map_y)){
    player->pos = add(player->pos, player->vel);
  }

  printf("POS = (%lu, %lu) DIR = (%f, %f) VEL(%f, %f) COLL = %d\n", map_x, map_y, player->dir.x, player->dir.y, player->vel.x, player->vel.y, !get_map_value(map, map_x, map_y));
  player->dir = rotate(player->dir, player->a_vel * delta_time);
  player->angle += player->a_vel;

  player->rect.x = player->pos.x -5;
  player->rect.y = player->pos.y -5;
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
