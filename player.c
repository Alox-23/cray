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

void player_handle_input(Player *player, SDL_Event *event){
  if (event->type == SDL_KEYDOWN){
    switch (event->key.keysym.scancode){
      case SDL_SCANCODE_LEFT:
        player->angle -= ROTATION_SPEED;
        player->dir = rotate(player->dir, -ROTATION_SPEED);
        break;
      case SDL_SCANCODE_RIGHT:
        player->angle += ROTATION_SPEED;
        player->dir = rotate(player->dir, ROTATION_SPEED);
        break;
      default:
        break;
    }
    if (event->key.keysym.scancode == SDL_SCANCODE_W){
      player->pos = add(player->pos, scale(player->dir, 5));
    }
  }
}

void update_player(Player *player){
  player->dir = normalize(player->dir);
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
