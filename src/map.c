#include "../include/map.h"
#include <stdio.h>

Map* map_create(size_t depth, size_t width, size_t height){
  Map* map = malloc(sizeof(Map));  
  if (map == NULL || width == 0 || height == 0 || depth == 0){
    return NULL;
  }
   
  map->buffer = malloc(sizeof(int) * height * width * depth); 
  if (!map->buffer){
    return NULL;
  }
  
  map->height = height;
  map->width = width;
  map->depth = depth;

  for (size_t z = 0; z < map->depth; z++){
    for (size_t x = 0; x < map->width; x++){
      for (size_t y = 0; y < map->height; y++){
        //set_map_value(map, x, y, (y/(height/5))+1);
        if(x % 4 == 0 && y % 4 == 0){
          map_set_value(map, z, x, y, (z/(depth/4))+1);
        }
        else {
          map_set_value(map, z, x, y, 0);
        }
      }
    } 
  }
   
  return map;
}

int map_get_value(Map *map, size_t z, size_t x, size_t y){
  if (map == NULL || x >= map->width || y >= map->height || z >= map->depth){
    return 0;
  }
  return map->buffer[(z * map->height * map->width) + (y * map->width) + x];
}

void map_set_value(Map *map, size_t z, size_t x, size_t y, int value){
  if (map == NULL || x >= map->width || y >= map->height){
    return;
  }
  map->buffer[(z * map->height * map->width) + (y * map->width) + x] = value;
}

void map_destroy(Map *map){
  if (!map){
    return;
  }

  free(map->buffer);
  map->buffer = NULL;
  free(map);
  map = NULL;
}
