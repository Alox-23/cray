#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#define TEXTURE_COUNT 2

typedef struct{
  int *buffer;
  size_t width;
  SDL_Rect rect;
  size_t height;
  size_t texture_index;
  size_t texture_count;
  char **textures;
}Map;

//impure function modifies struct of type MapArray that is probably decalred in main.
Map* init_map(size_t width, size_t height, size_t texture_count);
const char* get_texture(Map *map, size_t texture_index);
int get_map_value(Map *mapArray, size_t x, size_t y);
void set_map_value(Map *map, size_t x, size_t y, int value); 
void clean_map(Map *mapArray);

#endif
