#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

typedef struct{
  int *buffer;
  size_t width;
  SDL_Rect rect;
  size_t height;
}Map;

//impure function modifies struct of type MapArray that is probably decalred in main.
Map* map_create(size_t width, size_t height);
int map_get_value(Map *mapArray, size_t x, size_t y);
void map_set_value(Map *map, size_t x, size_t y, int value); 
void map_destroy(Map *mapArray);

#endif
