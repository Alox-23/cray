#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#define MAPX 10
#define MAPY 10

struct Map {
  int *buffer;
  size_t width;
  size_t height;
};

//impure function modifies struct of type MapArray that is probably decalred in main.
int init_map(struct Map *mapArray, size_t width, size_t height);
int* get_map_value(struct Map *mapArray, size_t x, size_t y);
void clean_map(struct Map *mapArray);

#endif
