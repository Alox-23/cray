#include "input.h"

bool check_quit(SDL_Event event){
  while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        return true;
      }
    } 
  return false;
}
