#include "player.h"

Player* init_player(){
  Player* p = malloc(sizeof(Player));
  if (!p) return NULL;
  return p;
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
