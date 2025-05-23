#include "player.h"

Player* init_player(){
  Player* p = malloc(sizeof(Player));
  if (!p) return NULL;
  
  p->pos.x = 50;
  p->pos.y = 50;

  p->dir.x = 1;
  p->dir.y = 0;

  return p;
}

void update_player(Player *player){
  printf("Player being updated!\n");
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
