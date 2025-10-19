#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

//macros for fast access of values
#define MAP_GET_VALUE(arr, z, x, y) ((arr)[(z)][(x)][(y)])

typedef struct{
  int ***buffer;
  size_t width;
  size_t height;
  size_t depth;
  SDL_Rect rect;
}Map;

//impure function modifies struct of type MapArray that is probably decalred in main.
Map* map_create(size_t depth, size_t width, size_t height);
void map_set_value(Map *map, size_t z, size_t x, size_t y, int value); 
void map_destroy(Map *mapArray);

#endif
