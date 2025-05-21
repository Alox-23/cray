#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

//farward declaration
struct Renderer;

struct Game{
  struct Renderer *renderer;
  SDL_Event event;
};

void cleanup(struct Game *game, int exit_status);
struct Game* init_game(); 

#endif
