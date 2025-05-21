#include "map.h"

//impure function that create a contiguous array of size width * height modifies a maps buffer array probably declared in main.c
int init_map(struct Map * map, size_t width, size_t height){
  if (map == NULL || width == 0 || height == 0){
    return 1;
  }
  map->buffer = malloc(sizeof(int) * height * width);
  map->height = height;
  map->width = width;
  return 0;
}

int* get_map_value(struct Map *map, size_t x, size_t y){
  if (map == NULL || x >= map->width || y >= map->height){
    return NULL;
  }
  return &map->buffer[y * map->width + x];
}

//free that array that was created in init_ma
void clean_map(struct Map *map){
  if (map != NULL){
    free(map->buffer);
    map->buffer = NULL;
    map->height = 0;
    map->width = 0;
  }
}
